#include "sensor_commands.h"
#include "SensorTile_accelero.h"
#include "SensorTile_gyro.h"
#include "SensorTile_magneto.h"
#include "SensorTile_pressure.h"
#include "SensorTile_temperature.h"
#include "SensorTile_humidity.h"
#include "DemoSerial.h"
#include "vcom.h"

extern void *ACCELERO_handle;
extern void *GYRO_handle;
extern void *MAGNETO_handle;
extern void *HUMIDITY_handle;
extern void *TEMPERATURE_handle;
extern void *PRESSURE_handle;

uint8_t lsm6dsl[] = {"LSM6DSM"};
uint8_t lsm303agr[] = {"LSM303AGR"};
uint8_t hts221[] = {"HTS221"};
uint8_t lps22hb[] = {"LPS22HB"};

uint32_t lsm6dsl_acc_fs[] = {4, 2, 4, 8, 16};      //g
uint32_t lsm6dsl_gyr_fs[] = {4, 245, 500, 1000, 2000};   //dps
float lsm6dsl_acc_odr[] = {10, 13, 26, 52, 104, 208, 416, 833, 1660, 3330, 6660};
float lsm6dsl_gyr_odr[] = {10, 13, 26, 52, 104, 208, 416, 833, 1660, 3330, 6660};

uint32_t lsm303agr_acc_fs[] = {4, 2, 4, 8, 16};      //g
uint32_t lsm303agr_mag_fs[] = {1, 50};     //Ga
float lsm303agr_acc_odr[] = {7, 1, 10, 25, 50, 100, 200, 400};
float lsm303agr_mag_odr[] = {3, 10, 20, 50};

uint32_t hts221_tem_fs[] = {0}; //C
uint32_t hts221_hum_fs[] = {0}; //%
float hts221_tem_odr[] = {3, 1, 7, 12.5};
float hts221_hum_odr[] = {3, 1, 7, 12.5};

uint32_t lps22hb_pre_fs[] = {0}; //Pa
uint32_t lps22hb_tem_fs[] = {0}; //C
float lps22hb_pre_odr[] = {5, 1, 10, 25, 50, 75};
float lps22hb_tem_odr[] = {5, 1, 10, 25, 50, 75};

/**
  * @brief  Handle Sensors command
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
int Handle_Sensor_command(TMsg *Msg)
{
  /* Commands */
  switch (Msg->Data[3])
  {
    case SC_GET_SENSOR_NAME:
      return SC_Get_Sensor_Name(Msg);

    case SC_READ_REGISTER:
      return SC_Read_Register(Msg);

    case SC_WRITE_REGISTER:
      return SC_Write_Register(Msg);

    case SC_GET_FULL_SCALE_LIST:
      return SC_Get_Full_Scale_List(Msg);

    case SC_SET_FULL_SCALE:
      return SC_Set_Full_Scale(Msg);

    case SC_GET_ODR_LIST:
      return SC_Get_ODR_List(Msg);

    case SC_SET_ODR:
      return SC_Set_ODR(Msg);

    default:
      return 0;
  }
}

int SC_Get_Sensor_Name(TMsg *Msg)
{
  uint8_t instance;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      BSP_ACCELERO_Get_Instance(ACCELERO_handle, &instance);
      switch (instance)
      {
        case LSM6DSM_X_0:
          Send_Sensor_Name(Msg, lsm6dsl);
          return 1;

        case LSM303AGR_X_0:
          Send_Sensor_Name(Msg, lsm303agr);
          return 1;

        default:
          return 0;
      }

    case SC_GYROSCOPE:
      BSP_GYRO_Get_Instance(GYRO_handle, &instance);
      switch (instance)
      {
        case LSM6DSM_G_0:
          Send_Sensor_Name(Msg, lsm6dsl);
          return 1;

        default:
          return 0;
      }

    case SC_MAGNETOMETER:
      BSP_MAGNETO_Get_Instance(MAGNETO_handle, &instance);
      switch (instance)
      {
        case LSM303AGR_M_0:
          Send_Sensor_Name(Msg, lsm303agr);
          return 1;

        default:
          return 0;
      }

    case SC_TEMPERATURE:
      BSP_TEMPERATURE_Get_Instance(TEMPERATURE_handle, &instance);
      switch (instance)
      {
        case HTS221_T_0:
          Send_Sensor_Name(Msg, hts221);
          return 1;

        case LPS22HB_T_0:
          Send_Sensor_Name(Msg, lps22hb);
          return 1;

        default:
          return 0;
      }

    case SC_HUMIDITY:
      BSP_HUMIDITY_Get_Instance(HUMIDITY_handle, &instance);
      switch (instance)
      {
        case HTS221_H_0:
          Send_Sensor_Name(Msg, hts221);
          return 1;

        default:
          return 0;
      }

    case SC_PRESSURE:
      BSP_PRESSURE_Get_Instance(PRESSURE_handle, &instance);
      switch (instance)
      {
        case LPS22HB_P_0:
          Send_Sensor_Name(Msg, lps22hb);
          return 1;

        default:
          return 0;
      }

    default:
      return 0;
  }
}

int SC_Read_Register(TMsg *Msg)
{
  uint8_t reg_value;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      BSP_ACCELERO_Read_Reg(ACCELERO_handle, Msg->Data[5], &reg_value);
      break;

    case SC_GYROSCOPE:
      BSP_GYRO_Read_Reg(GYRO_handle, Msg->Data[5], &reg_value);
      break;

    case SC_MAGNETOMETER:
      BSP_MAGNETO_Read_Reg(MAGNETO_handle, Msg->Data[5], &reg_value);
      break;

    case SC_TEMPERATURE:
      BSP_TEMPERATURE_Read_Reg(TEMPERATURE_handle, Msg->Data[5], &reg_value);
      break;

    case SC_HUMIDITY:
      BSP_HUMIDITY_Read_Reg(HUMIDITY_handle, Msg->Data[5], &reg_value);
      break;

    case SC_PRESSURE:
      BSP_PRESSURE_Read_Reg(PRESSURE_handle, Msg->Data[5], &reg_value);
      break;

    default:
      return 0;
  }

  BUILD_REPLY_HEADER(Msg);
  Msg->Data[6] = reg_value;
  Msg->Len = 7;
  VCOM_send_MSG(Msg);
  return 1;
}

int SC_Write_Register(TMsg *Msg)
{
  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      BSP_ACCELERO_Write_Reg(ACCELERO_handle, Msg->Data[5], Msg->Data[6]);
      break;

    case SC_GYROSCOPE:
      BSP_GYRO_Write_Reg(GYRO_handle, Msg->Data[5], Msg->Data[6]);
      break;

    case SC_MAGNETOMETER:
      BSP_MAGNETO_Write_Reg(MAGNETO_handle, Msg->Data[5], Msg->Data[6]);
      break;

    case SC_TEMPERATURE:
      BSP_TEMPERATURE_Write_Reg(TEMPERATURE_handle, Msg->Data[5], Msg->Data[6]);
      break;

    case SC_HUMIDITY:
      BSP_HUMIDITY_Write_Reg(HUMIDITY_handle, Msg->Data[5], Msg->Data[6]);
      break;

    case SC_PRESSURE:
      BSP_PRESSURE_Write_Reg(PRESSURE_handle, Msg->Data[5], Msg->Data[6]);
      break;

    default:
      return 0;
  }

  BUILD_REPLY_HEADER(Msg);
  Msg->Len = 7;
  VCOM_send_MSG(Msg);
  return 1;
}

int SC_Get_Full_Scale_List(TMsg *Msg)
{
  uint8_t instance;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      BSP_ACCELERO_Get_Instance(ACCELERO_handle, &instance);
      switch (instance)
      {
        case LSM6DSM_X_0:
          Send_Sensor_FS_List(Msg, lsm6dsl_acc_fs);
          return 1;

        case LSM303AGR_X_0:
          Send_Sensor_FS_List(Msg, lsm303agr_acc_fs);
          return 1;

        default:
          return 0;
      }

    case SC_GYROSCOPE:
      BSP_GYRO_Get_Instance(GYRO_handle, &instance);
      switch (instance)
      {
        case LSM6DSM_G_0:
          Send_Sensor_FS_List(Msg, lsm6dsl_gyr_fs);
          return 1;

        default:
          return 0;
      }

    case SC_MAGNETOMETER:
      BSP_MAGNETO_Get_Instance(MAGNETO_handle, &instance);
      switch (instance)
      {
        case LSM303AGR_M_0:
          Send_Sensor_FS_List(Msg, lsm303agr_mag_fs);
          return 1;

        default:
          return 0;
      }

    case SC_TEMPERATURE:
      BSP_TEMPERATURE_Get_Instance(TEMPERATURE_handle, &instance);
      switch (instance)
      {
        case HTS221_T_0:
          Send_Sensor_FS_List(Msg, hts221_tem_fs);
          return 1;

        case LPS22HB_T_0:
          Send_Sensor_FS_List(Msg, lps22hb_tem_fs);
          return 1;

        default:
          return 0;
      }

    case SC_HUMIDITY:
      BSP_HUMIDITY_Get_Instance(HUMIDITY_handle, &instance);
      switch (instance)
      {
        case HTS221_H_0:
          Send_Sensor_FS_List(Msg, hts221_hum_fs);
          return 1;

        default:
          return 0;
      }

    case SC_PRESSURE:
      BSP_PRESSURE_Get_Instance(PRESSURE_handle, &instance);
      switch (instance)
      {
        case LPS22HB_P_0:
          Send_Sensor_FS_List(Msg, lps22hb_pre_fs);
          return 1;
        default:
          return 0;
      }

    default:
      return 0;
  }
}

int SC_Set_Full_Scale(TMsg *Msg)
{
  uint32_t full_scale;

  full_scale = Deserialize(&Msg->Data[5], 4);

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      BSP_ACCELERO_Set_FS_Value(ACCELERO_handle, full_scale);
      break;

    case SC_GYROSCOPE:
      BSP_GYRO_Set_FS_Value(GYRO_handle, full_scale);
      break;

    case SC_MAGNETOMETER:
      //BSP_MAGNETO_Set_FS_Value(MAGNETO_handle, full_scale);
      break;

    case SC_TEMPERATURE:
      //BSP_TEMPERATURE_Set_FS_Value(TEMPERATURE_handle, full_scale);
      break;

    case SC_HUMIDITY:
      //BSP_HUMIDITY_Set_FS_Value(HUMIDITY_handle, full_scale);
      break;

    case SC_PRESSURE:
      //BSP_PRESSURE_Set_FS_Value(PRESSURE_handle, full_scale);

    default:
      return 0;
  }

  BUILD_REPLY_HEADER(Msg);
  Msg->Len = 9;
  VCOM_send_MSG(Msg);
  return 1;
}

int SC_Get_ODR_List(TMsg *Msg)
{
  uint8_t instance;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      BSP_ACCELERO_Get_Instance(ACCELERO_handle, &instance);
      switch (instance)
      {
        case LSM6DSM_X_0:
          Send_Sensor_ODR_List(Msg, lsm6dsl_acc_odr);
          return 1;

        case LSM303AGR_X_0:
          Send_Sensor_ODR_List(Msg, lsm303agr_acc_odr);
          return 1;

        default:
          return 0;
      }

    case SC_GYROSCOPE:
      BSP_GYRO_Get_Instance(GYRO_handle, &instance);
      switch (instance)
      {
        case LSM6DSM_G_0:
          Send_Sensor_ODR_List(Msg, lsm6dsl_gyr_odr);
          return 1;

        default:
          return 0;
      }

    case SC_MAGNETOMETER:
      BSP_MAGNETO_Get_Instance(MAGNETO_handle, &instance);
      switch (instance)
      {
        case LSM303AGR_M_0:
          Send_Sensor_ODR_List(Msg, lsm303agr_mag_odr);
          return 1;

        default:
          return 0;
      }

    case SC_TEMPERATURE:
      BSP_TEMPERATURE_Get_Instance(TEMPERATURE_handle, &instance);
      switch (instance)
      {
        case HTS221_T_0:
          Send_Sensor_ODR_List(Msg, hts221_tem_odr);
          return 1;

        case LPS22HB_T_0:
          Send_Sensor_ODR_List(Msg, lps22hb_tem_odr);
          return 1;

        default:
          return 0;
      }

    case SC_HUMIDITY:
      BSP_HUMIDITY_Get_Instance(HUMIDITY_handle, &instance);
      switch (instance)
      {
        case HTS221_H_0:
          Send_Sensor_ODR_List(Msg, hts221_hum_odr);
          return 1;

        default:
          return 0;
      }

    case SC_PRESSURE:
      BSP_PRESSURE_Get_Instance(PRESSURE_handle, &instance);
      switch (instance)
      {
        case LPS22HB_P_0:
          Send_Sensor_ODR_List(Msg, lps22hb_pre_odr);
          return 1;

        default:
          return 0;
      }

    default:
      return 0;
  }
}

int SC_Set_ODR(TMsg *Msg)
{
  float odr;

  ArrayToFloat(&Msg->Data[5], &odr);

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      BSP_ACCELERO_Set_ODR_Value(ACCELERO_handle, odr);
      break;

    case SC_GYROSCOPE:
      BSP_GYRO_Set_ODR_Value(GYRO_handle, odr);
      break;

    case SC_MAGNETOMETER:
      BSP_MAGNETO_Set_ODR_Value(MAGNETO_handle, odr);
      break;

    case SC_TEMPERATURE:
      BSP_TEMPERATURE_Set_ODR_Value(TEMPERATURE_handle, odr);
      break;

    case SC_HUMIDITY:
      BSP_HUMIDITY_Set_ODR_Value(HUMIDITY_handle, odr);
      break;

    case SC_PRESSURE:
      BSP_PRESSURE_Set_ODR_Value(PRESSURE_handle, odr);
      break;

    default:
      return 0;
  }

  BUILD_REPLY_HEADER(Msg);
  Msg->Len = 9;
  VCOM_send_MSG(Msg);
  return 1;
}

void Send_Sensor_Name(TMsg *Msg, uint8_t* sensor_name)
{
  int i = 0;
  BUILD_REPLY_HEADER(Msg);

  while (i < strlen((char const*)sensor_name))
  {
    Msg->Data[5 + i] = sensor_name[i];
    i++;
  }

  Msg->Len = 5 + i;
  VCOM_send_MSG(Msg);
}

void Send_Sensor_FS_List(TMsg *Msg, uint32_t* fs_list)
{
  int i = 0;
  BUILD_REPLY_HEADER(Msg);

  Serialize(&Msg->Data[5], fs_list[0], 4);

  for (i = 0; i < fs_list[0]; i++)
  {
    Serialize(&Msg->Data[9 + i * 4], fs_list[i + 1], 4);
  }

  Msg->Len = 9 + i * 4;
  VCOM_send_MSG(Msg);
}

void Send_Sensor_ODR_List(TMsg *Msg, float* odr_list)
{
  int i = 0;
  BUILD_REPLY_HEADER(Msg);

  Serialize(&Msg->Data[5], (int) odr_list[0], 4);

  for (i = 0; i < odr_list[0]; i++)
  {
    FloatToArray(&Msg->Data[9 + i * 4], odr_list[i + 1]);
  }

  Msg->Len = 9 + i * 4;
  VCOM_send_MSG(Msg);
}

void FloatToArray(uint8_t *Dest, float data)
{
  memcpy(Dest, (void *) &data, 4);
}

void ArrayToFloat(uint8_t *Source, float *data)
{
  memcpy((void *) data, Source, 4);
}

