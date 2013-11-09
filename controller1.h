typedef struct
{
	float basePwm; 					// This is PWM for both motors when going straight. In turns, will subtract from this.
	float P, I, D;
	float maxPidOut;				// Output of just PID will be maxPidOut >= |u|
} Controller1Settings;

typedef struct
{
	Controller1Settings settings;
	
	float iSum;
	float prevErr;
} Controller1;

typedef struct
{
	float uk, ui, ud;
	float u;
	float pwmLeft, pwmRight;
} Controller1Out;

void 						Controller1_Reset(void);
Controller1Out 	Controller1_Run(float fb);
