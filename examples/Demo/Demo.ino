/*
* Example Demo of IInputController.
* Uncomment the desired controller to use.
* Logs the abstracted IInputController to Serial.
* Nintendo controllers depend on https://github.com/GitMoDu/JoybusOverUart
*/


// Uncomment controller to use.
//#define DIRECT_CONTROLLER
#define USE_N64_CONTROLLER
//#define USE_GAMECUBE_CONTROLLER

#define SERIAL_BAUD_RATE 115200

#include <InputControllers.h>

static constexpr uint32_t UpdatePeriodMillis = 20;
static constexpr uint32_t LogPeriodMillis = 50;
static constexpr uint32_t LogLongPeriodMillis = 1000;
static constexpr uint32_t TimeOutPeriodMillis = 100;
static constexpr HardwareSerial* JoyBusSerial = &Serial3;


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
uint32_t LastLog = 0;


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

	LastLog = millis() - LogPeriodMillis;
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
			if (InputController.Connected())
			{
				LastLog = millis() - LogLongPeriodMillis;
			}
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

	LogIInputControllerState();
}

void LogIInputControllerState()
{
	if (InputController.Connected())
	{
		if (millis() - LastLog >= LogPeriodMillis)
		{
			LastLog = millis();

			Serial.println();
			Serial.print(F("Navigation\t"));
			if (InputController.GetHome())
			{
				Serial.print(F("Home "));
			}
			if (InputController.GetAccept())
			{
				Serial.print(F("Accept "));
			}
			if (InputController.GetReject())
			{
				Serial.print(F("Reject "));
			}
			Serial.println();

			Serial.print(F("DPad\t"));
			switch (InputController.DPad())
			{
			case DPadEnum::Up:
				Serial.println(F("Up"));
				break;
			case DPadEnum::Down:
				Serial.println(F("Down"));
				break;
			case DPadEnum::Left:
				Serial.println(F("Left"));
				break;
			case DPadEnum::Right:
				Serial.println(F("Right"));
				break;
			case DPadEnum::None:
				Serial.println(F("Center"));
			default:
				break;
			}

			Serial.print(F("Main Buttons\t"));
			if (InputController.A())
			{
				Serial.print(F("A  "));
			}
			else
			{
				Serial.print(F("__ "));
			}

			if (InputController.B())
			{
				Serial.print(F("B  "));
			}
			else
			{
				Serial.print(F("__ "));
			}

			if (InputController.X())
			{
				Serial.print(F("X  "));
			}
			else
			{
				Serial.print(F("__ "));
			}

			if (InputController.Y())
			{
				Serial.print(F("Y  "));
			}
			else
			{
				Serial.print(F("__ "));
			}

			if (InputController.FeatureL1())
			{
				if (InputController.L1())
				{
					Serial.print(F("L1 "));
				}
				else
				{
					Serial.print(F("__ "));
				}
			}

			if (InputController.FeatureR1())
			{
				if (InputController.R1())
				{
					Serial.print(F("R1 "));
				}
				else
				{
					Serial.print(F("__ "));
				}
			}

			if (InputController.FeatureL3())
			{
				if (InputController.L3())
				{
					Serial.print(F("L3 "));
				}
				else
				{
					Serial.print(F("__ "));
				}
			}

			if (InputController.FeatureR3())
			{
				if (InputController.R3())
				{
					Serial.print(F("R3 "));
				}
				else
				{
					Serial.print(F("__ "));
				}
			}
			Serial.println();

			Serial.print(F("Menu Buttons\t"));
			if (InputController.Start())
			{
				Serial.print(F("Start "));
			}
			if (InputController.Select())
			{
				Serial.print(F("Select "));
			}
			if (InputController.Home())
			{
				Serial.print(F("Home "));
			}
			if (InputController.Share())
			{
				Serial.print(F("Share "));
			}
			Serial.println();

			Serial.print(F("Joy1(x,y)\t("));
			Serial.print(InputController.Joy1X());
			Serial.print(',');
			Serial.print(InputController.Joy1Y());
			Serial.println(')');

			if (InputController.FeatureJoy2())
			{
				Serial.print(F("Joy2(x,y)\t("));
				Serial.print(InputController.Joy2X());
				Serial.print(',');
				Serial.print(InputController.Joy2Y());
				Serial.println(')');
			}

			if (InputController.FeatureL2())
			{
				Serial.print(F("L2\t"));
				Serial.println(InputController.L2());
			}

			if (InputController.FeatureR2())
			{
				Serial.print(F("R2\t"));
				Serial.println(InputController.R2());
			}
		}
	}
	else
	{
		if (millis() - LastLog >= LogLongPeriodMillis)
		{
			LastLog = millis();
			Serial.println(F("No controller"));
		}
	}
}
