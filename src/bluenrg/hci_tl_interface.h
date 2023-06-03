#ifndef HCI_TL_INTERFACE_H
#define HCI_TL_INTERFACE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l4xx.h"
#include "stm32l4xx_hal_exti.h"
#include "SensorTile_BlueNRG.h"

#define HCI_TL_SPI_EXTI_PORT  BNRG_SPI_EXTI_PORT
#define HCI_TL_SPI_EXTI_PIN   BNRG_SPI_EXTI_PIN
#define HCI_TL_SPI_EXTI_IRQn  BNRG_SPI_EXTI_IRQn

#define HCI_TL_SPI_IRQ_PORT   BNRG_SPI_IRQ_PORT
#define HCI_TL_SPI_IRQ_PIN    BNRG_SPI_IRQ_PIN

#define HCI_TL_SPI_CS_PORT    BNRG_SPI_CS_PORT
#define HCI_TL_SPI_CS_PIN     BNRG_SPI_CS_PIN

#define HCI_TL_RST_PORT       BNRG_SPI_RESET_PORT
#define HCI_TL_RST_PIN        BNRG_SPI_RESET_PIN

extern EXTI_HandleTypeDef     hexti0;
#define H_EXTI_0 hexti0int32_t HCI_TL_SPI_Init    (void* pConf);
int32_t HCI_TL_SPI_DeInit  (void);
int32_t HCI_TL_SPI_Reset   (void);
int32_t HCI_TL_SPI_Init(void);

#ifdef __cplusplus
}
#endif
#endif /* HCI_TL_INTERFACE_H */

