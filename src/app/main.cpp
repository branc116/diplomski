#include "LSM6DSM_ACC_GYRO_driver.h"
#include "SensorTile.h"
#include "SensorTile_BlueNRG.h"
#include "SensorTile_gyro.h"
#include "app_bluenrg_ms.h"
#include "bluenrg_aci_const.h"
#include "cube_hal.h"
#include "sample_service.h"
#include <stddef.h>
#include <stdint.h>
#include <stm32l476xx.h>
#include <stm32l4xx_hal.h>
#include <stm32l4xx_hal_def.h>
#include <stm32l4xx_hal_gpio.h>

template<class T, size_t buffSize>
struct CircBuffer {
  T& push(T el) {
    auto& ret = buffer[pos++] = el;
    if (pos >= buffSize) pos = 0;
    if (len < buffSize) ++len;
    return ret;
  }
  T pop() {
    int index = (pos - len + buffSize) % buffSize;
    auto ret = buffer[index];
    if (len > 0) --len;
    return ret;
  }
  const T& peek() {
    int index = (pos - len + buffSize) % buffSize;
    return buffer[index];
  }
  size_t pos = 0;
  size_t len = 0;
  T buffer[buffSize] = {};
};

struct SpiCs {
  SpiCs(GPIO_TypeDef* port, uint32_t pin) : port(port), pin(pin) {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
  }
  ~SpiCs() {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
  }
  GPIO_TypeDef* port;
  uint32_t pin;
};

template<typename TSelf>
struct BaseBLEChar : public TSelf {
  //    NODATA  = 0
  //    USHORT  = 4
  //    SSHORT  = 6
  //    USHORT2 = 8
  //    SSHORT2 = 10
  //    ULONG   = 12
  //    SLONG   = 14
  //    SFLOAT   = 15
  void fill_controll(uint8_t* buff) {
    static_assert(sizeof(TSelf) <= 20);
    for (int i = 0; i < 17; ++i) {
      buff[i] = (i % 2) * 0xFF;
    }
    this->fill_meta_data(buff);
  }
  bool send_next(uint8_t* buff) {
    if (send_next_controll < uwTick) {
      fill_controll(buff);
      return true;
    }
    if (this->has_next()) {
      this->next();
      memcpy(buff, (TSelf*)this, sizeof(TSelf));
      return true;
    }
    return false;
  }
  void confirm() {
    if (send_next_controll < uwTick) send_next_controll = uwTick + 3000;
    else this->confirm_data();
  }
  bool is_empty() {
    return this->has_next() || send_next_controll < uwTick;
  }
  uint32_t send_next_controll;
};

struct [[gnu::packed]] GyroReadout  {
  uint16_t temperature;
  int16_t acceleration[3];
  int16_t angular[3];
};

struct [[gnu::packed]] GyroReadoutBLE {
  static CircBuffer<GyroReadoutBLE, 16> circ_buff;
  void fill_meta_data(uint8_t* buff) {
    buff[17] = 0x4A; //ui16, 2*i16
    buff[18] = 0xAA; //4*i16
    buff[19] = 0x4C; //ui16, i32
  }
  void next() {
    *this = circ_buff.peek();
  }
  bool has_next() { return circ_buff.len > 0; }
  void confirm_data() {
    circ_buff.pop();
  }
  GyroReadout ro;
  uint16_t index;
  uint32_t timestamp;
};

CircBuffer<GyroReadoutBLE, 16> GyroReadoutBLE::circ_buff;

using Send_GyroReadoutBLE = BaseBLEChar<GyroReadoutBLE>;

struct [[gnu::packed]] BlueMetaData {
  void fill_meta_data(uint8_t* buff) {
    // 7*uint16_t, uint32_t
    buff[17] = 0x88; //ui16, 2*i16
    buff[18] = 0x84; //4*i16
    buff[19] = 0xC0;
  }
  bool has_next() { return true; }
  void next() {
    changed_attrs_count = blue_state.changed_attrs_count;
    gat_notification_cb_count = blue_state.gat_notification_cb_count;
    number_of_resets = blue_state.number_of_resets;
    number_of_attempts_to_read_data = blue_state.number_of_attempts_to_read_data;
    blue_number_of_times_in_interupt = blue_state.blue_number_of_times_in_interupt;
    number_of_send_attempts = blue_state.number_of_send_attempts;
    number_of_unsuccessfull_sends = blue_state.number_of_unsuccessfull_sends;
    timestamp = uwTick;
  }
  void confirm_data() {}
  uint16_t changed_attrs_count, gat_notification_cb_count;
  uint16_t number_of_resets, number_of_attempts_to_read_data;
  uint16_t blue_number_of_times_in_interupt;
  uint16_t number_of_send_attempts, number_of_unsuccessfull_sends;
  uint32_t timestamp;
};

using Send_BlueMetaData = BaseBLEChar<BlueMetaData>;

class Lsm6dsm {
  public:
    Lsm6dsm() {
      init_spi();
      bang_regs();
      isInitialized = true;
      index = 0;
    }

    GyroReadoutBLE get_readout(void) {
      GyroReadout ro{};
      spi_read(LSM6DSM_ACC_GYRO_OUT_TEMP_L, (uint8_t*)(&ro), sizeof(ro));
      return {ro, index++, uwTick};
    }

    int bang_regs() {
      int res = 0;
      uint8_t d = 0x0C;
      spi_write(LSM6DSM_ACC_GYRO_CTRL3_C, &d, 1);
      res += bang_reg(LSM6DSM_ACC_GYRO_CTRL3_C, LSM6DSM_ACC_GYRO_IF_INC_MASK, LSM6DSM_ACC_GYRO_IF_INC_ENABLED);
      res += bang_reg(LSM6DSM_ACC_GYRO_CTRL3_C, LSM6DSM_ACC_GYRO_BDU_MASK, LSM6DSM_ACC_GYRO_BDU_BLOCK_UPDATE);
      res += bang_reg(LSM6DSM_ACC_GYRO_FIFO_CTRL5, LSM6DSM_ACC_GYRO_FIFO_MODE_MASK, LSM6DSM_ACC_GYRO_FIFO_MODE_DYN_STREAM_2);
      res += bang_reg(LSM6DSM_ACC_GYRO_FIFO_CTRL5, LSM6DSM_ACC_GYRO_ODR_FIFO_MASK, LSM6DSM_ACC_GYRO_ODR_FIFO_13300Hz);
      res += bang_reg(LSM6DSM_ACC_GYRO_CTRL2_G, LSM6DSM_ACC_GYRO_FS_G_MASK, LSM6DSM_ACC_GYRO_FS_G_2000dps);
      res += bang_reg(LSM6DSM_ACC_GYRO_CTRL2_G, LSM6DSM_ACC_GYRO_ODR_G_MASK, LSM6DSM_ACC_GYRO_ODR_G_6660Hz);
      res += bang_reg(LSM6DSM_ACC_GYRO_CTRL1_XL, LSM6DSM_ACC_GYRO_FS_XL_MASK, LSM6DSM_ACC_GYRO_FS_XL_8g);
      res += bang_reg(LSM6DSM_ACC_GYRO_CTRL1_XL, LSM6DSM_ACC_GYRO_ODR_XL_MASK, LSM6DSM_ACC_GYRO_ODR_XL_6660Hz);
      res += bang_reg(LSM6DSM_ACC_GYRO_CTRL4_C, LSM6DSM_ACC_GYRO_I2C_DISABLE_MASK, LSM6DSM_ACC_GYRO_I2C_DISABLE_SPI_ONLY);
      return res;
    }

    int bang_reg(uint8_t reg, uint8_t mask_and, uint8_t mask_or){
      uint8_t value = 0;
      if( 0 != spi_read(reg, &value))
        return -1;

      value &= ~mask_and;
      value |= mask_or;

      if( 0 != spi_write(reg, &value, 1) )
        return -2;

      return 0;
    }

    int spi_write(uint8_t addr, uint8_t const * buff, uint16_t buffLen) {
      SpiCs cs(gpioPort, gpioCs.Pin);
      write_one(addr);
      for (int i = 0; i < buffLen; ++i) {
        write_one(buff[i]);
      }
      return 0;
    }

    int spi_read(uint8_t addr, uint8_t * buff, uint16_t len) {
      SpiCs cs(gpioPort, gpioCs.Pin);
      write_one(addr | 0x80);
      /* Disable the SPI and change the data line to input */
      __HAL_SPI_DISABLE(&spiHandle);
      SPI_1LINE_RX(&spiHandle);
      //__disable_irq();
      __HAL_SPI_ENABLE(&spiHandle);
      /* Transfer loop */
      while (len > 1U)
      {
        /* Check the RXNE flag */
        if (spiHandle.Instance->SR & SPI_FLAG_RXNE)
        {
          /* read the received data */
          *(buff++) = *(__IO uint8_t *) &spiHandle.Instance->DR;
          len--;
        }
      }
      /* In master RX mode the clock is automaticaly generated on the SPI enable.
         So to guarantee the clock generation for only one data, the clock must be
         disabled after the first bit and before the latest bit of the last Byte received */
      /* __DSB instruction are inserted to garantee that clock is Disabled in the right timeframe */
      __DSB();
      __DSB();
      __HAL_SPI_DISABLE(&spiHandle);
      //__enable_irq();
      while ((spiHandle.Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE);
      /* read the received data */
      *buff = *(__IO uint8_t *) &spiHandle.Instance->DR;
      while ((spiHandle.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);
      /* Change the data line to output and enable the SPI */
      SPI_1LINE_TX(&spiHandle);
      __HAL_SPI_ENABLE(&spiHandle);
      return 0;
    }

    uint8_t spi_read( uint8_t ReadAddr, uint8_t *val)
    {
      SpiCs cs(gpioPort, gpioCs.Pin);

      /* Write Reg Address */
      write_one(ReadAddr | 0x80);

      /* Disable the SPI and change the data line to input */
      __HAL_SPI_DISABLE(&spiHandle);
      SPI_1LINE_RX(&spiHandle);

      /* In master RX mode the clock is automaticaly generated on the SPI enable.
      So to guarantee the clock generation for only one data, the clock must be
      disabled after the first bit and before the latest bit */
      /* Interrupts should be disabled during this operation */

      __disable_irq();

      __HAL_SPI_ENABLE(&spiHandle);
      __asm("dsb\n");
      __asm("dsb\n");
      __HAL_SPI_DISABLE(&spiHandle);

      __enable_irq();

      while ((spiHandle.Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE);
      /* read the received data */
      *val = *(__IO uint8_t *) &spiHandle.Instance->DR;
      while ((spiHandle.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);

      /* Change the data line to output and enable the SPI */
      SPI_1LINE_TX(&spiHandle);
      __HAL_SPI_ENABLE(&spiHandle);

      return COMPONENT_OK;
    }
  private:
    void init_spi() {
      GPIO_InitTypeDef GPIO_InitStruct{};

      if(HAL_SPI_GetState(&spiHandle) == HAL_SPI_STATE_RESET )
      {
        SENSORTILE_SENSORS_SPI_CLK_ENABLE();
        SENSORTILE_SENSORS_SPI_GPIO_CLK_ENABLE();

        GPIO_InitStruct.Pin = SENSORTILE_SENSORS_SPI_MOSI_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(SENSORTILE_SENSORS_SPI_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = SENSORTILE_SENSORS_SPI_SCK_Pin;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(SENSORTILE_SENSORS_SPI_Port, &GPIO_InitStruct);

        spiHandle.Instance = SENSORTILE_SENSORS_SPI;
        spiHandle.Init.Mode = SPI_MODE_MASTER;
        spiHandle.Init.Direction = SPI_DIRECTION_1LINE;
        spiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
        spiHandle.Init.CLKPolarity = SPI_POLARITY_HIGH;
        spiHandle.Init.CLKPhase = SPI_PHASE_2EDGE;
        spiHandle.Init.NSS = SPI_NSS_SOFT;
        spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
        spiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
        spiHandle.Init.TIMode = SPI_TIMODE_DISABLED;
        spiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
        spiHandle.Init.CRCPolynomial = 7;
        spiHandle.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
        spiHandle.Init.NSSPMode = SPI_NSS_PULSE_DISABLED;
        HAL_SPI_Init(&spiHandle);

        SPI_1LINE_TX(&spiHandle);
        __HAL_SPI_ENABLE(&spiHandle);
      }
      gpioCs.Speed = GPIO_SPEED_HIGH;
      gpioCs.Pull = GPIO_NOPULL;
      gpioCs.Mode = GPIO_MODE_OUTPUT_PP;

      SENSORTILE_LSM6DSM_SPI_CS_GPIO_CLK_ENABLE();
      gpioCs.Pin = SENSORTILE_LSM6DSM_SPI_CS_Pin;
      gpioPort = SENSORTILE_LSM6DSM_SPI_CS_Port;

      /* Set the pin before init to avoid glitch */
      HAL_GPIO_WritePin(gpioPort, gpioCs.Pin, GPIO_PIN_SET);
      HAL_GPIO_Init(gpioPort, &gpioCs);
    }

    void write_one(uint8_t val) {
      /* check TXE flag */
      while ((spiHandle.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);

      /* Write the data */
      *((__IO uint8_t*) &spiHandle.Instance->DR) = val;

      /* Wait BSY flag */
      while ((spiHandle.Instance->SR & SPI_FLAG_FTLVL) != SPI_FTLVL_EMPTY);
      while ((spiHandle.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);

    }
    uint8_t isInitialized = 0;
    SPI_HandleTypeDef spiHandle{};
    GPIO_InitTypeDef gpioCs{};
    GPIO_TypeDef* gpioPort{};
    uint16_t index = 0;
};
uint32_t last_sent_controll = 0;

Send_BlueMetaData send_ble;
Send_GyroReadoutBLE send_ro;
#define CHAR_LIST(X) X(send_ble, 0) \
  X(send_ro, 1)

extern "C" bool fill(int char_id, uint8_t* buff) {
#define X(name, id) case id: return name.send_next(buff);
  switch (char_id) {
    CHAR_LIST(X)
    default: return false;
  }
#undef X
}

extern "C" void confirm_sent(int char_id) {
  if (char_id == 0) {
    send_ro.confirm();
  } else if (char_id == 1) {
    send_ble.confirm();
  }
}

extern "C" bool is_empty(int char_id) {
  if (char_id == 0) {
    return send_ro.is_empty();
  } else if (char_id == 1) {
    return send_ble.is_empty();
  } else {
    return true;
  }
}

static int entry(void) {
 Lsm6dsm l{};
 int t = (int)uwTick;
 int t2 = (int)uwTick + 2;
 initialize_blue_char_collection(&blue_state.chars);
 while(1) {
    if (blue_state.status == USER_PROCESS_STATUS__RESET) {
      MX_BlueNRG_MS_Init();
      blue_state.number_of_resets++;
    }else if (blue_state.everything_inited && blue_state.status == USER_PROCESS_STATUS__CONNECTED)  {
      if (!blue_send_next(&blue_state.chars)) {
        t = uwTick;
      }
      else if ((int)uwTick - t > 90) {
        hci_notify_asynch_evt();
        t = (int)uwTick;
      }
      if (t2 < (int)uwTick) {
        GyroReadoutBLE::circ_buff.push(l.get_readout());
        t2 = uwTick + 10;
      }
    }
  }
}

extern "C" int main_cpp(void) {
  entry();
  return 0;
}

