#include "lsm6dsm.hpp"
#include "SensorTile.h"
#include "sample_service.h"
#include "string.h"


static int entry(void) {
  Lsm6dsm l{};
  uint32_t  t = uwTick + 10;
  uint32_t t2 = uwTick + 2;
  memset(&blue_state, 0, sizeof(blue_state));
  blue_init();
  initialize_blue_char_collection(&blue_state.chars);

  while(1) {
    if (blue_state.status == USER_PROCESS_STATUS__RESET) {
      blue_init();
      blue_state.number_of_resets++;
    }else if (blue_state.status == USER_PROCESS_STATUS__CONNECTED)  {
      if (!blue_send_next(&blue_state.chars)) {
        t = uwTick;
      }
      else if (uwTick > t) {
        hci_notify_asynch_evt();
        t = uwTick + 5;
      }
      if (t2 < uwTick) {
        GyroAccTemp_Serialize::circ_buff.push(l.get_readout());
        t2 = uwTick + 6;
      }
    } else if (uwTick > t) {
      hci_notify_asynch_evt();
      t = uwTick + 5;
    }
  }
}

extern "C" int main_cpp(void) {
  entry();
  return 0;
}

