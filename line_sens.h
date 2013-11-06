#include <stdint.h>

typedef struct
{
	float			feedback;
	int				numOfLinesDetected;
	uint32_t	usedLinePatt;
	uint8_t 	finishLineDetected;
} LineSenseOut;

/**
	* Filters sensor readings, gives feedback and finish line indicator.
	* If only one line segment detected, feedback based on that line.
	* If more than one line segment detected, gives feedback closest to previous fb.
	* If no line segments detected, gives previous feedback.
	* If three segments detected, then finishLineDetected = 1.
	*/
LineSenseOut 	LS_Feedback(uint32_t sensReadings);

float LS_BitsToFeedback(uint32_t sensReadings);

/**
	* Number of detected lines.
	* If sensReadings is 	00001 10000, then returns 1
	* 										00000 00000, 							0
	* 										10000 10001,							3
	*/
int 	LS_NumOfLines(uint32_t sensReadings);

/**
	* sensReadings with other lines/islands removed
	* If sensReadings is 	00010 10000 and islandIdx 1, then returns 00000 10000
	* 										00010 10000								0								00010 00000
	*/	
uint32_t LS_FilteredReading(uint32_t sensReadings, int islandIdx);
