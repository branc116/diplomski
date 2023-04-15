#ifndef __LSM6DSM_ACC_GYRO_DRIVER_HL_H
#define __LSM6DSM_ACC_GYRO_DRIVER_HL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "accelerometer.h"
#include "gyroscope.h"

/* Include accelero sensor component drivers. */
#include "LSM6DSM_ACC_GYRO_driver.h"
#define LSM6DSM_SENSORS_MAX_NUM  1     /**< LSM6DSM max number of instances */

#define LSM6DSM_ACC_SENSITIVITY_FOR_FS_2G   0.061  /**< Sensitivity value for 2 g full scale [mg/LSB] */
#define LSM6DSM_ACC_SENSITIVITY_FOR_FS_4G   0.122  /**< Sensitivity value for 4 g full scale [mg/LSB] */
#define LSM6DSM_ACC_SENSITIVITY_FOR_FS_8G   0.244  /**< Sensitivity value for 8 g full scale [mg/LSB] */
#define LSM6DSM_ACC_SENSITIVITY_FOR_FS_16G  0.488  /**< Sensitivity value for 16 g full scale [mg/LSB] */

#define LSM6DSM_GYRO_SENSITIVITY_FOR_FS_125DPS   04.375  /**< Sensitivity value for 125 dps full scale [mdps/LSB] */
#define LSM6DSM_GYRO_SENSITIVITY_FOR_FS_245DPS   08.750  /**< Sensitivity value for 245 dps full scale [mdps/LSB] */
#define LSM6DSM_GYRO_SENSITIVITY_FOR_FS_500DPS   17.500  /**< Sensitivity value for 500 dps full scale [mdps/LSB] */
#define LSM6DSM_GYRO_SENSITIVITY_FOR_FS_1000DPS  35.000  /**< Sensitivity value for 1000 dps full scale [mdps/LSB] */
#define LSM6DSM_GYRO_SENSITIVITY_FOR_FS_2000DPS  70.000  /**< Sensitivity value for 2000 dps full scale [mdps/LSB] */

#define LSM6DSM_PEDOMETER_THRESHOLD_LOW       0x00  /**< Lowest  value of pedometer threshold */
#define LSM6DSM_PEDOMETER_THRESHOLD_MID_LOW   0x07
#define LSM6DSM_PEDOMETER_THRESHOLD_MID       0x0F
#define LSM6DSM_PEDOMETER_THRESHOLD_MID_HIGH  0x17
#define LSM6DSM_PEDOMETER_THRESHOLD_HIGH      0x1F  /**< Highest value of pedometer threshold */

#define LSM6DSM_WAKE_UP_THRESHOLD_LOW       0x01  /**< Lowest  value of wake up threshold */
#define LSM6DSM_WAKE_UP_THRESHOLD_MID_LOW   0x0F
#define LSM6DSM_WAKE_UP_THRESHOLD_MID       0x1F
#define LSM6DSM_WAKE_UP_THRESHOLD_MID_HIGH  0x2F
#define LSM6DSM_WAKE_UP_THRESHOLD_HIGH      0x3F  /**< Highest value of wake up threshold */

#define LSM6DSM_TAP_THRESHOLD_LOW       0x01  /**< Lowest  value of wake up threshold */
#define LSM6DSM_TAP_THRESHOLD_MID_LOW   0x08
#define LSM6DSM_TAP_THRESHOLD_MID       0x10
#define LSM6DSM_TAP_THRESHOLD_MID_HIGH  0x18
#define LSM6DSM_TAP_THRESHOLD_HIGH      0x1F  /**< Highest value of wake up threshold */

#define LSM6DSM_TAP_SHOCK_TIME_LOW       0x00  /**< Lowest  value of wake up threshold */
#define LSM6DSM_TAP_SHOCK_TIME_MID_LOW   0x01
#define LSM6DSM_TAP_SHOCK_TIME_MID_HIGH  0x02
#define LSM6DSM_TAP_SHOCK_TIME_HIGH      0x03  /**< Highest value of wake up threshold */

#define LSM6DSM_TAP_QUIET_TIME_LOW       0x00  /**< Lowest  value of wake up threshold */
#define LSM6DSM_TAP_QUIET_TIME_MID_LOW   0x01
#define LSM6DSM_TAP_QUIET_TIME_MID_HIGH  0x02
#define LSM6DSM_TAP_QUIET_TIME_HIGH      0x03  /**< Highest value of wake up threshold */

#define LSM6DSM_TAP_DURATION_TIME_LOW       0x00  /**< Lowest  value of wake up threshold */
#define LSM6DSM_TAP_DURATION_TIME_MID_LOW   0x04
#define LSM6DSM_TAP_DURATION_TIME_MID       0x08
#define LSM6DSM_TAP_DURATION_TIME_MID_HIGH  0x0C
#define LSM6DSM_TAP_DURATION_TIME_HIGH      0x0F  /**< Highest value of wake up threshold */

/**
 * @brief LSM6DSM accelero extended features driver internal structure definition
 */
typedef struct
{
  DrvStatusTypeDef ( *Enable_Free_Fall_Detection      ) ( DrvContextTypeDef*, SensorIntPin_t );
  DrvStatusTypeDef ( *Disable_Free_Fall_Detection     ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Get_Free_Fall_Detection_Status  ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Set_Free_Fall_Threshold         ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *Enable_Pedometer                ) ( DrvContextTypeDef*, SensorIntPin_t );
  DrvStatusTypeDef ( *Disable_Pedometer               ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Get_Pedometer_Status            ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Get_Step_Count                  ) ( DrvContextTypeDef*, uint16_t* );
  DrvStatusTypeDef ( *Enable_Step_Counter_Reset       ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Disable_Step_Counter_Reset      ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Set_Pedometer_Threshold         ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *Enable_Tilt_Detection           ) ( DrvContextTypeDef*, SensorIntPin_t );
  DrvStatusTypeDef ( *Disable_Tilt_Detection          ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Get_Tilt_Detection_Status       ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Enable_Wake_Up_Detection        ) ( DrvContextTypeDef*, SensorIntPin_t );
  DrvStatusTypeDef ( *Disable_Wake_Up_Detection       ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Get_Wake_Up_Detection_Status    ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Set_Wake_Up_Threshold           ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *Enable_Single_Tap_Detection     ) ( DrvContextTypeDef*, SensorIntPin_t );
  DrvStatusTypeDef ( *Disable_Single_Tap_Detection    ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Get_Single_Tap_Detection_Status ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Enable_Double_Tap_Detection     ) ( DrvContextTypeDef*, SensorIntPin_t );
  DrvStatusTypeDef ( *Disable_Double_Tap_Detection    ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Get_Double_Tap_Detection_Status ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Set_Tap_Threshold               ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *Set_Tap_Shock_Time              ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *Set_Tap_Quiet_Time              ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *Set_Tap_Duration_Time           ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *Enable_6D_Orientation           ) ( DrvContextTypeDef*, SensorIntPin_t );
  DrvStatusTypeDef ( *Disable_6D_Orientation          ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Get_6D_Orientation_Status       ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Get_6D_Orientation_XL           ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Get_6D_Orientation_XH           ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Get_6D_Orientation_YL           ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Get_6D_Orientation_YH           ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Get_6D_Orientation_ZL           ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Get_6D_Orientation_ZH           ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Get_Event_Status             ) ( DrvContextTypeDef*, ACCELERO_Event_Status_t* );
  DrvStatusTypeDef ( *FIFO_Set_ODR_Value              ) ( DrvContextTypeDef*, float );
  DrvStatusTypeDef ( *FIFO_Get_Full_Status            ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Get_Empty_Status           ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Get_Overrun_Status         ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Get_Pattern                ) ( DrvContextTypeDef*, uint16_t* );
  DrvStatusTypeDef ( *FIFO_Get_Data                   ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Get_Num_Of_Samples         ) ( DrvContextTypeDef*, uint16_t* );
  DrvStatusTypeDef ( *FIFO_X_Set_Decimation           ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_X_Get_Axis                 ) ( DrvContextTypeDef*, int32_t* );
  DrvStatusTypeDef ( *FIFO_Set_Mode                   ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_Set_INT1_FIFO_Full         ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_Set_Watermark_Level        ) ( DrvContextTypeDef*, uint16_t );
  DrvStatusTypeDef ( *FIFO_Set_Stop_On_Fth            ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *Set_Interrupt_Latch             ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *Set_SelfTest                    ) ( DrvContextTypeDef*, uint8_t );
} LSM6DSM_X_ExtDrv_t;

/**
 * @brief LSM6DSM gyro extended features driver internal structure definition
 */
typedef struct
{
  DrvStatusTypeDef ( *FIFO_Set_ODR_Value       ) ( DrvContextTypeDef*, float );
  DrvStatusTypeDef ( *FIFO_Get_Full_Status     ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Get_Empty_Status    ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Get_Overrun_Status  ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Get_Pattern         ) ( DrvContextTypeDef*, uint16_t* );
  DrvStatusTypeDef ( *FIFO_Get_Data            ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Get_Num_Of_Samples  ) ( DrvContextTypeDef*, uint16_t* );
  DrvStatusTypeDef ( *FIFO_G_Set_Decimation    ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_G_Get_Axis          ) ( DrvContextTypeDef*, int32_t* );
  DrvStatusTypeDef ( *FIFO_Set_Mode            ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_Set_INT1_FIFO_Full  ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_Set_Watermark_Level ) ( DrvContextTypeDef*, uint16_t );
  DrvStatusTypeDef ( *FIFO_Set_Stop_On_Fth     ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *Set_Interrupt_Latch      ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *Set_SelfTest             ) ( DrvContextTypeDef*, uint8_t );
} LSM6DSM_G_ExtDrv_t;

/**
 * @brief LSM6DSM combo specific data internal structure definition
 */
typedef struct
{
  uint8_t isAccInitialized;
  uint8_t isGyroInitialized;
} LSM6DSM_Combo_Data_t;

/**
 * @brief LSM6DSM accelero specific data internal structure definition
 */
typedef struct
{
  LSM6DSM_Combo_Data_t
  *comboData;       /* Combo data to manage in software SPI 3-Wire initialization of the combo sensors */
  float Previous_ODR;
} LSM6DSM_X_Data_t;

/**
 * @brief LSM6DSM gyro specific data internal structure definition
 */
typedef struct
{
  LSM6DSM_Combo_Data_t
  *comboData;       /* Combo data to manage in software SPI 3-Wire initialization of the combo sensors */
  float Previous_ODR;
} LSM6DSM_G_Data_t;

extern ACCELERO_Drv_t LSM6DSM_X_Drv;
extern GYRO_Drv_t LSM6DSM_G_Drv;
extern LSM6DSM_X_ExtDrv_t LSM6DSM_X_ExtDrv;
extern LSM6DSM_G_ExtDrv_t LSM6DSM_G_ExtDrv;
extern LSM6DSM_Combo_Data_t LSM6DSM_Combo_Data[LSM6DSM_SENSORS_MAX_NUM];

#ifdef __cplusplus
}
#endif

#endif /* __LSM6DSM_ACC_GYRO_DRIVER_HL_H */

