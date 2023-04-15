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
int32_t HCI_TL_SPI_Init(void* pConf)
{
//  GPIO_InitTypeDef GPIO_InitStruct;
//
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//
//  /* Configure EXTI Line */
//  GPIO_InitStruct.Pin = HCI_TL_SPI_EXTI_PIN;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(HCI_TL_SPI_EXTI_PORT, &GPIO_InitStruct);
//
//  /* Configure RESET Line */
//  GPIO_InitStruct.Pin =  HCI_TL_RST_PIN;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(HCI_TL_RST_PORT, &GPIO_InitStruct);
//
//  /* Configure CS */
//  GPIO_InitStruct.Pin = HCI_TL_SPI_CS_PIN;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(HCI_TL_SPI_CS_PORT, &GPIO_InitStruct);

  BNRG_SPI_Init();
  //Enable_SPI_IRQ();
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

/**
 * @brief  Reports if the BlueNRG has data for the host micro.
 *
 * @retval int32_t: 1 if data are present, 0 otherwise
 */
static int32_t IsDataAvailable(void)
{
  return (HAL_GPIO_ReadPin(HCI_TL_SPI_EXTI_PORT, HCI_TL_SPI_EXTI_PIN) == GPIO_PIN_SET);
}

/***************************** hci_tl_interface main functions *****************************/
/**
 * @brief  Register hci_tl_interface IO bus services
 *
 */
void hci_tl_lowlevel_init(void)
{
  tHciIO fops;

  /* Register IO bus services */
  fops.Init    = HCI_TL_SPI_Init;
  fops.DeInit  = HCI_TL_SPI_DeInit;
  fops.Send    = BlueNRG_Write;
  fops.Receive = BlueNRG_SPI_Read_All;
  fops.Reset   = BlueNRG_RST;
  fops.GetTick = HAL_GetTick;

  hci_register_io_bus (&fops);

  /* Register event irq handler */
//  HAL_EXTI_GetHandle(&hexti0, EXTI_LINE_1);
//  HAL_EXTI_RegisterCallback(&hexti0, HAL_EXTI_COMMON_CB_ID, hci_tl_lowlevel_isr);
//  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  //HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

/**
  * @brief HCI Transport Layer Low Level Interrupt Service Routine
  *
  */
void hci_tl_lowlevel_isr(void)
{
  /* Call hci_notify_asynch_evt() */
  while(IsDataAvailable())
  {
    if (hci_notify_asynch_evt(NULL))
    {
      return;
    }
  }

}
