#include <stm32l476xx.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "SensorTile_BlueNRG.h"
#include "main.h"
#include "app_bluenrg_ms.h"

static RTC_HandleTypeDef RtcHandle;            /*!< RTC HANDLE */

int main_cpp(void);
int main(void)
{
  HAL_Init();
  SystemClock_Config();

  //RTC_Config();
  //RTC_TimeStampConfig();

  BSP_LED_Init(LED1);
  BSP_LED_Off(LED1);

  MX_BlueNRG_MS_Init();

  main_cpp();
}

void RTC_TimeRegulate(uint8_t hh, uint8_t mm, uint8_t ss)
{

  RTC_TimeTypeDef stimestructure;

  stimestructure.TimeFormat     = RTC_HOURFORMAT12_AM;
  stimestructure.Hours          = hh;
  stimestructure.Minutes        = mm;
  stimestructure.Seconds        = ss;
  stimestructure.SubSeconds     = 0;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

  if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, FORMAT_BIN) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}


/**
  * @brief  TIM period elapsed callback
  * @param  htim: TIM handle
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  UNUSED(htim);
    //CDC_TIM_PeriodElapsedCallback(htim);
}

