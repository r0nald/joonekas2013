#include "Comm.h"

#include <string.h>
#include <stdlib.h>

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
		for(i = 0 ; i < 2*DataLen ; i += 2)
		{
			memcpy(hexByteBuf, InBuf + i, 2);
			*((char*)&LastMsg + i/2) = strtoul(hexByteBuf, 0, 16);
		}
		NewMsg = 1;
		InBufIdx = -1;
	}
	else
	{
		InBufIdx += msgDataEnd - msgDataStart;
	}
}

uint8_t Comm_NewMsg(InputMsg* out_msg)
{
	uint8_t hadNewMsg = NewMsg;
	NewMsg = 0;

	*out_msg = LastMsg;
	return hadNewMsg;
}
