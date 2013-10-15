#include <stdint.h>

typedef struct
{
	uint8_t 	cmdType;
	float 		leftPwm, rightPwm;
	uint8_t 	checksum;
} InputMsg;