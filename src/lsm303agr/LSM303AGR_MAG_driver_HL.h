#ifndef __LSM303AGR_MAG_DRIVER_HL_H
#define __LSM303AGR_MAG_DRIVER_HL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "magnetometer.h"

/* Include magnetic sensor component drivers. */
#include "LSM303AGR_MAG_driver.h"
#include "LSM303AGR_Combo_driver_HL.h"
#define LSM303AGR_MAG_SENSITIVITY_FOR_FS_50G  1.5

/**< Sensitivity value for 16 gauss full scale [mgauss/LSB] */

typedef struct
{
  LSM303AGR_Combo_Data_t *comboData;       /* Combo data to manage in software SPI 3-Wire initialization of the combo sensors */
} LSM303AGR_M_Data_t;

extern MAGNETO_Drv_t LSM303AGR_M_Drv;

#ifdef __cplusplus
}
#endif

#endif /* __LSM303AGR_MAG_DRIVER_HL_H */

