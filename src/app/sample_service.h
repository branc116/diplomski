#ifndef SAMPLE_SERVICE_H
#define SAMPLE_SERVICE_H

#include <stdint.h>
#include <string.h>
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>

#include "bluenrg_gap.h"
#include "bluenrg_aci_const.h"
#include "hci.h"
#include "hci_le.h"
#include "sm.h"

#define IDB04A1 0
#define IDB05A1 1

/**
* @brief Handle of TX Characteristic on the Server. The handle should be
*        discovered, but it is fixed only for this demo.
*/
//#define TX_HANDLE 0x0011

/**
* @brief Handle of RX Characteristic on the Client. The handle should be
*        discovered, but it is fixed only for this demo.
*/
//#define RX_HANDLE   0x0014

typedef enum {
  USER_PROCESS_STATUS__BEGIN,
  USER_PROCESS_STATUS__INITIALIZED,
  USER_PROCESS_STATUS__CONNECTED,
  USER_PROCESS_STATUS__RESET
} user_process_status_t;
typedef struct {
  uint16_t conn_handle; /**< The connection handle which modified the attribute. */
  uint16_t attr_handle; /**< Handle of the attribute that was modified. */
  uint8_t  data_length; /**< The length of the data */
  uint8_t  att_data[128]; /**< The new value (length is data_length) */
  uint16_t number_of_changes;
} blue_event_modified_t;

#define BLUE_EVENTS_CAPACITY 32
typedef struct {
  user_process_status_t status;
  uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
  uint16_t sampleServHandle, simpCharHandle;
  evt_le_connection_complete connection;
  int changed_attrs_count, gat_notification_cb_count;
  int number_of_resets, number_of_attempts_to_read_data;
  int blue_number_of_times_in_interupt;
  int number_of_send_attempts, number_of_unsuccessfull_sends, number_of_successfull_sends;
  int number_of_events;
  blue_event_modified_t events[BLUE_EVENTS_CAPACITY];
} blue_state_t;

tBleStatus Add_Sample_Service(void);
void Make_Connection(void);
void receiveData(uint8_t* data_buffer, uint8_t Nb_bytes);
int sendText(blue_state_t* state, const char* data_buffer, uint8_t Nb_bytes);
void startReadTXCharHandle(void);
void startReadRXCharHandle(void);
void enableNotification(void);
void Attribute_Modified_CB(uint16_t handle, uint8_t data_length,
                           uint8_t *att_data);
void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle);
void GAP_DisconnectionComplete_CB(void);
void GATT_Notification_CB(uint16_t attr_handle, uint8_t attr_len,
                          uint8_t *attr_value);
void user_notify(hci_uart_pckt* pData);

extern blue_state_t blue_state;

#ifdef __cplusplus
}
#endif

#endif /* SAMPLE_SERVICE_H */
