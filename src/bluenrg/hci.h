#ifndef HCI_H
#define HCI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bluenrg_types.h"

/**
 * @brief Initialize the Host Controller Interface.
 *        This function must be called before any data can be received
 *        from BLE controller.
 *
 * @param  UserEvtRx: ACI events callback function pointer
 *         This callback is triggered when an user event is received from
 *         the BLE core device.
 * @param  pConf: Configuration structure pointer
 */
void hci_init(void* pConf);

/**
 * @brief  Processing function that must be called after an event is received from
 *         HCI interface.
 *         It must be called outside ISR. It will call user_notify() if necessary.
 *
 */
void hci_user_evt_proc(void);

#ifdef __cplusplus
}
#endif

#endif /* HCI_H */

