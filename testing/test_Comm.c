#include "test_Comm.h"

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void test_Comm()
{
    int i;
    InputMsg msg;
    char c;
    msg.cmdType = 1;
    msg.leftPwm = 10.0; msg.rightPwm = -1337;
    msg.checksum = 0;

    printf("Sizeof InputMsg %d\n", sizeof(InputMsg));
    char* msgStr = msgAsStr(msg);
    printf("Len of msgStr : %d\n", strlen(msgStr));

    char* msgBuf = malloc(strlen(msgStr) + 3);

    msgBuf[0] = 0x02; msgBuf[strlen(msgStr)+1] = '\n'; msgBuf[strlen(msgStr) + 2] = 0;
    memcpy(msgBuf + 1, msgStr, strlen(msgStr));

    printf("%s", msgBuf);

    Comm_Init();

    char lotsOfTrash[] = "lkjhdflkjaslfhsjfhslkdhfhsalkfhsdjfhlkshfhflkhsdflkhsadlkfhsalkjfhsalkdhflkjsahflkjshflkjlkjhlkjhsflkjdsfkjsfkjhsjfkashf";
    /*
     * First lets test if still works after overflow of trash.
     */
    Comm_Process(lotsOfTrash, strlen(lotsOfTrash));

    /*
     * Let's send just perfect message and see if it works.
     */
    Comm_Process(msgBuf, strlen(msgStr) + 2);

    InputMsg recvMsg;

    recvMsg.leftPwm = 0; recvMsg.rightPwm = 0;
    if(Comm_NewMsg() == 1)
        printf("New message detected!\n");
    else
        printf("!!! New message NOT detected!\n");

    recvMsg = Comm_LastMsg();
    if(memcmp(&msg, &recvMsg, sizeof(InputMsg)) == 0)
        printf("Messages are equal!\n");
    else
        printf("!!! Messages are NOT equal!\n");

    if(Comm_NewMsg() == 1)
        printf("!!! New message detected AGAIN!\n");

    /*
     * Repeating message. Still works?
     */

    printf(" ---- TEST 2 ----\n");

    Comm_Process(msgBuf, strlen(msgStr) + 2);

    recvMsg.leftPwm = 0; recvMsg.rightPwm = 0;
    if(Comm_NewMsg() == 1)
        printf("New message detected!\n");
    else
        printf("!!! New message NOT detected!\n");

    recvMsg = Comm_LastMsg();
    if(memcmp(&msg, &recvMsg, sizeof(InputMsg)) == 0)
        printf("Messages are equal!\n");
    else
        printf("!!! Messages are NOT equal!\n");

    if(Comm_NewMsg() == 1)
        printf("!!! New message detected AGAIN!\n");

    /*
     * First sending trash, then correct message. Then trash again.
     */
    printf(" ---- TEST 3 ----\n");
    char trash[] = "kdfjsfsjh6576576567";

    Comm_Process(trash, strlen(trash));
    Comm_Process(msgBuf, strlen(msgStr) + 2);
    Comm_Process(trash, strlen(trash));

    recvMsg.leftPwm = 0; recvMsg.rightPwm = 0;
    if(Comm_NewMsg() == 1)
        printf("New message detected!\n");
    else
        printf("!!! New message NOT detected!\n");

    recvMsg = Comm_LastMsg();
    if(memcmp(&msg, &recvMsg, sizeof(InputMsg)) == 0)
        printf("Messages are equal!\n");
    else
        printf("!!! Messages are NOT equal!\n");

    if(Comm_NewMsg() == 1)
        printf("!!! New message detected AGAIN!\n");

    /*
     * Sending message piece-by-piece. Last part with trash.
     */
    printf(" ---- TEST 4 ----\n");

    Comm_Process(msgBuf, 8);

    char* lastPieceWithTrash = malloc( strlen(msgStr) + 2 - 8 + strlen(trash));
    memcpy(lastPieceWithTrash, msgBuf + 8, strlen(msgStr) + 2 - 8);
    memcpy(lastPieceWithTrash + strlen(msgStr) + 2 - 8, trash, strlen(trash));

    Comm_Process(lastPieceWithTrash, strlen(trash));

    recvMsg.leftPwm = 0; recvMsg.rightPwm = 0;
    if(Comm_NewMsg() == 1)
        printf("New message detected!\n");
    else
        printf("!!! New message NOT detected!\n");

    recvMsg = Comm_LastMsg();
    if(memcmp(&msg, &recvMsg, sizeof(InputMsg)) == 0)
        printf("Messages are equal!\n");
    else
        printf("!!! Messages are NOT equal!\n");

    if(Comm_NewMsg() == 1)
        printf("!!! New message detected AGAIN!\n");

    /*
     * Sending half of correct message, trash, then other half.
     */
    printf(" ---- TEST 5 ----\n");
    Comm_Process(msgBuf, 5);
    Comm_Process(trash, strlen(trash));
    Comm_Process(msgBuf + 5, strlen(msgStr) + 2 - 5);

    recvMsg.leftPwm = 0; recvMsg.rightPwm = 0;
    if(Comm_NewMsg() == 0)
        printf("New message not detected!\n");
    else
        printf("!!! New message detected!\n");

    /*
     * Repeating message. Still works?
     */

    printf(" ---- TEST 6 ----\n");

    Comm_Process(msgBuf, strlen(msgStr) + 2);

    recvMsg.leftPwm = 0; recvMsg.rightPwm = 0;
    if(Comm_NewMsg() == 1)
        printf("New message detected!\n");
    else
        printf("!!! New message NOT detected!\n");

    recvMsg = Comm_LastMsg();
    if(memcmp(&msg, &recvMsg, sizeof(InputMsg)) == 0)
        printf("Messages are equal!\n");
    else
        printf("!!! Messages are NOT equal!\n");

    if(Comm_NewMsg() == 1)
        printf("!!! New message detected AGAIN!\n");
    /*
     * Send message char-by-char
     */

    printf(" ---- TEST 7 ----\n");

    for(i = 0 ; i < strlen(msgStr) + 2 ; i++)
    {
        c = msgBuf[i];
        Comm_Process(&c, 1);
    }

    recvMsg.leftPwm = 0; recvMsg.rightPwm = 0;
    if(Comm_NewMsg() == 1)
        printf("New message detected!\n");
    else
        printf("!!! New message NOT detected!\n");

    recvMsg = Comm_LastMsg();
    if(memcmp(&msg, &recvMsg, sizeof(InputMsg)) == 0)
        printf("Messages are equal!\n");
    else
        printf("!!! Messages are NOT equal!\n");

    if(Comm_NewMsg() == 1)
        printf("!!! New message detected AGAIN!\n");

    char* recvMsgStr = msgAsStr(recvMsg);
    printf("%s\n", recvMsgStr);
}

char* msgAsStr(InputMsg msg)
{
    char* msgStr = malloc(2*sizeof(msg) + 1);
    int i;
    for(i = 0 ; i < 2*sizeof(InputMsg) ; i += 2)
    {
        sprintf(msgStr + i, "%02x", (unsigned)(unsigned char) *(((unsigned char*)&msg)+(i/2)) );
    }
    return msgStr;
}
