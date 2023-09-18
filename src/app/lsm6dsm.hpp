#pragma once
#include "ble_serializer.hpp"

class Lsm6dsm {
  public:
    Lsm6dsm();
    GyroAccTemp_Serialize get_readout(void);

    int bang_regs();
    int bang_reg(uint8_t reg, uint8_t mask_and, uint8_t mask_or);
    int spi_write(uint8_t addr, uint8_t const * buff, uint16_t buffLen);
    int spi_read(uint8_t addr, uint8_t * buff, uint16_t len);
    uint8_t spi_read( uint8_t ReadAddr, uint8_t *val);

  private:
    void init_spi();
    void write_one(uint8_t val);

    uint8_t isInitialized = 0;
    SPI_HandleTypeDef spiHandle{};
    GPIO_InitTypeDef gpioCs{};
    GPIO_TypeDef* gpioPort{};
    uint16_t index = 0;
};
