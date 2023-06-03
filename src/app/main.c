#include <stm32l476xx.h>
#include "SensorTile_BlueNRG.h"
#include "main.h"
#include "app_bluenrg_ms.h"

int main_cpp(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  BSP_LED_Init(LED1);
  BSP_LED_Off(LED1);

  MX_BlueNRG_MS_Init();

  main_cpp();
}

