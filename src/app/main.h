#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cube_hal.h"
#include "SensorTile.h"
#include "SensorTile_accelero.h"
#include "SensorTile_gyro.h"
#include "SensorTile_magneto.h"
#include "SensorTile_pressure.h"
#include "SensorTile_temperature.h"
#include "SensorTile_humidity.h"

void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

