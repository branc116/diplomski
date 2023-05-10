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

/**
 * Increase this parameter to overcome possible issues due to BLE devices crowded environment
 * or high number of incoming notifications from peripheral devices
 */
#define HCI_READ_PACKET_NUM_MAX 	   (150)

#define MIN(a,b)      ((a) < (b))? (a) : (b)
#define MAX(a,b)      ((a) > (b))? (a) : (b)

static uint32_t read_packet_counter = 1;
static struct hciDataPacketWithId { tHciDataPacket el; int32_t id;  } hciReadPacketBuffer[HCI_READ_PACKET_NUM_MAX];
//static tHciContext    hciContext;

static BOOL any_unread(void) {
  for (int i = 0; i < HCI_READ_PACKET_NUM_MAX; ++i) {
    if (hciReadPacketBuffer[i].id > 0) return TRUE;
  }
  return FALSE;
}

static BOOL any_available(void) {
  for (int i = 0; i < HCI_READ_PACKET_NUM_MAX; ++i) {
    if (hciReadPacketBuffer[i].id <= 0) return TRUE;
  }
  return FALSE;
}

static tHciDataPacket* take_first_available(void) {
  int32_t biggest = 0xFFFFFFFF;
  struct hciDataPacketWithId * ret = NULL;

  for (int i = 0; i < HCI_READ_PACKET_NUM_MAX; ++i) {
    if (hciReadPacketBuffer[i].id <= 0 && hciReadPacketBuffer[i].id > biggest) {
      biggest = hciReadPacketBuffer[i].id;
      ret = &hciReadPacketBuffer[i];
    }
  }
  if (ret) {
    ret->id = read_packet_counter++;
    return &ret->el;
  }
  return NULL;
}

static void invalidat(tHciDataPacket* data) {
 struct hciDataPacketWithId * r = (struct hciDataPacketWithId *)(data);
 r->el.data_len = 0;
 r->id = 0;
}

static tHciDataPacket* pop_last(void) {
  struct hciDataPacketWithId * ret = NULL;
  int32_t min_id = 0x7FFFFFFF;

  for (int i = 0; i < HCI_READ_PACKET_NUM_MAX; ++i) {
    if (hciReadPacketBuffer[i].id > 0 && hciReadPacketBuffer[i].id < min_id) {
      ret = &hciReadPacketBuffer[i];
      min_id = hciReadPacketBuffer[i].id;
    }
  }
  if (ret) {
    ret->id = -ret->id;
    return &ret->el;
  } else {
    return NULL;
  }
}

/**
  * @brief  Verify the packet type.
  *
  * @param  hciReadPacket The HCI data packet
  * @retval 0: valid packet, 1: incorrect packet, 2: wrong length (packet truncated or too long)
  */
static int verify_packet(const tHciDataPacket * hciReadPacket)
{
  const uint8_t *hci_pckt = hciReadPacket->dataBuff;

  if (hci_pckt[HCI_PCK_TYPE_OFFSET] != HCI_EVENT_PKT)
    return 1; /* Incorrect type */

  if (hci_pckt[EVENT_PARAMETER_TOT_LEN_OFFSET] != hciReadPacket->data_len - (1+HCI_EVENT_HDR_SIZE))
    return 2; /* Wrong length (packet truncated or too long) */

  return 0;
}

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
  hci_command_hdr hc;

  hc.opcode = htobs(cmd_opcode_pack(ogf, ocf));
  hc.plen = plen;

  payload[0] = HCI_COMMAND_PKT;
  BLUENRG_memcpy(payload + 1, &hc, sizeof(hc));
  BLUENRG_memcpy(payload + HCI_HDR_SIZE + HCI_COMMAND_HDR_SIZE, param, plen);

  BlueNRG_Write(payload, HCI_HDR_SIZE + HCI_COMMAND_HDR_SIZE + plen);
}

/********************** HCI Transport layer functions *****************************/

void hci_init(void)
{
  /* Initialize list heads of ready and free hci data packet queues */
  memset(hciReadPacketBuffer, 0, sizeof(hciReadPacketBuffer));
  read_packet_counter = 1;

  /* Initialize low level driver */
  HCI_TL_SPI_Init();
  BlueNRG_RST();
}

int hci_send_req(struct hci_request* r, BOOL async)
{
  uint8_t *ptr;
  uint16_t opcode = htobs(cmd_opcode_pack(r->ogf, r->ocf));
  hci_event_pckt *event_pckt;
  hci_uart_pckt *hci_hdr;

  tHciDataPacket * hciReadPacket = NULL;

  //free_event_list();

  send_cmd(r->ogf, r->ocf, r->clen, r->cparam);  if (async)
  {
    return 0;
  }

  while (1)
  {
    evt_cmd_complete  *cc;
    evt_cmd_status    *cs;
    evt_le_meta_event *me;
    uint32_t len;
    hci_user_evt_proc();

    uint32_t tickstart = HAL_GetTick();

    while (1)
    {
      hci_notify_asynch_evt();
      if ((HAL_GetTick() - tickstart) > HCI_DEFAULT_TIMEOUT_MS)
      {
        return -1;
      }

      if (any_unread()) break;
    }

    while(any_unread()) {
      /* Extract packet from HCI event queue. */
      hciReadPacket = pop_last();

      hci_hdr = (void *)hciReadPacket->dataBuff;

      if (hci_hdr->type == HCI_EVENT_PKT)
      {
        event_pckt = (void *)(hci_hdr->data);

        ptr = hciReadPacket->dataBuff + (1 + HCI_EVENT_HDR_SIZE);
        len = hciReadPacket->data_len - (1 + HCI_EVENT_HDR_SIZE);

        switch (event_pckt->evt)
        {
        case EVT_CMD_STATUS:
          cs = (void *) ptr;

          if (cs->opcode != opcode)
            return -5;

          if (r->event != EVT_CMD_STATUS) {
            if (cs->status) {
              return -6;
            }
            break;
          }

          r->rlen = MIN(len, r->rlen);
          BLUENRG_memcpy(r->rparam, ptr, r->rlen);
          goto done;

        case EVT_CMD_COMPLETE:
          cc = (void *) ptr;

          if (cc->opcode != opcode)
            return -2;

          ptr += EVT_CMD_COMPLETE_SIZE;
          len -= EVT_CMD_COMPLETE_SIZE;

          r->rlen = MIN(len, r->rlen);
          BLUENRG_memcpy(r->rparam, ptr, r->rlen);
          goto done;

        case EVT_LE_META_EVENT:
          me = (void *) ptr;

          if (me->subevent != r->event)
            return -3;

          len -= 1;
          r->rlen = MIN(len, r->rlen);
          BLUENRG_memcpy(r->rparam, me->data, r->rlen);
          goto done;

        case EVT_HARDWARE_ERROR:
          return -4;

        default:
          user_notify(hci_hdr);
          break;
        }
      }
    }
  }


done:
  user_notify(hci_hdr);

  return 0;
}

void process_idle(void) {
  hci_notify_asynch_evt();
  while (any_unread())
  {
    tHciDataPacket * hciReadPacket = pop_last();
    user_notify((hci_uart_pckt *)hciReadPacket->dataBuff);
  }
}

void hci_user_evt_proc(void)
{
  while (any_unread())
  {
    tHciDataPacket * hciReadPacket = pop_last();
    user_notify((hci_uart_pckt *)hciReadPacket->dataBuff);
  }
}

int32_t hci_notify_asynch_evt(void)
{
  uint8_t data_len;

  int32_t ret = 0;
  again: ;
  if (any_available())
  {
    /* Queuing a packet to read */
    tHciDataPacket * hciReadPacket = take_first_available();

    data_len = BlueNRG_SPI_Read_All(hciReadPacket->dataBuff, HCI_READ_PACKET_SIZE);
    if (data_len > 0)
    {
      hciReadPacket->data_len = data_len;
      if (verify_packet(hciReadPacket) != 0)
        invalidat(hciReadPacket);
      goto again;
    }
    else
    {
      /* Insert the packet back into the pool*/
      invalidat(hciReadPacket);
    }
  }
  return ret;
}
