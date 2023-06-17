#ifndef SAMPLE_SERVICE_H
#define SAMPLE_SERVICE_H

#include <stdint.h>
#include <stdbool.h>
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

#define BLUE_CHAR_STREAM_CAPACITY 512
typedef struct {
  uint16_t serv_handle, char_handle;
  uint8_t char_size;
  bool is_initialized, waiting_for_confirm;
  int sent_attempts, sent_packets;
  uint32_t send_ticks;
  uint16_t last_len, location, size;
  uint8_t char_uuid[2];
  uint8_t data[BLUE_CHAR_STREAM_CAPACITY];
} blue_char_stream_t;

#define BLUE_EVENTS_CAPACITY 32
#define BLUE_CHAN_SIZE 20
#define BLUE_NUMBER_OF_CHAR_STREAMS 1

typedef struct {
  int waiting_for_confirm;
  int sent_packets, initialized_streams, last_sent;
  blue_char_stream_t char_streams[BLUE_NUMBER_OF_CHAR_STREAMS];
} blue_char_collection_t;

void initialize_blue_char_collection(blue_char_collection_t* coll);

typedef struct {
  user_process_status_t status;
  uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
  evt_le_connection_complete connection;
  int changed_attrs_count, gat_notification_cb_count;
  int number_of_resets, number_of_attempts_to_read_data;
  int blue_number_of_times_in_interupt;
  int number_of_send_attempts, number_of_unsuccessfull_sends, number_of_successfull_sends;
  int number_of_events;
  uint32_t write_history;
  bool everything_inited;
  blue_char_collection_t chars;
  blue_event_modified_t events[BLUE_EVENTS_CAPACITY];
} blue_state_t;

void initialize_blue_char_collection(blue_char_collection_t* coll);
int blue_send_char_stream(blue_char_stream_t* state);
int blue_char_stream_push_int16(blue_char_stream_t* state, int16_t val);
int blue_send_next(blue_char_collection_t* coll);
void user_notify(hci_uart_pckt* pData);

extern blue_state_t blue_state;

#ifdef __cplusplus
}
#endif

#endif /* SAMPLE_SERVICE_H */
