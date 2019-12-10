/*
	Name:       Calibrator.ino
	Created:	2019-09-01 22:16:15
	Author:     SPACECOMPUTER\delta
*/

#define DEBUG_LOG
//#define WAIT_FOR_LOGGER

#define USE_SUPPORT_SCREEN



#define SERIAL_BAUD_RATE 500000



#define _TASK_OO_CALLBACKS
#define _TASK_SLEEP_ON_IDLE_RUN // Enable 1 ms SLEEP_IDLE powerdowns between tasks if no callback methods were invoked during the pass.


#include <TaskScheduler.h>


#include <GamecubeInputController.h>
#include <RumblerDAC.h>
#include <RumbleAnimatorTask.h>

#ifdef USE_SUPPORT_SCREEN
#include "ProcessSupportScreen.h"
#endif

#include "ControllerRumbleTester.h"


///Process scheduler.
Scheduler SchedulerBase;
///


class GamecubeControllerCalibration
{
public:
	static const uint8_t	GamecubeAxisRange = 255;
	static const uint8_t	GamecubeAxisMin = 0;
	static const uint8_t	GamecubeAxisMax = GamecubeAxisRange - 1;

public:
	//Joystick.
	static const uint8_t	JoyXMin = 25;
	static const uint8_t	JoyXMax = 233;
	static const int8_t		JoyXOffset = 3;

	static const uint8_t	JoyYMin = 2;
	static const uint8_t	JoyYMax = 42;
	static const int8_t		JoyYOffset = -12;

	static const uint8_t	JoyDeadZoneRadius = 3;
	static const uint8_t	JoyFilterSaturation = 140;

	//Joystick C.
	static const uint8_t	JoyCXMin = 33;
	static const uint8_t	JoyCXMax = 215;
	static const int8_t		JoyCXOffset = -2;

	static const uint8_t	JoyCYMin = 35;
	static const uint8_t	JoyCYMax = 223;
	static const int8_t		JoyCYOffset = 6;

	static const uint8_t	JoyCDeadZoneRadius = 2;
	static const uint8_t	JoyCFilterSaturation = 140;

	//Trigers.
	static const uint8_t	TriggerLMin = 30;
	static const uint8_t	TriggerLMax = 200;
	static const uint8_t	TriggerLDeadZone = 41;
	static const uint8_t	TriggerLFilterSaturation = 170;

	static const uint8_t	TriggerRMin = 38;
	static const uint8_t	TriggerRMax = 200;
	static const uint8_t	TriggerRDeadZone = 46;
	static const uint8_t	TriggerRFilterSaturation = 170;
};

const uint32_t GAMECUBE_CONTROLLER_PIN = PA0;
const uint32_t ControllerSamplePeriodMillis = 5;
const uint32_t ControllerUpdatePeriodMillis = 3;


GamecubeControllerCarInputControllerTask<GamecubeControllerCalibration, GAMECUBE_CONTROLLER_PIN, ControllerSamplePeriodMillis, ControllerUpdatePeriodMillis>  Controller(&SchedulerBase);

ControllerRumbleTester RumbleTester(&SchedulerBase);

///Support screen
#ifdef USE_SUPPORT_SCREEN
ProcessSupportScreen<GamecubeControllerCalibration> SupportScreen(&SchedulerBase, &Controller);
#endif
///


void Halt()
{
#ifdef DEBUG_LOG
	Serial.println("Critical Error");
	delay(1000);
#endif	
	while (1);;
}

void setup()
{
#ifdef DEBUG_LOG
	Serial.begin(SERIAL_BAUD_RATE);
#ifdef WAIT_FOR_LOGGER
	while (!Serial)
		;
#else
	delay(1000);
#endif
	delay(1000);
	Serial.println(F("Controller Calibrator"));
#endif

	//RumbleTester.Setup(Controller.GetDriver());

	if (Controller.Setup())
	{
#ifdef DEBUG_LOG
		Serial.println(F("Gamecube Setup OK"));
#endif
	}


#ifdef USE_SUPPORT_SCREEN
	if (!SupportScreen.Setup())
	{
#ifdef DEBUG_LOG
		Serial.println(F("SupportScreen Setup Failed."));
#endif
		Halt();
	}
#endif
}

void loop()
{
	SchedulerBase.execute();
}
