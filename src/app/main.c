#include <stm32l476xx.h>
#include "SensorTile_BlueNRG.h"
#include "cube_hal.h"
#include "SensorTile.h"

int main_cpp(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  BSP_LED_Init(LED1);
  BSP_LED_Off(LED1);

  main_cpp();
}

