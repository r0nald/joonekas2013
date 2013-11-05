#include "Comm.h"
#include "usbd_cdc_vcp.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

uint32_t 	Comm_OutputMsgPacketLen = 2*sizeof(OutputMsg) + 2;
uint8_t		Comm_TxBuffer[2*sizeof(OutputMsg) + 2];

static const char 	StartByte 				= 0x02;
static const char 	EndByte 					= '\n';
static const int 	DataLen						= 2*sizeof(InputMsg); 	// If InputMsg is transmitted as hex string.

static int			InBufIdx;
static char 		InBuf[2*2*sizeof(InputMsg)];
static InputMsg	LastMsg;
static uint8_t 	NewMsg;

void Comm_Init(void)
{
	InBufIdx = -1;
	NewMsg = 0;
}

void Comm_Process(uint8_t* Buf, uint32_t Len)
{
	int msgDataStart = 0;       // First byte of data in Buf. Byte after StartByte.
	int msgDataEnd;             // Last byte of data + 1. EndByte index.
	int i;
	char hexByteBuf[3];
	
	if(Len == 0)
        return;

	if(InBufIdx == -1)
	{
	    // We have not received StartByte. Looping until we find StartByte
		for(msgDataStart = 0 ; msgDataStart < Len && Buf[msgDataStart] != StartByte ; msgDataStart++)
            ;

		if(msgDataStart == Len)
            return;		// Buf did not contain beginning of a message

        // First byte of valid message is Buf[msgDataStart]
        msgDataStart++;
        InBufIdx = 0;
    }

    if(msgDataStart == 1 && Len == 1)
        return;         // Received only StartByte

	for(msgDataEnd = msgDataStart + 1; msgDataEnd < Len && Buf[msgDataEnd] != EndByte; msgDataEnd++)
		;

    if(InBufIdx + (msgDataEnd - msgDataStart) > DataLen)
    {
        // If we have received more data than should be one packet, start again.
        InBufIdx = -1;
        return;
    }

    memcpy(InBuf + InBufIdx, Buf + msgDataStart, msgDataEnd - msgDataStart);

	if(InBufIdx + (msgDataEnd-msgDataStart) == DataLen && Buf[msgDataEnd] == EndByte)
	{
		// We have a valid message in InBuf
		// assert(msgDataEnd - msgDataStart == 2*sizeof(InputMsg));
		hexByteBuf[2] = 0;
		for(i = 0 ; i < DataLen ; i += 2)
		{
			memcpy(hexByteBuf, InBuf + i, 2);
			*( ((char*)&LastMsg) + i/2) = strtoul(hexByteBuf, 0, 16);
		}
		NewMsg = 1;
		InBufIdx = -1;
	}
	else
	{
		InBufIdx += msgDataEnd - msgDataStart;
	}
}

uint8_t Comm_NewMsg(void)
{
	uint8_t hadNewMsg = NewMsg;
	NewMsg = 0;
	return hadNewMsg;
}

/*
 */
InputMsg Comm_LastMsg(void)
{
	return LastMsg;
}

void 		Comm_SendOutMsg(const OutputMsg* msg)
{
	char 			outBuf[2*sizeof(OutputMsg)+3];
	uint16_t 	len;
	
	Comm_OutMsgToStr(msg, outBuf, &len);
	VCP_DataTx((uint8_t*)outBuf, len);
}

void 		Comm_OutMsgToStr(const OutputMsg* msg, char* out_str, uint16_t* out_len)
{
	int i;
	out_str[0] = StartByte;
	for(i = 0 ; i < sizeof(OutputMsg) ; i++)
	{
		sprintf(out_str + 1 + 2*i, "%02x", (unsigned int) *( ((char*)msg) + i));
	}
	out_str[2*sizeof(OutputMsg)+1] = EndByte;
	*out_len = 2*sizeof(OutputMsg)+2;
}
