/*
* Example Controller Demo.
* Reads a source controller, translates and writes to VirtualPad.
* Logs the VirtualPad state to Serial.
* Uncomment the desired source controller to use.
* Nintendo controllers depend on https://github.com/GitMoDu/JoybusOverUart
*/


// Uncomment controller to use.
#define DIRECT_CONTROLLER
//#define USE_N64_CONTROLLER
//#define USE_GAMECUBE_CONTROLLER

#define SERIAL_BAUD_RATE 115200

#include <VirtualPadSources.h>

static constexpr uint32_t UpdatePeriodMillis = 20;
static constexpr uint32_t LogPeriodMillis = 50;
static constexpr uint32_t LogLongPeriodMillis = 1000;
static constexpr uint32_t TimeOutPeriodMillis = 100;
#if defined(USE_N64_CONTROLLER) || defined(USE_GAMECUBE_CONTROLLER)
static constexpr HardwareSerial* JoyBusSerial = &Serial3;
#endif

#if defined(DIRECT_CONTROLLER)
DirectInputVirtualPadWriter Pad(A0, A1, 3, 4);
#elif defined(USE_N64_CONTROLLER)
#include <Nintendo64OverUart.h>
Nintendo64OverUart JoyBusReader(JoyBusSerial);
Nintendo64Controller::data_t ControllerData{};
Nintendo64VirtualPadWriter<> Pad{};
bool Polling = false;
#elif defined(USE_GAMECUBE_CONTROLLER)
#include <GameCubeOverUart.h>
GameCubeOverUart JoyBusReader(JoyBusSerial);
GameCubeController::data_t ControllerData{};
GameCubeVirtualPadWriter<> Pad{};
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
	if (!Pad.Setup())
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
		Pad.Step();
}
#elif defined(USE_N64_CONTROLLER)
	if (timestamp - LastUpdate >= UpdatePeriodMillis)
	{
		JoyBusReader.Poll();
		delay(1);
		if (JoyBusReader.ReadControllerData(ControllerData))
		{
			Pad.MapControllerData(ControllerData);
			LastUpdate = timestamp;
			if (!Pad.Connected())
			{
				Pad.SetConnected(true);
			}
		}
		else if (timestamp - LastUpdate >= TimeOutPeriodMillis)
		{
			if (Pad.Connected())
			{
				LastLog = millis() - LogLongPeriodMillis;
			}
			Pad.Clear();
		}
	}
#elif defined(USE_GAMECUBE_CONTROLLER)
	if (Pad.Connected())
	{
		if (timestamp - LastUpdate >= UpdatePeriodMillis)
		{
			JoyBusReader.Poll();
			delay(1);
			if (JoyBusReader.ReadControllerData(ControllerData))
			{
				Pad.MapControllerData(ControllerData);
				LastUpdate = timestamp;
			}
			else if (timestamp - LastUpdate >= TimeOutPeriodMillis)
			{
				Pad.Clear();
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
				Pad.SetConnected(true);
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
	if (Pad.Connected())
	{
		if (millis() - LastLog >= LogPeriodMillis)
		{
			LastLog = millis();

			Serial.println();
			Serial.print(F("Navigation\t"));
			if (Pad.GetHome())
			{
				Serial.print(F("Home "));
			}
			if (Pad.GetAccept())
			{
				Serial.print(F("Accept "));
			}
			if (Pad.GetReject())
			{
				Serial.print(F("Reject "));
			}
			Serial.println();

			Serial.print(F("DPad\t"));
			switch (Pad.DPad())
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
			if (Pad.A())
			{
				Serial.print(F("A  "));
			}
			else
			{
				Serial.print(F("__ "));
			}

			if (Pad.B())
			{
				Serial.print(F("B  "));
			}
			else
			{
				Serial.print(F("__ "));
			}

			if (Pad.X())
			{
				Serial.print(F("X  "));
			}
			else
			{
				Serial.print(F("__ "));
			}

			if (Pad.Y())
			{
				Serial.print(F("Y  "));
			}
			else
			{
				Serial.print(F("__ "));
			}

			if (Pad.FeatureL1())
			{
				if (Pad.L1())
				{
					Serial.print(F("L1 "));
				}
				else
				{
					Serial.print(F("__ "));
				}
			}

			if (Pad.FeatureR1())
			{
				if (Pad.R1())
				{
					Serial.print(F("R1 "));
				}
				else
				{
					Serial.print(F("__ "));
				}
			}

			if (Pad.FeatureL3())
			{
				if (Pad.L3())
				{
					Serial.print(F("L3 "));
				}
				else
				{
					Serial.print(F("__ "));
				}
			}

			if (Pad.FeatureR3())
			{
				if (Pad.R3())
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
			if (Pad.Start())
			{
				Serial.print(F("Start "));
			}
			if (Pad.Select())
			{
				Serial.print(F("Select "));
			}
			if (Pad.Home())
			{
				Serial.print(F("Home "));
			}
			if (Pad.Share())
			{
				Serial.print(F("Share "));
			}
			Serial.println();

			Serial.print(F("Joy1(x,y)\t("));
			Serial.print(Pad.Joy1X());
			Serial.print(',');
			Serial.print(Pad.Joy1Y());
			Serial.println(')');

			if (Pad.FeatureJoy2())
			{
				Serial.print(F("Joy2(x,y)\t("));
				Serial.print(Pad.Joy2X());
				Serial.print(',');
				Serial.print(Pad.Joy2Y());
				Serial.println(')');
			}

			if (Pad.FeatureL2())
			{
				Serial.print(F("L2\t"));
				Serial.println(Pad.L2());
			}

			if (Pad.FeatureR2())
			{
				Serial.print(F("R2\t"));
				Serial.println(Pad.R2());
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
