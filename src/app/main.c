#include <stm32l476xx.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "SensorTile_BlueNRG.h"
#include "main.h"
#include "vcom.h"
#include "DemoSerial.h"
#include "app_bluenrg_ms.h"

typedef struct displayFloatToInt_s {
  int8_t   sign;     /* 0 means positive, 1 means negative*/
  uint32_t out_int;
  uint32_t out_dec;
} displayFloatToInt_t;
#define DATA_SIZE  256

uint32_t Sensors_Enabled  = 0xFFFFFF;                  /*!< Sensors Enabled/Disabled */
uint32_t DataTxPeriod     = 50;                 /*!< TX DATA Period */
uint8_t  DataLoggerActive = 1;                  /*!< DataLogger ON/OFF */

static RTC_HandleTypeDef RtcHandle;            /*!< RTC HANDLE */

static int RTC_SYNCH_PREDIV;

static void RTC_Config(void);
static void RTC_TimeStampConfig(void);

int main_cpp(void);
int main(void)
{
  HAL_Init();
  SystemClock_Config();

  RTC_Config();
  RTC_TimeStampConfig();

  BSP_LED_Init(LED1);
  BSP_LED_Off(LED1);

  volatile int i = 0;
  BNRG_SPI_Init();
  uint32_t res = MX_BlueNRG_MS_Init();
  while (res) {
    ++i;
    res = MX_BlueNRG_MS_Init();
  }

  main_cpp();
}


/**
  * @brief  Configures the RTC
  */
static void RTC_Config(void)
{
  RtcHandle.Instance = RTC;

  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
    - Hour Format    = Format 12
    - Asynch Prediv  = Value according to source clock
    - Synch Prediv   = Value according to source clock
    - OutPut         = Output Disable
    - OutPutPolarity = High Polarity
    - OutPutType     = Open Drain
   */
  RtcHandle.Init.HourFormat     = RTC_HOURFORMAT_12;
  RtcHandle.Init.AsynchPrediv   = RTC_ASYNCH_PREDIV_LSE;
  RtcHandle.Init.SynchPrediv    = RTC_SYNCH_PREDIV_LSE;
  RtcHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;

  RTC_SYNCH_PREDIV = RTC_SYNCH_PREDIV_LSE;

  if (HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  Configures the current time and date
  */
static void RTC_TimeStampConfig(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;

  /* Configure the Date using BCD format */
  /* Set Date: Monday January 1st 2000 */
  sdatestructure.Year    = 0x00;
  sdatestructure.Month   = RTC_MONTH_JANUARY;
  sdatestructure.Date    = 0x01;
  sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;

  if (HAL_RTC_SetDate(&RtcHandle, &sdatestructure, FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Configure the Time using BCD format */
  /* Set Time: 00:00:00 */
  stimestructure.Hours          = 0x00;
  stimestructure.Minutes        = 0x00;
  stimestructure.Seconds        = 0x00;
  stimestructure.TimeFormat     = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

  if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
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
    CDC_TIM_PeriodElapsedCallback(htim);
}

