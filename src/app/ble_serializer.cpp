#include "ble_serializer.hpp"
#include "sample_service.h"
#include <stdint.h>
#include <stm32l4xx_hal.h>

void GyroAccTemp_Serialize::fill_meta_data(uint8_t* buff) {
  buff[17] = 0x4A; //ui16, 2*i16
  buff[18] = 0xAA; //4*i16
  buff[19] = 0x4C; //ui16, i32
}

void GyroAccTemp_Serialize::next() {
  *this = circ_buff.peek();
  if (this->timestamp < last_timestamp) InfLoop();
}

bool GyroAccTemp_Serialize::has_next() { return circ_buff.len > 0; }

void GyroAccTemp_Serialize::confirm_data() {
  circ_buff.pop();
}

CircBuffer<GyroAccTemp_Serialize, GyroAccTemp_Serialize::circ_buff_len> GyroAccTemp_Serialize::circ_buff;
uint32_t GyroAccTemp_Serialize::last_timestamp = 0;


void BlueMetaData_Serialize::fill_meta_data(uint8_t* buff) {
  // 4*uint32_t, 2*uint16_t
  buff[17] = 0xCC; //ui32, ui32
  buff[18] = 0xCC; //ui32, ui32
  buff[19] = 0x80; //2*ui16
}

bool BlueMetaData_Serialize::has_next() { return sent_next < uwTick; }

void BlueMetaData_Serialize::next() {
  number_of_attempts_to_read_data = blue_state.number_of_attempts_to_read_data;
  blue_number_of_times_in_interupt = blue_state.blue_number_of_times_in_interupt;
  number_of_successfull_sends = blue_state.number_of_successfull_sends;
  number_of_unsuccessfull_sends = blue_state.number_of_unsuccessfull_sends;
  ro_q_len = (uint16_t)GyroAccTemp_Serialize::circ_buff.len;
  timestamp = (uint16_t)uwTick;
}
void BlueMetaData_Serialize::confirm_data() {
  BlueMetaData_Serialize::sent_next = uwTick + 100; // Send every 100 miliseconds
  this->next();
}

uint32_t BlueMetaData_Serialize::sent_next = 0;


using Send_BlueMetaData = BleSerializer<BlueMetaData_Serialize>;
using Send_GyroReadoutBLE = BleSerializer<GyroAccTemp_Serialize>;

Send_BlueMetaData send_ble;
Send_GyroReadoutBLE send_ro;

#define CHAR_LIST(X) X(send_ro, 0) \
  X(send_ble, 1)

extern "C" bool fill(int char_id, uint8_t* buff) {
  switch (char_id) {
#define X(name, id) case id: return name.send_next(buff);
    CHAR_LIST(X)
#undef X
    default: return false;
  }
}

extern "C" void confirm_sent(int char_id) {
  switch (char_id) {
#define X(name, id) case id: name.confirm(); break;
    CHAR_LIST(X)
#undef X
  }
}

extern "C" bool is_empty(int char_id) {
  switch (char_id) {
#define X(name, id) case id: return name.is_empty();
    CHAR_LIST(X)
#undef X
    default: return true;
  }
}

