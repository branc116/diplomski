#ifndef SAMPLE_SERVICE_H
#define SAMPLE_SERVICE_H

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

tBleStatus Add_Sample_Service(void);
void Make_Connection(void);
void receiveData(uint8_t* data_buffer, uint8_t Nb_bytes);
void sendData(const uint8_t* data_buffer, uint8_t Nb_bytes);
int sendText(const char* data_buffer, uint8_t Nb_bytes);
void startReadTXCharHandle(void);
void startReadRXCharHandle(void);
void enableNotification(void);
void Attribute_Modified_CB(uint16_t handle, uint8_t data_length,
                           uint8_t *att_data);
void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle);
void GAP_DisconnectionComplete_CB(void);
void GATT_Notification_CB(uint16_t attr_handle, uint8_t attr_len,
                          uint8_t *attr_value);
void user_notify(void * pData);

#ifdef __cplusplus
}
#endif

#endif /* SAMPLE_SERVICE_H */
