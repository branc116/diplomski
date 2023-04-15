#ifndef __LSM303AGR_COMBO_DRIVER_HL_H
#define __LSM303AGR_COMBO_DRIVER_HL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/** @addtogroup LSM303AGR_COMBO_Public_Constants Public constants
 */
#define LSM303AGR_SENSORS_MAX_NUM  1     /**< LSM303AGR max number of instances */

/**
 * @brief LSM303AGR combo specific data internal structure definition
 */
typedef struct
{
  uint8_t isAccInitialized;
  uint8_t isMagInitialized;
} LSM303AGR_Combo_Data_t;

extern LSM303AGR_Combo_Data_t LSM303AGR_Combo_Data[LSM303AGR_SENSORS_MAX_NUM];

#ifdef __cplusplus
}
#endif

#endif /* __LSM303AGR_COMBO_DRIVER_HL_H */

