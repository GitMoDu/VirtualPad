/*
* Example Controller Demo.
* Reads a source controller, translates and writes to VirtualPad.
* Logs the VirtualPad state and actions to Serial.
* 
* Uncomment the desired source controller to use.
* Nintendo controllers depend on https://github.com/GitMoDu/JoybusOverUart
*/


// Uncomment controller to use.
#define DIRECT_CONTROLLER
//#define USE_N64_CONTROLLER
//#define USE_GAMECUBE_CONTROLLER
//#define USE_MEGA_DRIVE_CONTROLLER

#define SERIAL_BAUD_RATE 115200

#include <VirtualPadSources.h>
#include "TestActions.h"

static constexpr uint32_t UpdatePeriodMillis = 5;
static constexpr uint32_t LogPeriodMillis = 100;
static constexpr uint32_t LogLongPeriodMillis = 1000;
static constexpr uint32_t TimeOutPeriodMillis = 100;
#if defined(USE_N64_CONTROLLER) || defined(USE_GAMECUBE_CONTROLLER)
static constexpr HardwareSerial* JoyBusSerial = &Serial2;
#endif

#if defined(DIRECT_CONTROLLER)
//DirectInputVirtualPadWriter Pad(UINT8_MAX, UINT8_MAX, 4, 3);
DirectInputVirtualPadWriter Pad(A0, A1, 4, 3, 5, 2, 8, 6, 7);
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
#elif defined(USE_MEGA_DRIVE_CONTROLLER)
enum class MegaDrivePin : uint8_t { Up = 2, Down = 3, InfoLeft = 4, InfoRight = 5, AB = 6, StartC = 7, Select = 8 };
MegaDriveVirtualPadWriter<MegaDrivePin> Pad{};
#endif

uint32_t LastUpdate = 0;
uint32_t LastLog = 0;

TestActions Actions{};

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
#elif defined(USE_MEGA_DRIVE_CONTROLLER)
	Pad.Setup();
#endif

	Serial.println();
	LogPadInfo();
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
#elif defined(USE_MEGA_DRIVE_CONTROLLER)
	if (timestamp - LastUpdate >= UpdatePeriodMillis)
	{
		LastUpdate = timestamp;
		Pad.Read();
	}
#endif

	if (Pad.Connected())
	{
		Actions.Parse(&Pad);

		if (Actions.HasDownAction())
		{
			LogActions();
		}
	}
	else
	{
		Actions.Clear();
	}

	LogPadState();
}

void LogPadInfo()
{
	Serial.println(F("Pad Features: "));

	if (Pad.FeatureStart() || Pad.FeatureSelect() || Pad.FeatureHome() || Pad.FeatureShare())
	{
		Serial.print(F("\tMenu Buttons: "));
		if (Pad.FeatureStart()) { Serial.print(F("Start ")); }
		if (Pad.FeatureSelect()) { Serial.print(F("Select ")); }
		if (Pad.FeatureHome()) { Serial.print(F("Home ")); }
		if (Pad.FeatureShare()) { Serial.print(F("Share ")); }
		Serial.println();
	}
	else
	{
		Serial.println(F("\tNo Menu buttons"));
	}

	if (Pad.FeatureJoy1() || Pad.FeatureJoy2())
	{
		Serial.print(F("\tJoysticks: "));
		if (Pad.FeatureJoy1()) { Serial.print(F("Joy1 ")); }
		if (Pad.FeatureJoy2()) { Serial.print(F("Joy2 ")); }
		Serial.println();
	}
	else
	{
		Serial.println(F("\tNo Joysticks"));
	}

	if (Pad.FeatureDPad())
	{
		Serial.print(F("\tDpad"));
	}
	else
	{
		Serial.print(F("\tNo DPad"));
	}
	Serial.println();

	if (Pad.FeatureA() || Pad.FeatureB() || Pad.FeatureX() || Pad.FeatureY())
	{
		Serial.print(F("\tFace Buttons: "));
		if (Pad.FeatureA()) { Serial.print(F("A ")); }
		if (Pad.FeatureB()) { Serial.print(F("B ")); }
		if (Pad.FeatureX()) { Serial.print(F("X ")); }
		if (Pad.FeatureY()) { Serial.print(F("Y ")); }
		Serial.println();
	}
	else
	{
		Serial.println(F("\tNo Face buttons"));
	}

	if (Pad.FeatureL2() || Pad.FeatureR2())
	{
		Serial.print(F("\tJoystick Buttons: "));
		if (Pad.FeatureL3()) { Serial.print(F("L3 ")); }
		if (Pad.FeatureR3()) { Serial.print(F("R3 ")); }
		Serial.println();
	}
	else
	{
		Serial.println(F("\tNo Joystick buttons"));
	}

	if (Pad.FeatureL1() || Pad.FeatureR1())
	{
		Serial.print(F("\tShoulder buttons: "));
		if (Pad.FeatureL1()) { Serial.print(F("L1 ")); }
		if (Pad.FeatureR1()) { Serial.print(F("R1 ")); }
		Serial.println();
	}
	else
	{
		Serial.println(F("\tNo Shoulder buttons"));
	}

	if (Pad.FeatureL2() || Pad.FeatureR2())
	{
		Serial.print(F("\tTriggers: "));
		if (Pad.FeatureL2()) { Serial.print(F("L2 ")); }
		if (Pad.FeatureR2()) { Serial.print(F("R2 ")); }
		Serial.println();
	}
	else
	{
		Serial.println(F("\tNo triggers"));
	}
	Serial.println();
}

void LogPadState()
{
	if (Pad.Connected())
	{
		if (millis() - LastLog >= LogPeriodMillis)
		{
			LastLog = millis();

			Serial.println();
			Serial.print(F("Navigation\t"));
			if (Pad.Home())
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
			case DPadEnum::UpRight:
				Serial.println(F("UpRight"));
				break;
			case DPadEnum::Right:
				Serial.println(F("Right"));
				break;
			case DPadEnum::DownRight:
				Serial.println(F("DownRight"));
				break;
			case DPadEnum::Down:
				Serial.println(F("Down"));
				break;
			case DPadEnum::DownLeft:
				Serial.println(F("DownLeft"));
				break;
			case DPadEnum::Left:
				Serial.println(F("Left"));
				break;
			case DPadEnum::UpLeft:
				Serial.println(F("UpLeft"));
				break;
			case DPadEnum::None:
			default:
				Serial.println(F("Center"));
				break;
			}

			Serial.print(F("Main Buttons\t"));

			if (Pad.FeatureA())
			{
				if (Pad.A())
				{
					Serial.print(F("A  "));
				}
				else
				{
					Serial.print(F("__ "));
				}
			}

			if (Pad.FeatureB())
			{
				if (Pad.B())
				{
					Serial.print(F("B  "));
				}
				else
				{
					Serial.print(F("__ "));
				}
			}

			if (Pad.FeatureX())
			{
				if (Pad.X())
				{
					Serial.print(F("X  "));
				}
				else
				{
					Serial.print(F("__ "));
				}
			}

			if (Pad.FeatureY())
			{
				if (Pad.Y())
				{
					Serial.print(F("Y  "));
				}
				else
				{
					Serial.print(F("__ "));
				}
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

void LogActions()
{
	if (Actions.A())
		Serial.print(F("A\t"));
	if (Actions.B())
		Serial.print(F("B\t"));
	if (Actions.X())
		Serial.print(F("X\t"));
	if (Actions.Y())
		Serial.print(F("Y\t"));
	if (Actions.L1())
		Serial.print(F("L1\t"));
	if (Actions.R1())
		Serial.print(F("R1\t"));
	if (Actions.L3())
		Serial.print(F("L3\t"));
	if (Actions.R3())
		Serial.print(F("R3\t"));

	if (Actions.Start())
		Serial.print(F("Start\t"));
	if (Actions.Select())
		Serial.print(F("Select\t"));
	if (Actions.Home())
		Serial.print(F("Home\t"));
	if (Actions.Share())
		Serial.print(F("Share "));

	if (Actions.L2())
		Serial.print(F("L2\t"));
	if (Actions.R2())
		Serial.print(F("R2\t"));

	if (Actions.DPad() != DPadEnum::None)
	{
		Serial.print(F("DPad::"));
		PrintDPadActions(Actions.DPad());
		Serial.print('\t');
	}

	if (Actions.Joy1() != DPadEnum::None)
	{
		Serial.print(F("Joy1::"));
		PrintDPadActions(Actions.Joy1());
		Serial.print('\t');
	}

	if (Actions.Joy2() != DPadEnum::None)
	{
		Serial.print(F("Joy2::"));
		PrintDPadActions(Actions.Joy2());
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
	case DPadEnum::UpRight:
		Serial.print(F("UpRight"));
		break;
	case DPadEnum::Right:
		Serial.print(F("Right"));
		break;
	case DPadEnum::DownRight:
		Serial.print(F("DownRight"));
		break;
	case DPadEnum::Down:
		Serial.print(F("Down"));
		break;
	case DPadEnum::DownLeft:
		Serial.print(F("DownLeft"));
		break;
	case DPadEnum::Left:
		Serial.print(F("Left"));
		break;
	case DPadEnum::UpLeft:
		Serial.print(F("UpLeft"));
		break;
	case DPadEnum::None:
	default:
		break;
	}
}
