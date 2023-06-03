/**
  * @brief   This file provides the implementation for all functions prototypes
  *          for the STM32 BlueNRG HCI Transport Layer interface
  */

//#include "RTE_Components.h"

#include "SensorTile_BlueNRG.h"
#include "hci_tl.h"
#include "stm32l4xx_ll_bus.h"

EXTI_HandleTypeDef hexti0;

/******************** IO Operation and BUS services ***************************/

/**
 * @brief  Initializes the peripherals communication with the BlueNRG
 *         Expansion Board (via SPI, I2C, USART, ...)
 *
 * @param  void* Pointer to configuration struct
 * @retval int32_t Status
 */
int32_t HCI_TL_SPI_Init(void)
{
  //Disable_SPI_IRQ();
  BNRG_SPI_Init();
  Clear_SPI_IRQ();
  return 0;
}

/**
 * @brief  DeInitializes the peripherals communication with the BlueNRG
 *         Expansion Board (via SPI, I2C, USART, ...)
 *
 * @retval int32_t 0
 */
int32_t HCI_TL_SPI_DeInit(void)
{
  HAL_GPIO_DeInit(HCI_TL_SPI_EXTI_PORT, HCI_TL_SPI_EXTI_PIN);
  HAL_GPIO_DeInit(HCI_TL_SPI_CS_PORT, HCI_TL_SPI_CS_PIN);
  HAL_GPIO_DeInit(HCI_TL_RST_PORT, HCI_TL_RST_PIN);
  return 0;
}

