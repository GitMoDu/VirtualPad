#define DEBUG_LOG
//#define WAIT_FOR_LOGGER

#define USE_SUPPORT_SCREEN
#define DEBUG_IINPUT_CONTROLLER



#define SERIAL_BAUD_RATE 115200



#define _TASK_OO_CALLBACKS
#define _TASK_SLEEP_ON_IDLE_RUN // Enable 1 ms SLEEP_IDLE powerdowns between tasks if no callback methods were invoked during the pass.
#include <TaskScheduler.h>

#include <NintendoInputControllersInclude.h>


#ifdef USE_SUPPORT_SCREEN
#include "ProcessSupportScreen.h"
#endif


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
	static const uint8_t	JoyXMin = 26;
	static const uint8_t	JoyXMax = 231;
	static const int8_t		JoyXOffset = -1;

	static const uint8_t	JoyYMin = 30;
	static const uint8_t	JoyYMax = 237;
	static const int8_t		JoyYOffset = 0;

	static const uint8_t	JoyDeadZoneRadius = 9;

	//Joystick C.
	static const uint8_t	JoyCXMin = 25;
	static const uint8_t	JoyCXMax = 225;
	static const int8_t		JoyCXOffset = 0;

	static const uint8_t	JoyCYMin = 24;
	static const uint8_t	JoyCYMax = 233;
	static const int8_t		JoyCYOffset = 0;

	static const uint8_t	JoyCDeadZoneRadius = 9;

	//Trigers.
	static const uint8_t	TriggerLMin = 30;
	static const uint8_t	TriggerLMax = 180;
	static const uint8_t	TriggerLDeadZone = 41;

	static const uint8_t	TriggerRMin = 38;
	static const uint8_t	TriggerRMax = 180;
	static const uint8_t	TriggerRDeadZone = 46;
};

class N64ControllerCalibration
{
public:
	//Joystick.
	static const int8_t		JoyXMin = -75;
	static const int8_t		JoyXMax = 75;
	static const int8_t		JoyXOffset = 0;

	static const uint8_t	JoyYMin = -78;
	static const uint8_t	JoyYMax = 78;
	static const int8_t		JoyYOffset = 0;

	static const uint8_t	JoyDeadZoneRadius = 1;
};

const uint32_t CONTROLLER_PIN = PA0;
const uint32_t ControllerUpdatePeriodMillis = 4;


GamecubeControllerTask<GamecubeControllerCalibration, CONTROLLER_PIN, ControllerUpdatePeriodMillis>  Controller(&SchedulerBase);
//N64ControllerTask<N64ControllerCalibration, CONTROLLER_PIN, ControllerUpdatePeriodMillis>  Controller(&SchedulerBase);


///Support screen
#ifdef USE_SUPPORT_SCREEN
ProcessSupportScreen<2, GamecubeControllerCalibration> SupportScreen(&SchedulerBase, &Controller);
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

#ifdef USE_SUPPORT_SCREEN
	if (!SupportScreen.Setup())
	{
#ifdef DEBUG_LOG
		Serial.println(F("SupportScreen Setup Failed."));
#endif
		//Halt();
	}
#ifdef DEBUG_LOG
	else
	{
		SupportScreen.Debug(&Serial);
	}
#endif
#endif

	Controller.StartController();

#ifdef DEBUG_LOG
	Serial.println(F("Calibrator Setup OK"));
#endif
}

void loop()
{
	SchedulerBase.execute();
}
