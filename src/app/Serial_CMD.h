#ifndef __SERIAL_CMD_H
#define __SERIAL_CMD_H

#define DATALOG_MODE         100
#define DATALOG_FUSION_MODE    1
#define DATALOG_AR_MODE        2
#define DATALOG_CP_MODE        3
#define DATALOG_GR_MODE        4
#define DATALOG_EXT_MODE     101

#define CMD_Ping                                                                0x01
#define CMD_Read_PresString                                                     0x02
#define CMD_NACK                                                                0x03
#define CMD_CheckModeSupport                                                    0x04
#define CMD_UploadAR                                                            0x05
#define CMD_UploadCP                                                            0x06
#define CMD_Start_Data_Streaming                                                0x08
#define CMD_Stop_Data_Streaming                                                 0x09
#define CMD_StartDemo                                                           0x0A
#define CMD_Sleep_Sec                                                           0x0B
#define CMD_Set_DateTime                                                        0x0C
#define CMD_Get_DateTime                                                        0x0D
#define CMD_Enter_DFU_Mode                                                      0x0E
#define CMD_Reset                                                               0x0F

#define CMD_Sensor                                                              0x50

#define CMD_Reply_Add                                                           0x80

#endif /* __SERIAL_CMD_H */

