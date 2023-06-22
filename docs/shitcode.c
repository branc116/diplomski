DrvStatusTypeDef BSP_GYRO_Get_WhoAmI
(void *handle, uint8_t *who_am_i)
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  if ( who_am_i == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Get_WhoAmI == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Get_WhoAmI( ctx, who_am_i ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  return COMPONENT_OK;
}

// file: commonsensors/gyroscope.h
typedef struct
{
  DrvStatusTypeDef ( *Init            ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *DeInit          ) DrvContextTypeDef* );
  DrvStatusTypeDef ( *Sensor_Enable   ) DrvContextTypeDef* );
  DrvStatusTypeDef ( *Sensor_Disable  ) DrvContextTypeDef* );
  DrvStatusTypeDef ( *Get_WhoAmI      ) DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Check_WhoAmI    ) DrvContextTypeDef* );
  DrvStatusTypeDef ( *Get_Axes        ) DrvContextTypeDef*, SensorAxes_t* );
  DrvStatusTypeDef ( *Get_AxesRaw     ) DrvContextTypeDef*, SensorAxesRaw_t* );
  DrvStatusTypeDef ( *Get_Sensitivity ) DrvContextTypeDef*, float* );
  DrvStatusTypeDef ( *Get_ODR         ) DrvContextTypeDef*, float* );
  DrvStatusTypeDef ( *Set_ODR         ) DrvContextTypeDef*, SensorOdr_t );
  DrvStatusTypeDef ( *Set_ODR_Value   ) DrvContextTypeDef*, float );
  DrvStatusTypeDef ( *Get_FS          ) DrvContextTypeDef*, float* );
  DrvStatusTypeDef ( *Set_FS          ) DrvContextTypeDef*, SensorFs_t );
  DrvStatusTypeDef ( *Set_FS_Value    ) DrvContextTypeDef*, float );
  DrvStatusTypeDef ( *Get_Axes_Status ) DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Set_Axes_Status ) DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Read_Reg        ) DrvContextTypeDef*, uint8_t, uint8_t* );
  DrvStatusTypeDef ( *Write_Reg       ) DrvContextTypeDef*, uint8_t, uint8_t );
  DrvStatusTypeDef ( *Get_DRDY_Status ) DrvContextTypeDef*, uint8_t* );
} GYRO_Drv_t;

// This is assigned to Get_WhoAmI field of GYRO_Drv_t struct.
// file: lsm6dsm/LSM6DSM_ACC_GYRO_driver.c
status_t LSM6DSM_ACC_GYRO_R_WHO_AM_I
(void *handle, u8_t *value)
{
  if( !LSM6DSM_ACC_GYRO_ReadReg(handle, 
       LSM6DSM_ACC_GYRO_WHO_AM_I_REG, (u8_t *)value, 1))
    return MEMS_ERROR;

  *value &= LSM6DSM_ACC_GYRO_WHO_AM_I_BIT_MASK; //coerce
  *value = *value >> LSM6DSM_ACC_GYRO_WHO_AM_I_BIT_POSITION; //mask

  return MEMS_SUCCESS;
}

//file: lsm6dsm/LSM6DSM_ACC_GYRO_driver.c
status_t LSM6DSM_ACC_GYRO_ReadReg
(void *handle, u8_t Reg, u8_t* Data, u16_t len)
{
  if (Sensor_IO_Read(handle, Reg, Data, len))
  {
    return MEMS_ERROR;
  }
  else
  {
    return MEMS_SUCCESS;
  }
}

// file: sensortile/SensorTile.c
// I2C path in this function is never used
// because SnsorTile Stm32L4 cpu is using
// spi to communicate with LSM6DSM
uint8_t Sensor_IO_Read
(void *handle, uint8_t ReadAddr,
uint8_t *pBuffer, uint16_t nBytesToRead)
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;

  if(ctx->ifType == 0)
  {
    if ( nBytesToRead > 1 )
    {
      if (ctx->who_am_i == HTS221_WHO_AM_I_VAL)
      {
        ReadAddr |= 0x80;  /* Enable I2C multi-bytes Write */
      }
    }
    return Sensor_IO_I2C_Read
      ( handle, ReadAddr, pBuffer, nBytesToRead );
  }
  if(ctx->ifType == 1 )
  {
    if ( nBytesToRead > 1 ) {
      switch(ctx->who_am_i)
        {
          case LSM303AGR_ACC_WHO_AM_I: ReadAddr |= 0x40; break;
          case LSM303AGR_MAG_WHO_AM_I: break;
          default:;
        }
    }
   return
     Sensor_IO_SPI_Read( handle, ReadAddr, pBuffer, nBytesToRead );
  }

  return COMPONENT_ERROR;
}
