#ifndef __STM32_BLUENRG_BLE_H
#define __STM32_BLUENRG_BLE_H

#ifdef __cplusplus
 extern "C" {
#endif

  #include "stm32l4xx_hal.h"
  #define SYSCLK_FREQ 80000000

// SPI Instance
#define BNRG_SPI_INSTANCE           SPI1
#define BNRG_SPI_CLK_ENABLE()       __SPI1_CLK_ENABLE()

// SPI Configuration
#define BNRG_SPI_MODE               SPI_MODE_MASTER
#define BNRG_SPI_DIRECTION          SPI_DIRECTION_2LINES
#define BNRG_SPI_DATASIZE           SPI_DATASIZE_8BIT
#define BNRG_SPI_CLKPOLARITY        SPI_POLARITY_LOW
#define BNRG_SPI_CLKPHASE           SPI_PHASE_1EDGE
#define BNRG_SPI_NSS                SPI_NSS_SOFT
#define BNRG_SPI_FIRSTBIT           SPI_FIRSTBIT_MSB
#define BNRG_SPI_TIMODE             SPI_TIMODE_DISABLED
#define BNRG_SPI_CRCPOLYNOMIAL      7
#define BNRG_SPI_BAUDRATEPRESCALER  SPI_BAUDRATEPRESCALER_16
#define BNRG_SPI_CRCCALCULATION     SPI_CRCCALCULATION_DISABLED

// SPI Reset Pin: PH.0
#define BNRG_SPI_RESET_PIN          GPIO_PIN_0
#define BNRG_SPI_RESET_MODE         GPIO_MODE_OUTPUT_PP
#define BNRG_SPI_RESET_PULL         GPIO_PULLUP
#define BNRG_SPI_RESET_SPEED        GPIO_SPEED_LOW
#define BNRG_SPI_RESET_ALTERNATE    0
#define BNRG_SPI_RESET_PORT         GPIOH
#define BNRG_SPI_RESET_CLK_ENABLE() __GPIOH_CLK_ENABLE()

// SCLK: PA.5
#define BNRG_SPI_SCLK_PIN           GPIO_PIN_5
#define BNRG_SPI_SCLK_MODE          GPIO_MODE_AF_PP
#define BNRG_SPI_SCLK_PULL          GPIO_PULLDOWN
#define BNRG_SPI_SCLK_SPEED         GPIO_SPEED_HIGH
#define BNRG_SPI_SCLK_ALTERNATE     GPIO_AF5_SPI1
#define BNRG_SPI_SCLK_PORT          GPIOA
#define BNRG_SPI_SCLK_CLK_ENABLE()  __GPIOA_CLK_ENABLE()

// MISO (Master Input Slave Output): PA.6
#define BNRG_SPI_MISO_PIN           GPIO_PIN_6
#define BNRG_SPI_MISO_MODE          GPIO_MODE_AF_PP
#define BNRG_SPI_MISO_PULL          GPIO_NOPULL
#define BNRG_SPI_MISO_SPEED         GPIO_SPEED_HIGH
#define BNRG_SPI_MISO_ALTERNATE     GPIO_AF5_SPI1
#define BNRG_SPI_MISO_PORT          GPIOA
#define BNRG_SPI_MISO_CLK_ENABLE()  __GPIOA_CLK_ENABLE()

// MOSI (Master Output Slave Input): PA.7
#define BNRG_SPI_MOSI_PIN           GPIO_PIN_7
#define BNRG_SPI_MOSI_MODE          GPIO_MODE_AF_PP
#define BNRG_SPI_MOSI_PULL          GPIO_NOPULL
#define BNRG_SPI_MOSI_SPEED         GPIO_SPEED_HIGH
#define BNRG_SPI_MOSI_ALTERNATE     GPIO_AF5_SPI1
#define BNRG_SPI_MOSI_PORT          GPIOA
#define BNRG_SPI_MOSI_CLK_ENABLE()  __GPIOA_CLK_ENABLE()

// NSS/CSN/CS: PB.2
#define BNRG_SPI_CS_PIN             GPIO_PIN_2
#define BNRG_SPI_CS_MODE            GPIO_MODE_OUTPUT_PP
#define BNRG_SPI_CS_PULL            GPIO_PULLUP
#define BNRG_SPI_CS_SPEED           GPIO_SPEED_HIGH
#define BNRG_SPI_CS_ALTERNATE       0
#define BNRG_SPI_CS_PORT            GPIOB
#define BNRG_SPI_CS_CLK_ENABLE()    __GPIOB_CLK_ENABLE()

// IRQ: PC.5
#define BNRG_SPI_IRQ_PIN            GPIO_PIN_5
#define BNRG_SPI_IRQ_MODE           GPIO_MODE_IT_RISING
#define BNRG_SPI_IRQ_PULL           GPIO_NOPULL
#define BNRG_SPI_IRQ_SPEED          GPIO_SPEED_HIGH
#define BNRG_SPI_IRQ_ALTERNATE      0
#define BNRG_SPI_IRQ_PORT           GPIOC
#define BNRG_SPI_IRQ_CLK_ENABLE()   __GPIOC_CLK_ENABLE()

// EXTI External Interrupt for SPI
// NOTE: if you change the IRQ pin remember to implement a corresponding handler
// function like EXTI0_IRQHandler() in the user project
#define BNRG_SPI_EXTI_IRQn          EXTI9_5_IRQn
#define BNRG_SPI_EXTI_IRQHandler    EXTI9_5_IRQHandler
#define BNRG_SPI_EXTI_PIN           BNRG_SPI_IRQ_PIN
#define BNRG_SPI_EXTI_PORT          BNRG_SPI_IRQ_PORT
//#define RTC_WAKEUP_IRQHandler       RTC_WKUP_IRQHandler

void Enable_SPI_IRQ(void);
void Disable_SPI_IRQ(void);
void Clear_SPI_IRQ(void);
void Clear_SPI_EXTI_Flag(void);

void BNRG_SPI_Init(void);
void BlueNRG_RST(void);
void    BlueNRG_HW_Bootloader(void);
int32_t BlueNRG_SPI_Read_All(uint8_t *buffer,
                             uint16_t buff_size);

int32_t BlueNRG_Write(uint8_t* data, uint16_t size);
void us150Delay(void);
#ifdef __cplusplus
}
#endif

#endif /* __STM32_BLUENRG_BLE_H */

