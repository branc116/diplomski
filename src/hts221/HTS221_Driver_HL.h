#ifndef __HTS221_DRIVER_HL_H
#define __HTS221_DRIVER_HL_H

#ifdef __cplusplus
extern "C" {
#endif
#include "humidity.h"
#include "temperature.h"

#include "HTS221_Driver.h"
#define HTS221_SENSORS_MAX_NUM  1     /**< HTS221 max number of instances */

#define HTS221_ADDRESS_DEFAULT  0xBE  /**< HTS221 I2C Address */

typedef struct
{
  uint8_t isHumInitialized;
  uint8_t isTempInitialized;
  uint8_t isHumEnabled;
  uint8_t isTempEnabled;
} HTS221_Combo_Data_t;

/**
 * @brief HTS221 humidity specific data internal structure definition
 */

typedef struct
{
  HTS221_Combo_Data_t *comboData;       /* Combo data to manage in software enable/disable of the combo sensors */
} HTS221_H_Data_t;
/**
 * @brief HTS221 temperature specific data internal structure definition
 */

typedef struct
{
  HTS221_Combo_Data_t *comboData;       /* Combo data to manage in software enable/disable of the combo sensors */
} HTS221_T_Data_t;

extern HUMIDITY_Drv_t HTS221_H_Drv;
extern TEMPERATURE_Drv_t HTS221_T_Drv;
extern HTS221_Combo_Data_t HTS221_Combo_Data[HTS221_SENSORS_MAX_NUM];

#ifdef __cplusplus
}
#endif

#endif /* __HTS221_DRIVER_HL_H */

