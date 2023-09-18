// @brief   Contains the basic functions for managing the framework required for handling the HCI interface

#include "SensorTile_BlueNRG.h"
#include "bluenrg_types.h"
#include "hci_const.h"
#include "hci.h"
#include "hci_tl.h"
#include "sample_service.h"
#include <stdint.h>
#include <string.h>

#define HCI_LOG_ON                      0
#define HCI_PCK_TYPE_OFFSET             0
#define EVENT_PARAMETER_TOT_LEN_OFFSET  2

#define MIN(a,b)      ((a) < (b))? (a) : (b)
#define MAX(a,b)      ((a) > (b))? (a) : (b)

/**
  * @brief  Send an HCI command.
  *
  * @param  ogf The Opcode Group Field
  * @param  ocf The Opcode Command Field
  * @param  plen The HCI command length
  * @param  param The HCI command parameters
  */
static void send_cmd(uint16_t ogf, uint16_t ocf, uint8_t plen, void *param)
{
  uint8_t payload[HCI_MAX_PAYLOAD_SIZE];
  memset(payload, 0, sizeof(payload));
  hci_command_hdr hc;

  hc.opcode = htobs(cmd_opcode_pack(ogf, ocf));
  hc.plen = plen;

  payload[0] = HCI_COMMAND_PKT;
  BLUENRG_memcpy(payload + 1, &hc, sizeof(hc));
  BLUENRG_memcpy(payload + HCI_HDR_SIZE + HCI_COMMAND_HDR_SIZE, param, plen);


  Disable_SPI_IRQ();
  while(BlueNRG_Write(payload, HCI_HDR_SIZE + HCI_COMMAND_HDR_SIZE + plen)) ;
  Enable_SPI_IRQ();
}

/********************** HCI Transport layer functions *****************************/

void hci_init(void)
{
  /* Initialize low level driver */
  HCI_TL_SPI_Init();
  Enable_SPI_IRQ();
  BlueNRG_RST();
}

int hci_send_req(struct hci_request* r)
{
  send_cmd(r->ogf, r->ocf, r->clen, r->cparam);
  return 0;
}

void hci_notify_asynch_evt(void)
{
  static uint8_t buff[128];
  int n = 3;
  Disable_SPI_IRQ();
  again: ;
  blue_state.number_of_attempts_to_read_data++;
  uint32_t data_len = BlueNRG_SPI_Read_All(buff, sizeof(buff));
  if (data_len > 0)
  {
    user_notify((hci_uart_pckt *)buff);
  }
  else {
    n--;
    if (n > 0) goto again;
  }
  Enable_SPI_IRQ();
}


void EXTI9_5_IRQHandler(void) {
  blue_state.blue_number_of_times_in_interupt++;
  hci_notify_asynch_evt();
  Clear_SPI_EXTI_Flag();
}

