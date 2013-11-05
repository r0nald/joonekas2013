#include <stdint.h>

/*
 * __packed packs the struct for Keil compiler. 
 * Not supported by gcc so for testing compile with -D__packed=/C comments/
 */
typedef __packed struct
{
	float 		leftPwm, rightPwm;
	uint8_t 	cmdType;
	uint8_t 	checksum;
} __attribute__((packed)) InputMsg;

typedef __packed struct
{
	uint32_t 	time;
	float			fval; 	
	float 		fval2;
	uint16_t 	lineSensors;
} __attribute__((packed)) OutputMsg;

extern uint32_t 	Comm_OutputMsgPacketLen;
extern uint8_t		Comm_TxBuffer[];

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
 * Returns non-zero only once.
 */
uint8_t Comm_NewMsg(void);

/*
 */
InputMsg Comm_LastMsg(void);

/*
 * Returns data with StartByte, hex string of msg data and EndByte
 * Assumes enough data(2*sizeof(OutputMsg) + 2) has been allocated.
 */
void 		Comm_OutMsgToStr(const OutputMsg* msg, char* out_str, uint16_t* out_len);

/*
 * Creates packet string with Comm_OutMsgToStr and transmits with VCP_DatTx
 */
void 		Comm_SendOutMsg(const OutputMsg* msg);
