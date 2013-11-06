#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "line_sens.h"

void test_line_sens(void)
{
    uint32_t line = 0b0000000000;
    int i;
    float fb;

    assert(LS_NumOfLines(line) == 0);
    for(i = 0 ; i < 10 ; i++)
        assert(LS_NumOfLines(1<<i) == 1);

    assert(LS_NumOfLines(0b1111111111) == 1);
    assert(LS_NumOfLines(0b1111011111) == 2);
    assert(LS_NumOfLines(0b1000010001) == 3);
    assert(LS_NumOfLines(0b1011110001) == 3);
    assert(LS_NumOfLines(0b1000000001) == 2);

    printf("LS_NumOfLines OK\n");

    assert(LS_FilteredReading(0b1000010000, 1) == 0b1000000000);
    assert(LS_FilteredReading(0b1000010000, 0) == 0b0000010000);
    assert(LS_FilteredReading(0b0000000000, 0) == 0b0000000000);

    assert(LS_FilteredReading(0b1000111000, 0) == 0b0000111000);
    assert(LS_FilteredReading(0b1000111001, 1) == 0b0000111000);

    printf("LS_FilteredReading OK\n");

    for(line = 0 ; line < (1<<10) ; line++)
    {
        fb = LS_BitsToFeedback(line);
        assert(-4.19 < fb && fb < 4.19);
    }

    printf("LS_BitsToFeedback OK\n");
}
