#ifndef __SENSOR_COMMANDS__
#define __SENSOR_COMMANDS__

#include <stdint.h>
#include "serial_protocol.h"

#define SC_ACCELEROMETER                                                0x01
#define SC_GYROSCOPE                                                    0x02
#define SC_MAGNETOMETER                                                 0x03
#define SC_TEMPERATURE                                                  0x04
#define SC_HUMIDITY                                                     0x05
#define SC_PRESSURE                                                     0x06
#define SC_UV                                                           0x07

#define SC_GET_SENSOR_NAME                                              0x01
#define SC_READ_REGISTER                                                0x02
#define SC_WRITE_REGISTER                                               0x03
#define SC_GET_FULL_SCALE_LIST                                          0x04
#define SC_SET_FULL_SCALE                                               0x05
#define SC_GET_ODR_LIST                                                 0x06
#define SC_SET_ODR                                                      0x07

int SC_Get_Sensor_Name(TMsg *Msg);
int SC_Read_Register(TMsg *Msg);
int SC_Write_Register(TMsg *Msg);
int SC_Get_Full_Scale_List(TMsg *Msg);
int SC_Set_Full_Scale(TMsg *Msg);
int SC_Get_ODR_List(TMsg *Msg);
int SC_Set_ODR(TMsg *Msg);

void Send_Sensor_Name(TMsg *Msg, uint8_t* sensor_name);
void Send_Sensor_FS_List(TMsg *Msg, uint32_t* fs_list);
void Send_Sensor_ODR_List(TMsg *Msg, float* odr_list);

void FloatToArray(uint8_t *Dest, float data);
void ArrayToFloat(uint8_t *Source, float *data);

int Handle_Sensor_command(TMsg *Msg);

#endif /* __SENSOR_COMMANDS__ */

