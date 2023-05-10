#ifndef HCI_TL_H
#define HCI_TL_H

#include "hci_tl_interface.h"
#include "bluenrg_types.h"
#include "bluenrg_conf.h"

/**
 * @brief Structure hosting the HCI request
 */
struct hci_request {
  uint16_t ogf;     /**< Opcode Group Field */
  uint16_t ocf;     /**< Opcode Command Field */
  uint32_t event;   /**< HCI Event */
  void     *cparam; /**< HCI Command from MCU to Host */
  uint32_t clen;    /**< Command Length */
  void     *rparam; /**< Response from Host to MCU */
  uint32_t rlen;    /**< Response Length */
};

/**
 * @brief Structure used to read received HCI data packet
 */
typedef struct _tHciDataPacket
{
  uint8_t   dataBuff[HCI_READ_PACKET_SIZE];
  uint8_t   data_len;
} tHciDataPacket;

/**
 * @brief Structure used to manage the BUS IO operations.
 *        All the structure fields will point to functions defined at user level.
 */
//typedef struct
//{
//  int32_t (* Init)    (void* pConf); /**< Pointer to HCI TL function for the IO Bus initialization */
//  int32_t (* DeInit)  (void); /**< Pointer to HCI TL function for the IO Bus de-initialization */
//  void    (* Reset)   (void); /**< Pointer to HCI TL function for the IO Bus reset */
//  int32_t (* Receive) (uint8_t*, uint16_t); /**< Pointer to HCI TL function for the IO Bus data reception */
//  int32_t (* Send)    (uint8_t*, uint16_t); /**< Pointer to HCI TL function for the IO Bus data transmission */
//  int32_t (* DataAck) (uint8_t*, uint16_t* len); /**< Pointer to HCI TL function for the IO Bus data ack reception */
//  uint32_t (* GetTick) (void); /**< Pointer to BSP function for getting the HAL time base timestamp */
//} tHciIO;

/**
 * @brief Describe the HCI flow status
 */
typedef enum
{
  HCI_DATA_FLOW_DISABLE = 0,
  HCI_DATA_FLOW_ENABLE,
} tHciflowStatus;

/**
 * @brief Contain the HCI context
 */
//typedef struct
//{
//  tHciIO io; /**< Manage the BUS IO operations */
//} tHciContext;

/**
 * @brief  Send an HCI request either in synchronous or in asynchronous mode.
 *
 * @param  r: The HCI request
 * @param  async: TRUE if asynchronous mode, FALSE if synchronous mode
 * @retval int: 0 when success, -1 when failure
 */
int hci_send_req(struct hci_request *r, BOOL async);

void process_idle(void);

/**
 * @brief  Interrupt service routine that must be called when the BlueNRG
 *         reports a packet received or an event to the host through the
 *         BlueNRG interrupt line.
 *
 * @param  pdata Packet or event pointer
 * @retval 0: packet/event processed, 1: no packet/event processed
 */
int32_t hci_notify_asynch_evt(void);

/**
 * @brief  This function resume the User Event Flow which has been stopped on return
 *         from UserEvtRx() when the User Event has not been processed.
 *
 */
void hci_resume_flow(void);

/**
 * @brief  This function is called when an ACI/HCI command is sent and the response
 *         is waited from the BLE core.
 *         The application shall implement a mechanism to not return from this function
 *         until the waited event is received.
 *         This is notified to the application with hci_cmd_resp_release().
 *         It is called from the same context the HCI command has been sent.
 *
 * @param  timeout: Waiting timeout
 */
void hci_cmd_resp_wait(uint32_t timeout);

/**
 * @brief  This function is called when an ACI/HCI command is sent and the response is
 *         received from the BLE core.
 *
 * @param  flag: Release flag
 */
void hci_cmd_resp_release(uint32_t flag);

#endif /* HCI_TL_H */

