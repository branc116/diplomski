#ifndef __SERIAL_PROTOCOL__
#define __SERIAL_PROTOCOL__

#include <stdint.h>

#define TMsg_EOF                0xF0
#define TMsg_BS                 0xF1
#define TMsg_BS_EOF             0xF2


#ifdef USE_USB_OTG_HS
#define TMsg_MaxLen             512
#else
#define TMsg_MaxLen             256
#endif

/* _NOTE_: USB buffer size is set in 'usbd_cdc_interface.c' and should be
   at least of size: (2 * TMsg_MaxLen)
*/

/**
 * @brief  Serial message structure definition
 */
typedef struct
{
  uint32_t Len;
  uint8_t Data[TMsg_MaxLen];
} TMsg;

int ByteStuffCopyByte(uint8_t *Dest, uint8_t Source);
int ReverseByteStuffCopyByte2(uint8_t Source0, uint8_t Source1, uint8_t *Dest);
int ByteStuffCopy(uint8_t *Dest, TMsg *Source);
int ReverseByteStuffCopyByte(uint8_t *Source, uint8_t *Dest);
int ReverseByteStuffCopy(TMsg *Dest, uint8_t *Source);
void CHK_ComputeAndAdd(TMsg *Msg);
int CHK_CheckAndRemove(TMsg *Msg);
uint32_t Deserialize(uint8_t *Source, uint32_t Len);
int32_t Deserialize_s32(uint8_t *Source, uint32_t Len);
void Serialize(uint8_t *Dest, uint32_t Source, uint32_t Len);
void Serialize_s32(uint8_t *Dest, int32_t Source, uint32_t Len);

#endif /* __SERIAL_PROTOCOL__ */

