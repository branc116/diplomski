#include "app_bluenrg_ms.h"
#include "sample_service.h"
#include <stdint.h>

uint32_t MX_BlueNRG_MS_Init(void)
{
  blue_state.status = USER_PROCESS_STATUS__BEGIN;
  hci_init();
  return 0;
}
