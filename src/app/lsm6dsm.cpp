#include "lsm6dsm.hpp"
#include "ble_serializer.hpp"
#include "LSM6DSM_ACC_GYRO_driver.h"
#include "SensorTile_gyro.h"

struct SpiCs {
  SpiCs(GPIO_TypeDef* port, uint32_t pin) : port(port), pin((uint16_t)pin) {
    HAL_GPIO_WritePin(port, (uint16_t)pin, GPIO_PIN_RESET);
  }
  ~SpiCs() {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
  }
  GPIO_TypeDef* port;
  uint16_t pin;
};

Lsm6dsm::Lsm6dsm() {
  init_spi();
  bang_regs();
  isInitialized = true;
  index = 0;
}

GyroAccTemp_Serialize Lsm6dsm::get_readout(void) {
  GyroAccTempReadout ro{};
  spi_read(LSM6DSM_ACC_GYRO_OUT_TEMP_L, (uint8_t*)(&ro), sizeof(ro));
  return {ro, index++, uwTick};
}

int Lsm6dsm::bang_regs() {
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

int Lsm6dsm::bang_reg(uint8_t reg, uint8_t mask_and, uint8_t mask_or) {
  uint8_t value = 0;
  if( 0 != spi_read(reg, &value))
    return -1;

  value &= ~mask_and;
  value |= mask_or;

  if( 0 != spi_write(reg, &value, 1) )
    return -2;

  return 0;
}

int Lsm6dsm::spi_write(uint8_t addr, uint8_t const * buff, uint16_t buffLen) {
  SpiCs cs(gpioPort, gpioCs.Pin);
  write_one(addr);
  for (int i = 0; i < buffLen; ++i) {
    write_one(buff[i]);
  }
  return 0;
}

int Lsm6dsm::spi_read(uint8_t addr, uint8_t * buff, uint16_t len) {
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

uint8_t Lsm6dsm::spi_read( uint8_t ReadAddr, uint8_t *val) {
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

void Lsm6dsm::init_spi() {
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

void Lsm6dsm::write_one(uint8_t val) {
  /* check TXE flag */
  while ((spiHandle.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);

  /* Write the data */
  *((__IO uint8_t*) &spiHandle.Instance->DR) = val;

  /* Wait BSY flag */
  while ((spiHandle.Instance->SR & SPI_FLAG_FTLVL) != SPI_FTLVL_EMPTY);
  while ((spiHandle.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);

}

