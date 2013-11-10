#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "line_sens.h"

uint8_t near(float a, float b, float tol)
{
    return fabs(a - b) <= tol;
}

void test_line_sens(void)
{
    uint32_t line = 0b0000000000;
    int i;
    float fb;
    LineSenseOut lsOut;

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

    for(line = 0 ; line < (1<<10) ; line++)
    {
        lsOut = LS_Feedback(line);
        assert(-4.19 < lsOut.feedback && lsOut.feedback < 4.19);
    }

    lsOut = LS_Feedback(0);
    assert(near(lsOut.feedback, 0, 1e-6));

    lsOut = LS_Feedback(0b1000110001);
    assert(near(lsOut.feedback, 0, 1e-6));
    assert(lsOut.finishLineDetected);

    lsOut = LS_Feedback(0b1000110000);
    assert(near(lsOut.feedback, 0, 1e-6));
    assert(lsOut.finishLineDetected == 0);

    lsOut = LS_Feedback(0b1000000000);
    assert(near(lsOut.feedback, 4.18, 1e-6));

    lsOut = LS_Feedback(0b0000000001);
    assert(near(lsOut.feedback, -4.18, 1e-6));

    lsOut = LS_Feedback(0b0000000011);
    assert(near(lsOut.feedback, (-4.18 -2.55)/2, 1e-6));

    lsOut = LS_Feedback(0b1111111111);
    assert(near(lsOut.feedback, 0, 1e-6));

    lsOut = LS_Feedback(0b0000000111);
    assert(near(lsOut.feedback, -2.5, 1));

    printf("LS_BitsToFeedback OK\n");
}
