#include "app_bluenrg_ms.h"

#include "SensorTile.h"
#include "hci_tl.h"
#include "sample_service.h"
#include "bluenrg_utils.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_hal_aci.h"
#include <stdint.h>

extern volatile uint8_t set_connectable;
extern volatile int     connected;
extern volatile uint8_t notification_enabled;

extern volatile uint8_t end_read_tx_char_handle;
extern volatile uint8_t end_read_rx_char_handle;

static void User_Process(void);

uint32_t MX_BlueNRG_MS_Init(void)
{
  blue_state.status = USER_PROCESS_STATUS__BEGIN;
  hci_init();
  
  return 0;
}

static int blue_number_of_times_in_ms_process = 0;
/*
 * BlueNRG-MS background task
 */
void MX_BlueNRG_MS_Process(void)
{
  blue_number_of_times_in_ms_process++;
  hci_notify_asynch_evt();
}

volatile int succ = 0;
volatile int unsucc = 0;
int bip = 0;

/**
 * @brief  Configure the device as Client or Server and manage the communication
 *         between a client and a server.
 *
 */
static void User_Process(void)
{
//    int res = sendText("hello_world_from_bluecuc_this is the end of the world as we know it\n\n", 16);
}

