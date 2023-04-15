#include "DemoSerial.h"
#include "main.h"
#include "vcom.h"
#include "sensor_commands.h"

extern uint32_t Sensors_Enabled;
extern uint32_t DataTxPeriod;
extern uint8_t DataLoggerActive;

volatile uint8_t SenderInterface = 0;
uint8_t PresentationString[] = {"MEMS shield demo,101,4.0.0,0.0.0,STLKT01V1"};
volatile uint8_t DataStreamingDest = 1;

/**
  * @brief  Build the reply header
  * @param  Msg the pointer to the message to be built
  */
void BUILD_REPLY_HEADER(TMsg *Msg)
{
  Msg->Data[0] = Msg->Data[1];
  Msg->Data[1] = DEV_ADDR;
  Msg->Data[2] += CMD_Reply_Add;
}

/**
  * @brief  Build the nack header
  * @param  Msg the pointer to the message to be built
  */
void BUILD_NACK_HEADER(TMsg *Msg)
{
  Msg->Data[0] = Msg->Data[1];
  Msg->Data[1] = DEV_ADDR;
  Msg->Data[2] = CMD_NACK;
}

/**
  * @brief  Initialize the streaming header
  * @param  Msg the pointer to the header to be initialized
  */
void INIT_STREAMING_HEADER(TMsg *Msg)
{
  Msg->Data[0] = DataStreamingDest;
  Msg->Data[1] = DEV_ADDR;
  Msg->Data[2] = CMD_Start_Data_Streaming;
  Msg->Len = 3;
}

/**
  * @brief  Initialize the streaming message
  * @param  Msg the pointer to the message to be initialized
  */
void INIT_STREAMING_MSG(TMsg *Msg)
{
  uint8_t i;

  Msg->Data[0] = DataStreamingDest;
  Msg->Data[1] = DEV_ADDR;
  Msg->Data[2] = CMD_Start_Data_Streaming;
  for(i = 3; i < STREAMING_MSG_LENGTH + 3; i++)
  {
    Msg->Data[i] = 0;
  }
  Msg->Len = 3;

}

/**
  * @brief  Handle a message
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
int HandleMSG(TMsg *Msg)
//  DestAddr | SouceAddr | CMD | PAYLOAD
//      1          1        1       N
{
  uint32_t i;

  if (Msg->Len < 2) return 0;
  if (Msg->Data[0] != DEV_ADDR) return 0;
  switch (Msg->Data[2])   // CMD
  {
    case CMD_Ping:
      if (Msg->Len != 3) return 0;
      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 3;
      VCOM_send_MSG(Msg);
      return 1;

    case CMD_Enter_DFU_Mode:
      if (Msg->Len != 3) return 0;
      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 3;
      return 1;

    case CMD_Read_PresString:
      if (Msg->Len != 3) return 0;
      BUILD_REPLY_HEADER(Msg);
      i = 0;
      while (i < (sizeof(PresentationString) - 1))
      {
        Msg->Data[3 + i] = PresentationString[i];
        i++;
      }

      Msg->Len = 3 + i;
      VCOM_send_MSG(Msg);
      return 1;

    case CMD_CheckModeSupport:
      if (Msg->Len < 3) return 0;
      BUILD_REPLY_HEADER(Msg);
      Serialize_s32(&Msg->Data[3], DATALOG_EXT_MODE, 4);
      Msg->Len = 3 + 4;
      VCOM_send_MSG(Msg);
      return 1;

    case CMD_Sensor:
      /* Check if the command lenght is at least 5 bytes */
      if (Msg->Len < 5) return 0;
      Handle_Sensor_command(Msg);
      return 1;

    case CMD_Start_Data_Streaming:
      if (Msg->Len < 3) return 0;
      Sensors_Enabled = Deserialize(&Msg->Data[3], 4);
      //DataTxPeriod = Deserialize(&Msg->Data[7], 4);
      DataLoggerActive = 1;
      DataStreamingDest = Msg->Data[1];
      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 3;
      VCOM_send_MSG(Msg);
      return 1;

    case CMD_Stop_Data_Streaming:
      if (Msg->Len < 3) return 0;
      Sensors_Enabled = 0;
      DataLoggerActive = 0;
      BUILD_REPLY_HEADER(Msg);
      VCOM_send_MSG(Msg);
      return 1;

    case CMD_Set_DateTime:
      if (Msg->Len < 3) return 0;
      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 3;
      RTC_TimeRegulate(Msg->Data[3], Msg->Data[4], Msg->Data[5]);
      VCOM_send_MSG(Msg);
      return 1;

    default:
      return 0;
  }
}
