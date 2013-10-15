#include "Comm.h"

#include <string.h>
#include <stdlib.h>

static const char 	StartByte 				= 0x02;
static const char 	EndByte 					= '\n';
static const int 		DataLen						= 2*sizeof(InputMsg); 	// If InputMsg is transmitted as hex string.

static int			InBufIdx; 
static char 		InBuf[2*DataLen];
static InputMsg	LastMsg;
static uint8_t 	NewMsg;

void Comm_Init(void)
{
	InBufIdx = 0;
	NewMsg = 0;
}

void Comm_Process(uint8_t* Buf, uint32_t Len)
{
	int startByteIdx;
	int msgDataStart = 0;
	int msgDataEnd;
	int i;
	char hexByteBuf[3];
	
	if(InBufIdx == 0)
	{
		// Looping until beginning of a valid message. In case there is garbage or partial old message in Buf.
		for(startByteIdx = 0 ; startByteIdx < Len && startByteIdx != StartByte ; startByteIdx++) 
			;
		if(startByteIdx == Len)
			return;		// Buf did not contain beginning of a message
		
		msgDataStart = startByteIdx + 1;
	}
	
	for(msgDataEnd = msgDataStart + 1; msgDataEnd < Len && Buf[msgDataEnd] != EndByte; msgDataEnd++)
		;
	
	//!!! TODO: Overflow checks!
	memcpy(InBuf + InBufIdx, Buf + msgDataStart, msgDataEnd - msgDataStart);		
	
	if(msgDataEnd < Len && Buf[msgDataEnd] == EndByte)
	{
		// We have a valid message in InBuf
		// assert(msgDataEnd - msgDataStart == 2*sizeof(InputMsg));
		hexByteBuf[2] = 0;
		for(i = 0 ; i < 2*DataLen ; i += 2)
		{
			memcpy(hexByteBuf, InBuf + i, 2);
			*((char*)&LastMsg) = strtoul(hexByteBuf, 0, 16);
		}
		NewMsg = 1;		
		InBufIdx = 0;
	}
	else
	{
		InBufIdx = msgDataEnd - msgDataStart;
	}
}

uint8_t Comm_NewMsg(InputMsg* out_msg)
{
	uint8_t hadNewMsg = NewMsg;
	NewMsg = 0;
	
	*out_msg = LastMsg;
	return hadNewMsg;
}
