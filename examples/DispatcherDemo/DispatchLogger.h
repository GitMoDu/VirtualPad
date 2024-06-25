// DispatchLogger.h

#ifndef _DISPATCH_LOGGER_h
#define _DISPATCH_LOGGER_h

#include <InputTaskControllers.h>

/// <summary>
/// Example class with virtual IInputControllerDispatch interface.
/// </summary>
class DispatchLogger : public virtual IInputControllerDispatch
{
private:
	const uint32_t LogPeriodMillis;
	uint32_t LastLog = 0;

public:
	DispatchLogger(const uint32_t logPeriodMillis)
		: LogPeriodMillis(logPeriodMillis)
	{}

public:
	void OnControllerUpdate(IInputController* controller) final
	{
		if (controller->Connected())
		{
			// Only log to serial if enough time has passed since last update.
			if (millis() - LastLog >= LogPeriodMillis)
			{
				LastLog = millis();

				Serial.println();
				Serial.print(F("Navigation\t"));
				if (controller->GetHome())
				{
					Serial.print(F("Home "));
				}
				if (controller->GetAccept())
				{
					Serial.print(F("Accept "));
				}
				if (controller->GetReject())
				{
					Serial.print(F("Reject "));
				}
				Serial.println();

				Serial.print(F("DPad\t"));
				switch (controller->DPad())
				{
				case DPadEnum::Up:
					Serial.println(F("Up"));
					break;
				case DPadEnum::UpLeft:
					Serial.println(F("UpLeft"));
					break;
				case DPadEnum::UpRight:
					Serial.println(F("UpRight"));
					break;
				case DPadEnum::Down:
					Serial.println(F("Down"));
					break;
				case DPadEnum::DownLeft:
					Serial.println(F("DownLeft"));
					break;
				case DPadEnum::DownRight:
					Serial.println(F("DownRight"));
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
				if (controller->A())
				{
					Serial.print(F("A  "));
				}
				else
				{
					Serial.print(F("__ "));
				}

				if (controller->B())
				{
					Serial.print(F("B  "));
				}
				else
				{
					Serial.print(F("__ "));
				}

				if (controller->X())
				{
					Serial.print(F("X  "));
				}
				else
				{
					Serial.print(F("__ "));
				}

				if (controller->Y())
				{
					Serial.print(F("Y  "));
				}
				else
				{
					Serial.print(F("__ "));
				}

				if (controller->FeatureL1())
				{
					if (controller->L1())
					{
						Serial.print(F("L1 "));
					}
					else
					{
						Serial.print(F("__ "));
					}
				}

				if (controller->FeatureR1())
				{
					if (controller->R1())
					{
						Serial.print(F("R1 "));
					}
					else
					{
						Serial.print(F("__ "));
					}
				}

				if (controller->FeatureL3())
				{
					if (controller->L3())
					{
						Serial.print(F("L3 "));
					}
					else
					{
						Serial.print(F("__ "));
					}
				}

				if (controller->FeatureR3())
				{
					if (controller->R3())
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
				if (controller->Start())
				{
					Serial.print(F("Start "));
				}
				if (controller->Select())
				{
					Serial.print(F("Select "));
				}
				if (controller->Home())
				{
					Serial.print(F("Home "));
				}
				if (controller->Share())
				{
					Serial.print(F("Share "));
				}
				Serial.println();

				Serial.print(F("Joy1(x,y)\t("));
				Serial.print(controller->Joy1X());
				Serial.print(',');
				Serial.print(controller->Joy1Y());
				Serial.println(')');

				if (controller->FeatureJoy2())
				{
					Serial.print(F("Joy2(x,y)\t("));
					Serial.print(controller->Joy2X());
					Serial.print(',');
					Serial.print(controller->Joy2Y());
					Serial.println(')');
				}

				if (controller->FeatureL2())
				{
					Serial.print(F("L2\t"));
					Serial.println(controller->L2());
				}

				if (controller->FeatureR2())
				{
					Serial.print(F("R2\t"));
					Serial.println(controller->R2());
				}
			}
		}
		else
		{
			Serial.println(F("No controller"));

			LastLog = millis() - LogPeriodMillis;
		}
	}
};
#endif

