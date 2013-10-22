#include "joonekas.h"

#include "Comm.h"

void Joonekas_SysTick(void)
{
	OutputMsg msg;
	Comm_SendOutMsg(&msg);	
}
