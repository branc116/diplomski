#ifndef __LPS22HB_DRIVER_HL_H
#define __LPS22HB_DRIVER_HL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pressure.h"
#include "temperature.h"

/* Include pressure sensor component drivers. */
#include "LPS22HB_Driver.h"
#define LPS22HB_SENSORS_MAX_NUM  1     /**< LPS22HB max number of instances */

#define LPS22HB_ADDRESS_LOW      0xB8  /**< LPS22HB I2C Address Low */
#define LPS22HB_ADDRESS_HIGH     0xBA  /**< LPS22HB I2C Address High */

/**
 * @brief LPS22HB pressure extended features driver internal structure definition
 */
typedef struct
{
  DrvStatusTypeDef ( *FIFO_Get_Empty_Status    ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Get_Full_Status     ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Get_Ovr_Status      ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Get_Fth_Status      ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Stop_On_Fth         ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_Usage               ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_Get_Num_Of_Samples  ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Get_Data            ) ( DrvContextTypeDef*, float*, float* );
  DrvStatusTypeDef ( *FIFO_Get_Mode            ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Set_Mode            ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_Get_Watermark_Level ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Set_Watermark_Level ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_Watermark_Usage     ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_Set_Interrupt       ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_Reset_Interrupt     ) ( DrvContextTypeDef*, uint8_t );
} LPS22HB_P_ExtDrv_t;

/**
 * @brief LPS22HB temperature extended features driver internal structure definition
 */
typedef struct
{
  DrvStatusTypeDef ( *FIFO_Get_Empty_Status    ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Get_Full_Status     ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Get_Ovr_Status      ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Get_Fth_Status      ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Stop_On_Fth         ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_Usage               ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_Get_Num_Of_Samples  ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Get_Data            ) ( DrvContextTypeDef*, float*, float* );
  DrvStatusTypeDef ( *FIFO_Get_Mode            ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Set_Mode            ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_Get_Watermark_Level ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *FIFO_Set_Watermark_Level ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_Watermark_Usage     ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_Set_Interrupt       ) ( DrvContextTypeDef*, uint8_t );
  DrvStatusTypeDef ( *FIFO_Reset_Interrupt     ) ( DrvContextTypeDef*, uint8_t );
} LPS22HB_T_ExtDrv_t;

/**
 * @brief LPS22HB combo specific data internal structure definition
 */
typedef struct
{
  uint8_t isPressInitialized;
  uint8_t isTempInitialized;
  uint8_t isPressEnabled;
  uint8_t isTempEnabled;
  float Last_ODR;
} LPS22HB_Combo_Data_t;

/**
 * @brief LPS22HB pressure specific data internal structure definition
 */
typedef struct
{
  LPS22HB_Combo_Data_t *comboData;       /* Combo data to manage in software enable/disable of the combo sensors */
} LPS22HB_P_Data_t;

/**
 * @brief LPS22HB temperature specific data internal structure definition
 */
typedef struct
{
  LPS22HB_Combo_Data_t *comboData;       /* Combo data to manage in software enable/disable of the combo sensors */
} LPS22HB_T_Data_t;

extern PRESSURE_Drv_t LPS22HB_P_Drv;
extern TEMPERATURE_Drv_t LPS22HB_T_Drv;
extern LPS22HB_Combo_Data_t LPS22HB_Combo_Data[LPS22HB_SENSORS_MAX_NUM];
extern LPS22HB_P_ExtDrv_t LPS22HB_P_ExtDrv;
extern LPS22HB_T_ExtDrv_t LPS22HB_T_ExtDrv;

#ifdef __cplusplus
}
#endif

#endif /* __LPS22HB_DRIVER_HL_H */

