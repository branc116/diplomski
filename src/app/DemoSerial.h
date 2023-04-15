#ifndef __DEMO_SERIAL_H__
#define __DEMO_SERIAL_H__

#include "cube_hal.h"
#include "serial_protocol.h"
#include "Serial_CMD.h"

#define SENDER_UART                     0x01
#define SENDER_USB                      0x02
#define SENDER_SPI                      0x03

#define DEV_ADDR                        50
#define I2C_DATA_MAX_LENGTH_BYTES       16
#define STREAMING_MSG_LENGTH            56

int HandleMSG(TMsg *Msg);
void BUILD_REPLY_HEADER(TMsg *Msg);
void BUILD_NACK_HEADER(TMsg *Msg);
void INIT_STREAMING_MSG(TMsg *Msg);
void INIT_STREAMING_HEADER(TMsg *Msg);

#endif /* __DEMO_SERIAL_H__ */
