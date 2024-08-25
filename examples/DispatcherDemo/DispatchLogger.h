// DispatchLogger.h

#ifndef _DISPATCH_LOGGER_h
#define _DISPATCH_LOGGER_h

#include <VirtualPad.h>

/// <summary>
/// Example class with virtual VirtualPadListener interface.
/// </summary>
class DispatchLogger : public virtual VirtualPadListener
{
private:
	const uint32_t LogPeriodMillis;
	uint32_t LastLog = 0;

public:
	DispatchLogger(const uint32_t logPeriodMillis)
		: VirtualPadListener()
		, LogPeriodMillis(logPeriodMillis)
	{}

public:
	void OnUpdate(VirtualPad* pad) final
	{
		if (pad->Connected())
		{
			// Only log to serial if enough time has passed since last update.
			if (millis() - LastLog >= LogPeriodMillis)
			{
				LastLog = millis();

				Serial.println();
				Serial.print(F("Navigation\t"));
				if (pad->GetHome())
				{
					Serial.print(F("Home "));
				}
				if (pad->GetAccept())
				{
					Serial.print(F("Accept "));
				}
				if (pad->GetReject())
				{
					Serial.print(F("Reject "));
				}
				Serial.println();

				Serial.print(F("DPad\t"));
				switch (pad->DPad())
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
				if (pad->A())
				{
					Serial.print(F("A  "));
				}
				else
				{
					Serial.print(F("__ "));
				}

				if (pad->B())
				{
					Serial.print(F("B  "));
				}
				else
				{
					Serial.print(F("__ "));
				}

				if (pad->X())
				{
					Serial.print(F("X  "));
				}
				else
				{
					Serial.print(F("__ "));
				}

				if (pad->Y())
				{
					Serial.print(F("Y  "));
				}
				else
				{
					Serial.print(F("__ "));
				}

				if (pad->FeatureL1())
				{
					if (pad->L1())
					{
						Serial.print(F("L1 "));
					}
					else
					{
						Serial.print(F("__ "));
					}
				}

				if (pad->FeatureR1())
				{
					if (pad->R1())
					{
						Serial.print(F("R1 "));
					}
					else
					{
						Serial.print(F("__ "));
					}
				}

				if (pad->FeatureL3())
				{
					if (pad->L3())
					{
						Serial.print(F("L3 "));
					}
					else
					{
						Serial.print(F("__ "));
					}
				}

				if (pad->FeatureR3())
				{
					if (pad->R3())
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
				if (pad->Start())
				{
					Serial.print(F("Start "));
				}
				if (pad->Select())
				{
					Serial.print(F("Select "));
				}
				if (pad->Home())
				{
					Serial.print(F("Home "));
				}
				if (pad->Share())
				{
					Serial.print(F("Share "));
				}
				Serial.println();

				Serial.print(F("Joy1(x,y)\t("));
				Serial.print(pad->Joy1X());
				Serial.print(',');
				Serial.print(pad->Joy1Y());
				Serial.println(')');

				if (pad->FeatureJoy2())
				{
					Serial.print(F("Joy2(x,y)\t("));
					Serial.print(pad->Joy2X());
					Serial.print(',');
					Serial.print(pad->Joy2Y());
					Serial.println(')');
				}

				if (pad->FeatureL2())
				{
					Serial.print(F("L2\t"));
					Serial.println(pad->L2());
				}

				if (pad->FeatureR2())
				{
					Serial.print(F("R2\t"));
					Serial.println(pad->R2());
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

