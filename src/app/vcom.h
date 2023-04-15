#ifndef __VCOM__H
#define __VCOM__H

#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"
#include "serial_protocol.h"

/**
 * @brief  Serial message status values enumeration
 */
typedef enum
{
  OK,              /* Message read OK. */
  NO_DATA,         /* No data in message buffer. */
  UNEXP_END_ERR,   /* Unexpected end of message found. */
  WRONG_SEQ_ERR,   /* Wrong sequence found. */
  WRONG_CHECK_ERR, /* Wrong message checksum. */
  MSG_LEN_ERR,     /* Message length exceeded */
  UNKNOWN_ERROR    /* Unknown error. */
} vcom_msg_status_t;

int VCOM_init(void);
vcom_msg_status_t VCOM_receive_MSG(TMsg *Msg, uint32_t len_max);
vcom_msg_status_t VCOM_send_MSG(TMsg *Msg);
uint32_t VCOM_read(char *buffer, uint32_t len_max);
uint32_t VCOM_write(char *buffer, uint32_t len_max);

#endif /* __VCOM__H */

