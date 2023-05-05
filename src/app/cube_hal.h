#ifndef _CUBE_HAL_H_
#define _CUBE_HAL_H_

#ifdef __cplusplus
 extern "C" {
#endif

#define USE_SENSORTILE

#ifdef USE_SENSORTILE
#include "stm32l4xx_hal.h"
#include "SensorTile.h"
#include "stm32l4xx_hal_conf.h"
#include "stm32l4xx_hal_def.h"
#endif

#ifdef USE_SENSORTILE

#define RTC_ASYNCH_PREDIV_LSI  0x7F
#define RTC_SYNCH_PREDIV_LSI   0xF9

#define RTC_ASYNCH_PREDIV_LSE  0x7F
#define RTC_SYNCH_PREDIV_LSE   0xFF
#endif

void SystemClock_Config(void);
uint32_t Get_DMA_Flag_Status(DMA_HandleTypeDef *handle_dma); // TODO: Check if needed
uint32_t Get_DMA_Counter(DMA_HandleTypeDef *handle_dma);     // TODO: Check if needed
void Config_DMA_Handler(DMA_HandleTypeDef *handle_dma);      // TODO: Check if needed

#ifdef __cplusplus
}
#endif

#endif /* _CUBE_HAL_H_ */

