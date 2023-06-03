#include "sample_service.h"
#include "SensorTile_BlueNRG.h"
#include "bluenrg_aci_const.h"
#include "bluenrg_def.h"
#include "bluenrg_gap.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_hal_aci.h"

#include "bluenrg_types.h"
#include "hci_const.h"
#include "hci_tl.h"
#include "help.h"
#include "stm32l4xx.h"
#include "SensorTile.h"
#include "stm32l4xx_hal_conf.h"
#include <stm32l476xx.h>
#include <stm32l4xx_hal.h>
#include <sys/_stdint.h>

int sendText(blue_state_t* state, const char* data_buffer, uint8_t Nb_bytes)
{
  state->number_of_send_attempts++;
  return aci_gatt_update_char_value(state->sampleServHandle, state->simpCharHandle, 0, Nb_bytes, data_buffer);
}

evt_disconn_complete last_disconn_complete_msg;
const char name[] = "BlueNRG";

blue_state_t blue_state;

void InfLoop(void) {
  volatile int i = 0;
  while(1) ++i;
}

void add_event(blue_state_t* state, uint16_t conn_handle, uint16_t attr_handle, uint8_t data_length, uint8_t const * data) {
  state->number_of_events++;
  for (int i = 0; i < BLUE_EVENTS_CAPACITY; ++i) {
    if (state->events[i].number_of_changes == 0) {
      state->events[i].conn_handle = conn_handle;
      state->events[i].attr_handle = attr_handle;
      ++state->events[i].number_of_changes;
      state->events[i].data_length = data_length;
      memcpy(state->events[i].att_data, data, data_length > 128 ? 128 : data_length);
      return;
    } else if (0 && state->events[i].conn_handle == conn_handle && state->events[i].attr_handle == attr_handle) {
      ++state->events[i].number_of_changes;
      state->events[i].data_length = data_length;
      memcpy(state->events[i].att_data, data, data_length > 128 ? 128 : data_length);
      return;
    }
  }
  InfLoop();
}

static void user_notify_begin(blue_state_t* state, uint8_t evt, evt_cmd_complete *cc, evt_blue_aci *eba) {
  if (evt == EVT_VENDOR) {
    switch (eba->ecode) {
      case 1: {
                uint8_t SERVER_BDADDR[] = {0xaa, 0x00, 0x00, 0xE1, 0x80, 0x02};
                uint8_t bdaddr[6];
                BLUENRG_memcpy(bdaddr, SERVER_BDADDR, sizeof(SERVER_BDADDR));
                aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
                    CONFIG_DATA_PUBADDR_LEN,
                    bdaddr);
                break;
              }
      default: {
                 InfLoop();
               }
    }
    return;
  }
  if (evt != EVT_CMD_COMPLETE) InfLoop();
  switch(cc->opcode) {
    case to_opcode(OGF_VENDOR_CMD, OCF_GAP_SET_DISCOVERABLE):
      {
        state->status = USER_PROCESS_STATUS__INITIALIZED;
        break;
      }
    case to_opcode(OGF_LE_CTL, OCF_LE_SET_SCAN_RESPONSE_DATA):
      {
        const char local_name[] = "\nBlueNRG_Chat";
        aci_gap_set_discoverable(ADV_DATA_TYPE, ADV_INTERV_MIN, ADV_INTERV_MAX, PUBLIC_ADDR,
            NO_WHITE_LIST_USE, 12, local_name, 0, NULL, 0, 0);
        break;
      }
    case to_opcode(OGF_VENDOR_CMD, OCF_HAL_SET_TX_POWER_LEVEL):
      {
        hci_le_set_scan_resp_data(0,NULL);
        break;
      }
    case to_opcode(OGF_VENDOR_CMD, OCF_GATT_ADD_CHAR):
      {
        gatt_add_serv_rp* resp = (gatt_add_serv_rp*)cc->res;
        if (resp->status) InfLoop();

        state->simpCharHandle = btohs(resp->handle);
        aci_hal_set_tx_power_level(1,7);
        break;
      }
    case to_opcode(OGF_VENDOR_CMD, OCF_GATT_ADD_SERV):
      {
        gatt_add_serv_rp* resp = (gatt_add_serv_rp*)cc->res;
        if (resp->status) InfLoop();
        state->sampleServHandle = btohs(resp->handle);

        const uint8_t simp_char_uuid[16] = {0x66,0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe1,0xf2,0x73,0xd9};
        aci_gatt_add_char(state->sampleServHandle, // serviceHandle
            UUID_TYPE_16, //charUuidType
            simp_char_uuid, //const uint8_t* charUuid
            20, //uint8_t charValueLen
            CHAR_PROP_NOTIFY | CHAR_PROP_WRITE_WITHOUT_RESP | CHAR_PROP_WRITE, //uint8_t charProperties
            ATTR_PERMISSION_NONE, //uint8_t secPermissions
            GATT_NOTIFY_ATTRIBUTE_WRITE, //uint8_t gattEvtMask
            16, //uint8_t encryKeySize
            0, // uint8_t isVariable
            &state->simpCharHandle // uint16_t* charHandle
            );
        break;
      }
    case to_opcode(OGF_VENDOR_CMD, OCF_GAP_SET_AUTH_REQUIREMENT):
      {
        const uint8_t service_uuid[16] = {0x66,0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe0,0xf2,0x73,0xd9};
        aci_gatt_add_serv(UUID_TYPE_128, service_uuid, PRIMARY_SERVICE, 7, &state->sampleServHandle);
        break;
      }
    case to_opcode(OGF_VENDOR_CMD, OCF_GATT_UPD_CHAR_VAL):
      aci_gap_set_auth_requirement(MITM_PROTECTION_NOT_REQUIRED, OOB_AUTH_DATA_ABSENT, NULL, 7,
          16, USE_FIXED_PIN_FOR_PAIRING, 123456, BONDING);
      break;
    case to_opcode(OGF_VENDOR_CMD, OCF_GAP_INIT):
      {
        gap_init_rp* res = (gap_init_rp *)cc->res;
        state->service_handle = res->service_handle;
        state->dev_name_char_handle = res->dev_name_char_handle;
        state->appearance_char_handle = res->appearance_char_handle;
        aci_gatt_update_char_value(state->service_handle, state->dev_name_char_handle, 0, sizeof(name) - 1, (uint8_t *)name);
        break;
      }
    case to_opcode(OGF_VENDOR_CMD, OCF_GATT_INIT):
      aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x07, &state->service_handle, &state->dev_name_char_handle, &state->appearance_char_handle);
      break;

    case to_opcode(OGF_VENDOR_CMD, OCF_HAL_WRITE_CONFIG_DATA):
      // Mac address is written
      aci_gatt_init();
      break;
    default:
      InfLoop();
      break;
  }
}

void user_notify_initialized(blue_state_t* state, uint8_t event, evt_le_meta_event* meta) {
  if (event == EVT_LE_META_EVENT) {
    if (meta->subevent == EVT_LE_CONN_COMPLETE) {
      evt_le_connection_complete *cc = (evt_le_connection_complete *)meta->data;
      //GAP_ConnectionComplete_CB(cc->peer_bdaddr, cc->handle);
      memcpy(&state->connection, cc, sizeof(evt_le_connection_complete));
      state->status = USER_PROCESS_STATUS__CONNECTED;
      BSP_LED_On(LED1);
    }
  } else {
    InfLoop();
  }
}

void user_notify_connected(blue_state_t* state, uint8_t ev, evt_blue_aci* eaci, evt_cmd_complete* cc) {
  UNUSED(state);
  if (ev == EVT_VENDOR) {
    switch(eaci->ecode) {
      case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED:
        {
          evt_gatt_attr_modified_IDB05A1 *evt = (evt_gatt_attr_modified_IDB05A1*)eaci->data;
          add_event(state, evt->conn_handle, evt->attr_handle, evt->data_length, evt->att_data);
          state->changed_attrs_count++;
          if (state->changed_attrs_count == 2) {
            //sendText(state, "hello_world_from_bluecuc_this is the end of the world as we know it\n\n", 20);
            state->can_send = true;
            state->send_tick = uwTick;
          }
        }
        break;

      case EVT_BLUE_GATT_NOTIFICATION:
        {
          evt_gatt_attr_notification *evt = (evt_gatt_attr_notification*)eaci->data;
          add_event(state, evt->conn_handle, evt->attr_handle, evt->event_data_length, evt->attr_value);
          state->gat_notification_cb_count++;
        }
        break;

      case EVT_BLUE_GATT_DISC_READ_CHAR_BY_UUID_RESP:
        {
          PRINTF("EVT_BLUE_GATT_DISC_READ_CHAR_BY_UUID_RESP\n");
          evt_gatt_disc_read_char_by_uuid_resp *evt = (void*)eaci->data;
          add_event(state, evt->conn_handle, evt->attr_handle, evt->event_data_length, evt->attr_value);
          InfLoop();
        }
        break;

      case EVT_BLUE_GATT_PROCEDURE_COMPLETE:
        {
          /* Wait for gatt procedure complete event trigger related to Discovery Charac by UUID */
          evt_gatt_procedure_complete *pr = (void*)eaci->data;
          UNUSED(pr);
          InfLoop();
        }
        break;
      case EVT_BLUE_GATT_ERROR_RESP:
        {
          evt_gatt_error_resp * evt = (void*)eaci->data;
          uint8_t data[2] = { evt->req_opcode, evt->error_code };
          add_event(state, evt->conn_handle, evt->attr_handle, evt->event_data_length, data);
          InfLoop();
        }
        break;
      case EVT_BLUE_GATT_TX_POOL_AVAILABLE:
        {
          evt_gatt_tx_pool_available* p_ = (void*)eaci->data;
          UNUSED(p_);
          state->can_send = true;
          state->send_tick = uwTick + 10;
        }
        break;
      default:
        InfLoop();
    }
  } else if (ev == EVT_DISCONN_COMPLETE) {
    state->status = USER_PROCESS_STATUS__RESET;
    BSP_LED_Off(LED1);
  } else if (ev == EVT_CMD_COMPLETE) {
    if (cc->opcode == to_opcode(OGF_VENDOR_CMD, 0x106));
    else if (cc->opcode != OCF_GATT_UPD_CHAR_VAL)
      InfLoop();
    uint8_t status = cc->res[0];
    if (status) {
      state->number_of_unsuccessfull_sends++;
    }else {
      state->number_of_successfull_sends++;
      state->can_send = true;
    }
    state->send_tick = uwTick;

  }else {
    InfLoop();
  }
}

void user_notify(hci_uart_pckt* hci_pckt)
{
  if(hci_pckt->type != HCI_EVENT_PKT)
    return;

  hci_event_pckt *event_pckt = (hci_event_pckt*)hci_pckt->data;

  evt_cmd_complete  *cc =  (evt_cmd_complete *)(event_pckt->data);
  evt_le_meta_event *lme = (evt_le_meta_event *)event_pckt->data;
  evt_blue_aci *blue_evt = (evt_blue_aci *)event_pckt->data;

  if (blue_state.status == USER_PROCESS_STATUS__BEGIN) {
    user_notify_begin(&blue_state, event_pckt->evt, cc, blue_evt);
  } else if (blue_state.status == USER_PROCESS_STATUS__INITIALIZED) {
    user_notify_initialized(&blue_state, event_pckt->evt, lme);
  } else if (blue_state.status == USER_PROCESS_STATUS__CONNECTED) {
    user_notify_connected(&blue_state, event_pckt->evt, blue_evt, cc);
  } else if (blue_state.status == USER_PROCESS_STATUS__RESET) { }
  else {
    InfLoop();
  }
  return;
}

