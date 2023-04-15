// BlueNRG-M0 initialization and applicative code
#include "app_bluenrg_ms.h"

#include "hci_tl.h"
#include "sample_service.h"
#include "bluenrg_utils.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_hal_aci.h"
#include <stdint.h>

/**
 * Define the role here only if it is not already defined in the project options
 * For the CLIENT_ROLE comment the line below
 * For the SERVER_ROLE uncomment the line below
 */
#define SERVER_ROLE

#define BDADDR_SIZE 6

uint8_t bnrg_expansion_board = IDB05A1;
static volatile uint8_t user_button_init_state = 1;
static volatile uint8_t user_button_pressed = 0;

extern volatile uint8_t set_connectable;
extern volatile int     connected;
extern volatile uint8_t notification_enabled;

extern volatile uint8_t end_read_tx_char_handle;
extern volatile uint8_t end_read_rx_char_handle;

static void User_Process(void);
static void User_Init(void);

#if PRINT_CSV_FORMAT
extern volatile uint32_t ms_counter;
/**
 * @brief  This function is a utility to print the log time
 *         in the format HH:MM:SS:MSS (DK GUI time format)
 */
void print_csv_time(void){
  uint32_t ms = HAL_GetTick();
  PRINT_CSV("%02ld:%02ld:%02ld.%03ld", (long)(ms/(60*60*1000)%24), (long)(ms/(60*1000)%60), (long)((ms/1000)%60), (long)(ms%1000));
}
#endif
volatile int break_ret(volatile int i) {
  return i;
}

uint32_t MX_BlueNRG_MS_Init(void)
{
  /* Initialize the peripherals and the BLE Stack */
  uint8_t CLIENT_BDADDR[] = {0xbb, 0x00, 0x00, 0xE1, 0x80, 0x02};
  uint8_t SERVER_BDADDR[] = {0xaa, 0x00, 0x00, 0xE1, 0x80, 0x02};
  uint8_t bdaddr[BDADDR_SIZE];
  uint16_t service_handle, dev_name_char_handle, appearance_char_handle;

  uint8_t  hwVersion = 49;
  uint16_t fwVersion = 1827;
  int ret;

  User_Init();

  hci_init(user_notify, NULL);  if (0) {
    BLUENRG_memcpy(bdaddr, CLIENT_BDADDR, sizeof(CLIENT_BDADDR));
  } else {
    BLUENRG_memcpy(bdaddr, SERVER_BDADDR, sizeof(SERVER_BDADDR));
  }

  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
                                  CONFIG_DATA_PUBADDR_LEN,
                                  bdaddr);
  if (ret) {
    printf("Setting BD_ADDR failed 0x%02x.\n", ret);
    return break_ret(ret);
  }

  ret = aci_gatt_init();
  if (ret) {
    printf("GATT_Init failed.\n");
    return break_ret(ret);
  }
  ret = aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);

  if (ret != BLE_STATUS_SUCCESS) {
    printf("GAP_Init failed.\n");
    return break_ret(ret);
  }

  const char name[] = "BlueNRG";
  ret = aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0, sizeof(name) - 1, (uint8_t *)name);
  if(ret){
    PRINTF("aci_gatt_update_char_value failed.\n");
    return break_ret(ret);
  }

  ret = aci_gap_set_auth_requirement(MITM_PROTECTION_NOT_REQUIRED,
                                     OOB_AUTH_DATA_ABSENT,
                                     NULL,
                                     7,
                                     16,
                                     USE_FIXED_PIN_FOR_PAIRING,
                                     123456,
                                     BONDING);
  if (ret == BLE_STATUS_SUCCESS) {
    printf("BLE Stack Initialized.\n");
  }

  if (1) {
    printf("SERVER: BLE Stack Initialized\n");
    ret = Add_Sample_Service();

    if (ret == BLE_STATUS_SUCCESS)
      printf("Service added successfully.\n");
    else {
      printf("Error while adding service.\n");
      return break_ret(ret);
    }

  } else {
    printf("CLIENT: BLE Stack Initialized\n");
  }

  /* Set output power level */
  ret = aci_hal_set_tx_power_level(1,4);
  return break_ret(ret);
}

/*
 * BlueNRG-MS background task
 */
void MX_BlueNRG_MS_Process(void)
{
  User_Process();
  hci_user_evt_proc();
}

/**
 * @brief  Initialize User process.
 *
 */
static void User_Init(void)
{
  //BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
  //BSP_LED_Init(LED2);

  //BSP_COM_Init(COM1);
}
static void Debug(void) {
  uint8_t link_status[8] = {0};
  uint16_t conn_handle[8] = {0};
  uint16_t build_number = 0;
  uint8_t hwVersion = 0;
  uint16_t fwVersion = 0;

  volatile uint32_t stat = aci_hal_get_link_status(link_status, conn_handle);
  stat = aci_hal_get_fw_build_number(&build_number);
  stat = getBlueNRGVersion(&hwVersion, &fwVersion);
}

volatile int succ = 0;
volatile int unsucc = 0;

/**
 * @brief  Configure the device as Client or Server and manage the communication
 *         between a client and a server.
 *
 */
static void User_Process(void)
{
  if (set_connectable)
  {
    /* Establish connection with remote device */
    Make_Connection();
    //user_button_init_state = BSP_PB_GetState(BUTTON_KEY);
  }
  process_idle();
  if (connected) {
    int ok = sendText("hello", 5);
    if (ok) {
      unsucc--;
    }
    else {
      succ++;
    }

  }

//  if (1)
//  {
//    /* Start TX handle Characteristic dynamic discovery if not yet done */
//    if (connected && !end_read_tx_char_handle){
//      startReadTXCharHandle();
//    }
//    /* Start RX handle Characteristic dynamic discovery if not yet done */
//    else if (connected && !end_read_rx_char_handle){
//      startReadRXCharHandle();
//    }
//
//    if (connected && end_read_tx_char_handle && end_read_rx_char_handle && !notification_enabled)
//    {
//      //BSP_LED_Off(LED2); //end of the connection and chars discovery phase
//      enableNotification();
//    }
//  }
//
//  /* Check if the User Button has been pushed */
//  if (user_button_pressed)
//  {
//    /* Debouncing */
//    HAL_Delay(50);
//
//    /* Wait until the User Button is released */
//    //while (BSP_PB_GetState(BUTTON_KEY) == !user_button_init_state);
//
//    /* Debouncing */
//    HAL_Delay(50);
//
//    if (connected && notification_enabled)
//    {
//      /* Send a toggle command to the remote device */
//      uint8_t data[20] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J'};
//      sendData(data, sizeof(data));
//
//      //BSP_LED_Toggle(LED2);  /* Toggle the LED2 locally. */
//                               /* If uncommented be sure the BSP_LED_Init(LED2)
//                                * is called in main().
//                                * E.g. it can be enabled for debugging. */
//    }
//
//    /* Reset the User Button flag */
//    user_button_pressed = 0;
//  }
}

/**
  * @brief  BSP Push Button callback
  * @param  Button Specifies the pin connected EXTI line
void BSP_PB_Callback(Button_TypeDef Button)
{
  //Set the User Button flag
  user_button_pressed = 1;
}
*/
