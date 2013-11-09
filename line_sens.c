#include "line_sens.h"

#include <stdlib.h>
#include <math.h>

static const int 		SensorCount = 10;
static const float 	SensorPoss[10] = {
	-4.18, -2.55, -1.85, -1.15, -0.45,
	0.45, 1.15, 1.85, 2.55, 4.18
};

static float				prevFeedback;

/**
	* Filters sensor readings, gives feedback and finish line indicator.
	* If only one line segment detected, feedback based on that line.
	* If more than one line segment detected, gives feedback closest to previous fb.
	* If no line segments detected, gives previous feedback.
	* If three segments detected, then finishLineDetected = 1.
	*/
LineSenseOut 	LS_Feedback(uint32_t sensReadings)
{
	LineSenseOut 	out;

	int 			i;
	float			smallestFbDiff = 10;		// Assuming that always fabs(tempFb - prevFeedback) < 10
	float			tempFb;
	uint32_t 	filtReading;

    out.finishLineDetected = 0;
	out.numOfLinesDetected = LS_NumOfLines(sensReadings);

	switch(out.numOfLinesDetected)
	{
		case 0:
			out.feedback 						= prevFeedback;
			out.finishLineDetected 	= 0;
			out.usedLinePatt 				= sensReadings;
			break;

		case 1:
			out.feedback 						= LS_BitsToFeedback(sensReadings);
			out.finishLineDetected 	= 0;
			out.usedLinePatt 				= sensReadings;
			break;

		default:

			for(i = 0 ; i < out.numOfLinesDetected ; i++)
			{
				filtReading = LS_FilteredReading(sensReadings, i);
				tempFb = LS_BitsToFeedback(filtReading);

				if(fabs(tempFb - prevFeedback) < smallestFbDiff)
				{
					out.feedback 			= tempFb;
					out.usedLinePatt 	= filtReading;
					smallestFbDiff 		= fabs(out.feedback - prevFeedback);
				}
			}

			if(out.numOfLinesDetected == 3)
                out.finishLineDetected = 1;
			break;
	}

	prevFeedback = out.feedback;
	return out;
}

/**
	* Each lineDetected bit tells whether sensor is over line.
	* LSB is left-most sensor, 10. bit is right-most sensor
	*/
float LS_BitsToFeedback(uint32_t sensReadings)
{
	float fb;
	int 	i;
	float sensSum 		= 0;
	int 	iSensCount 	= 0;

	if(sensReadings == 0)
        return 0;

	for(i = 0 ; i < SensorCount ; i++)
	{
		if(sensReadings & (1<<i))
		{
			sensSum += SensorPoss[i];
			iSensCount++;
		}
	}

	fb = sensSum / iSensCount;

	return fb;
}

/**
	* Number of detected lines.
	* If lineDetected is 	00001 10000, then returns 1
	* 										00000 00000, 							0
	* 										10000 10001,							3
	*/
int 	LS_NumOfLines(uint32_t sensReadings)
{
	int 		numLines 			= 0;
	uint8_t lineDetected 	= 0;
	int 		i;

	for(i = 0 ; i < SensorCount ; i++)
	{
		if(sensReadings & (1<<i))
		{
			if(!lineDetected)
			{
				lineDetected = 1;
				numLines++;
			}
		}
		else
		{
			lineDetected = 0;
		}
	}

	return numLines;
}

/**
	* sensReadings with other lines/islands removed
	* If sensReadings is 	00010 10000 and islandIdx 1, then returns 00000 10000
	* 										00010 10000								0								00010 00000
	*/
uint32_t LS_FilteredReading(uint32_t sensReadings, int islandIdx)
{
	int 			i;
	int 			lineIdx = -1;
	uint8_t		lineDetected = 0;
	uint32_t	filteredReadings = 0;

	for(i = 0 ; i < SensorCount ; i++)
	{
		if(sensReadings & (1<<i))
		{
			if(!lineDetected)
			{
				lineDetected = 1;
				lineIdx++;
			}
			if(lineIdx == islandIdx)
				filteredReadings |= (1<<i);
		}
		else
		{
			lineDetected = 0;
		}
	}

	return filteredReadings;
}
