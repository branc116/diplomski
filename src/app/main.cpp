#include "lsm6dsm.hpp"
#include "SensorTile.h"
#include "sample_service.h"


static int entry(void) {
  Lsm6dsm l{};
  uint32_t  t = uwTick + 90;
  uint32_t t2 = uwTick + 2;
  blue_init();
  initialize_blue_char_collection(&blue_state.chars);
  while(1) {
    if (blue_state.status == USER_PROCESS_STATUS__RESET) {
      blue_init();
      blue_state.number_of_resets++;
    }else if (blue_state.status == USER_PROCESS_STATUS__CONNECTED)  {
      if (!blue_send_next(&blue_state.chars)) {
        t = (int)uwTick;
      }
      else if (uwTick > t) {
        hci_notify_asynch_evt();
        t = uwTick + 90;
      }
      if (t2 < uwTick) {
        GyroAccTemp_Serialize::circ_buff.push(l.get_readout());
        t2 = uwTick + 10;
      }
    } else if (uwTick > t) {
      hci_notify_asynch_evt();
      t = uwTick + 90;
    }
  }
}

extern "C" int main_cpp(void) {
  entry();
  return 0;
}

