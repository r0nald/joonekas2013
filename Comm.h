#include <stdint.h>

typedef struct
{
	uint8_t 	cmdType;
	float 		leftPwm, rightPwm;
	uint8_t 	checksum;
} InputMsg;

/* 
 * Call this to initialize internal states of Comm
 */
void Comm_Init(void);

/*
 * This function should be called if data is received from (USB) serial port
 */
void Comm_Process(uint8_t* Buf, uint32_t Len);

/*
 * This command will return non-zero, if a new command has been received.
 * Returns non-zero only once. If out_msg argument is non-zero, will output last message
 */
uint8_t Comm_NewMsg(InputMsg* out_msg);
