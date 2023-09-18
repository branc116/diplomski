#pragma once
#include "stdint.h"
#include "string.h"
#include "stm32l4xx_hal.h"
#include "circ_buffer.hpp"

//    NODATA  = 0
//    USHORT  = 4
//    SSHORT  = 6
//    USHORT2 = 8
//    SSHORT2 = 10
//    ULONG   = 12
//    SLONG   = 14
//    SFLOAT   = 15
template<typename TSelf>
struct BleSerializer : public TSelf {
  void fill_controll(uint8_t* buff) {
    static_assert(sizeof(TSelf) <= 20);
    for (int i = 0; i < 17; ++i) {
      buff[i] = (i % 2) * 0xFF;
    }
    this->fill_meta_data(buff);
  }
  bool send_next(uint8_t* buff) {
    if (send_next_controll < uwTick) {
      fill_controll(buff);
      return true;
    }
    if (this->has_next()) {
      this->next();
      memcpy(buff, (TSelf*)this, sizeof(TSelf));
      return true;
    }
    return false;
  }
  void confirm() {
    if (send_next_controll < uwTick) send_next_controll = uwTick + 3000;
    else this->confirm_data();
  }
  bool is_empty() {
    return !this->has_next() && send_next_controll > uwTick;
  }
  uint32_t send_next_controll;
};

struct [[gnu::packed]] GyroAccTempReadout  {
  uint16_t temperature;
  int16_t acceleration[3];
  int16_t angular[3];
};

struct [[gnu::packed]] GyroAccTemp_Serialize {
  static CircBuffer<GyroAccTemp_Serialize, 16> circ_buff;

  void fill_meta_data(uint8_t* buff); 
  void next();
  bool has_next();
  void confirm_data();

  GyroAccTempReadout ro;
  uint16_t index;
  uint32_t timestamp;
};


struct [[gnu::packed]] BlueMetaData_Serialize {
  void fill_meta_data(uint8_t* buff);
  bool has_next();
  void next(); 
  void confirm_data() {}
  uint32_t number_of_attempts_to_read_data;
  uint32_t blue_number_of_times_in_interupt;
  uint32_t number_of_send_attempts, number_of_unsuccessfull_sends;
  uint16_t number_of_resets;
  uint16_t timestamp;
};

