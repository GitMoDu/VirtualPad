/*
* Example Actions Demo.
* Reads a source controller and translates to abstract IInputController.
* Logs the IInputController actions to Serial.
* Uncomment the desired source controller to use.
* Nintendo controllers depend on https://github.com/GitMoDu/JoybusOverUart
*/


// Uncomment controller to use.
#define DIRECT_CONTROLLER
//#define USE_N64_CONTROLLER
//#define USE_GAMECUBE_CONTROLLER

#define SERIAL_BAUD_RATE 115200

#include <InputControllers.h>
#include <InputTaskControllers.h>
#include "Actions.h"

static constexpr uint32_t UpdatePeriodMillis = 5;
static constexpr uint32_t TimeOutPeriodMillis = 100;

#if defined(USE_N64_CONTROLLER) || defined(USE_GAMECUBE_CONTROLLER)
static constexpr HardwareSerial* JoyBusSerial = &Serial3;
#endif

#if defined(DIRECT_CONTROLLER)
DirectInputController InputController(A0, A1, 3, 4);
#elif defined(USE_N64_CONTROLLER)
#include <Nintendo64OverUart.h>
Nintendo64OverUart JoyBusReader(JoyBusSerial);
Nintendo64Controller::data_t ControllerData{};
Nintendo64InputController<> InputController{};
bool Polling = false;
#elif defined(USE_GAMECUBE_CONTROLLER)
#include <GameCubeOverUart.h>
GameCubeOverUart JoyBusReader(JoyBusSerial);
GameCubeController::data_t ControllerData{};
GameCubeInputController<> InputController{};
#endif

uint32_t LastUpdate = 0;

Actions ControllerActions{};


void Halt()
{
#ifdef DEBUG
	Serial.println();
	Serial.println("Critical Error");
	delay(1000);
#endif	
	while (1);;
}

void setup()
{
	Serial.begin(SERIAL_BAUD_RATE);
	while (!Serial)
		;
	delay(1000);
	Serial.print(F("Controller Demo..."));

#if defined(DIRECT_CONTROLLER)
	if (!InputController.Setup())
	{
		Halt();
	}
#elif defined(USE_GAMECUBE_CONTROLLER) || defined(USE_N64_CONTROLLER)
	JoyBusReader.Start();
#endif

	Serial.println(F("Starting!"));

	LastUpdate = millis();
}

void loop()
{
	uint32_t timestamp = millis();


#if defined(DIRECT_CONTROLLER)
	if (timestamp - LastUpdate >= UpdatePeriodMillis)
	{
		LastUpdate = timestamp;
		InputController.Step();
	}
#elif defined(USE_N64_CONTROLLER)
	if (timestamp - LastUpdate >= UpdatePeriodMillis)
	{
		JoyBusReader.Poll();
		delay(1);
		if (JoyBusReader.ReadControllerData(ControllerData))
		{
			InputController.MapControllerData(ControllerData);
			LastUpdate = timestamp;
			if (!InputController.Connected())
			{
				InputController.State.Connected = true;
			}
		}
		else if (timestamp - LastUpdate >= TimeOutPeriodMillis)
		{
			InputController.Clear();
		}
	}
#elif defined(USE_GAMECUBE_CONTROLLER)
	if (InputController.Connected())
	{
		if (timestamp - LastUpdate >= UpdatePeriodMillis)
		{
			JoyBusReader.Poll();
			delay(1);
			if (JoyBusReader.ReadControllerData(ControllerData))
			{
				InputController.MapControllerData(ControllerData);
				LastUpdate = timestamp;
			}
			else if (timestamp - LastUpdate >= TimeOutPeriodMillis)
			{
				InputController.Clear();
			}
		}
	}
	else
	{
		if (timestamp - LastUpdate >= UpdatePeriodMillis)
		{
			JoyBusReader.WakeUpPoll();
			delay(1);
			if (JoyBusReader.ReadControllerData(ControllerData))
			{
				InputController.State.Connected = true;
				LastUpdate = timestamp - UpdatePeriodMillis;
			}
			else
			{
				LastUpdate = timestamp;
			}
		}
	}
#endif


	if (InputController.Connected())
	{
		ControllerActions.Parse(&InputController);

		if (ControllerActions.HasDownAction())
		{
			LogActions();
		}
	}
	else
	{
		ControllerActions.Clear();
	}
}

void LogActions()
{
	if (ControllerActions.A())
		Serial.print(F("A\t"));
	if (ControllerActions.B())
		Serial.print(F("B\t"));
	if (ControllerActions.X())
		Serial.print(F("X\t"));
	if (ControllerActions.Y())
		Serial.print(F("Y\t"));
	if (ControllerActions.L1())
		Serial.print(F("L1\t"));
	if (ControllerActions.R1())
		Serial.print(F("R1\t"));
	if (ControllerActions.L3())
		Serial.print(F("L3\t"));
	if (ControllerActions.R3())
		Serial.print(F("R3\t"));

	if (ControllerActions.Start())
		Serial.print(F("Start\t"));
	if (ControllerActions.Select())
		Serial.print(F("Select\t"));
	if (ControllerActions.Home())
		Serial.print(F("Home\t"));
	if (ControllerActions.Share())
		Serial.print(F("Share "));

	if (ControllerActions.L2())
		Serial.print(F("L2\t"));
	if (ControllerActions.R2())
		Serial.print(F("R2\t"));

	if (ControllerActions.DPad() != DPadEnum::None)
	{
		Serial.print(F("DPad::"));
		PrintDPadActions(ControllerActions.DPad());
		Serial.print('\t');
	}

	if (ControllerActions.Joy1() != DPadEnum::None)
	{
		Serial.print(F("Joy1::"));
		PrintDPadActions(ControllerActions.Joy1());
		Serial.print('\t');
	}

	if (ControllerActions.Joy2() != DPadEnum::None)
	{
		Serial.print(F("Joy2::"));
		PrintDPadActions(ControllerActions.Joy2());
		Serial.print('\t');
	}

	Serial.println();
}

void PrintDPadActions(const DPadEnum dpad)
{
	switch (dpad)
	{
	case DPadEnum::Up:
		Serial.print(F("Up"));
		break;
	case DPadEnum::UpLeft:
		Serial.print(F("UpLeft"));
		break;
	case DPadEnum::UpRight:
		Serial.print(F("UpRight"));
		break;
	case DPadEnum::Down:
		Serial.print(F("Down"));
		break;
	case DPadEnum::DownLeft:
		Serial.print(F("DownLeft"));
		break;
	case DPadEnum::DownRight:
		Serial.print(F("DownRight"));
		break;
	case DPadEnum::Left:
		Serial.print(F("Left"));
		break;
	case DPadEnum::Right:
		Serial.print(F("Right"));
		break;
	case DPadEnum::None:
	default:
		break;
	}
}
