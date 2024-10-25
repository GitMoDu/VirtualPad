// MegaDriveVirtualPadWriter.h

#ifndef _MEGA_DRIVE_VIRTUAL_PAD_WRITER_h
#define _MEGA_DRIVE_VIRTUAL_PAD_WRITER_h

#include <stdint.h>

#include <WriteVirtualPad.h>

/// <summary>
/// Mega Drive Controller Pad (3 Button) reader and VirtualPad writer.
/// A, B buttons mapped directly.
/// C is mapped to R3.
/// </summary>
/// <typeparam name="ControllerPin">Pin mapping enum.</typeparam>
template<typename ControllerPin>
class MegaDriveVirtualPadWriter : public WriteVirtualPad
{
public:
	static constexpr uint32_t Features = VirtualPadConfiguration::GetFeatureFlags<
		FeaturesEnum::DPad,
		FeaturesEnum::Start,
		FeaturesEnum::A, FeaturesEnum::B,
		FeaturesEnum::R3>();

protected:
	static constexpr uint8_t PIN_INPUT_MODE = INPUT;

public:
	MegaDriveVirtualPadWriter()
		: WriteVirtualPad(Features)
	{}

	void Setup()
	{
		pinMode((uint8_t)ControllerPin::Up, PIN_INPUT_MODE);
		pinMode((uint8_t)ControllerPin::Down, PIN_INPUT_MODE);
		pinMode((uint8_t)ControllerPin::InfoLeft, PIN_INPUT_MODE);
		pinMode((uint8_t)ControllerPin::InfoRight, PIN_INPUT_MODE);
		pinMode((uint8_t)ControllerPin::AB, PIN_INPUT_MODE);
		pinMode((uint8_t)ControllerPin::StartC, PIN_INPUT_MODE);

		pinMode((uint8_t)ControllerPin::Select, OUTPUT);
		digitalWrite((uint8_t)ControllerPin::Select, LOW);
	}

	void Read()
	{
		digitalWrite((uint8_t)ControllerPin::Select, LOW);
		if (digitalRead((uint8_t)ControllerPin::InfoLeft) || digitalRead((uint8_t)ControllerPin::InfoRight))
		{
			if (Connected())
			{
				SetConnected(false);
			}
		}
		else if (!Connected())
		{
			SetConnected(true);
		}

		WriteVirtualPad::SetA(!digitalRead((uint8_t)ControllerPin::AB));
		WriteVirtualPad::SetStart(!digitalRead((uint8_t)ControllerPin::StartC));

		digitalWrite((uint8_t)ControllerPin::Select, HIGH);
		WriteVirtualPad::SetB(!digitalRead((uint8_t)ControllerPin::AB));
		WriteVirtualPad::SetR3(!digitalRead((uint8_t)ControllerPin::StartC));

		WriteVirtualPad::SetDPad(
			!digitalRead((uint8_t)ControllerPin::Up),
			!digitalRead((uint8_t)ControllerPin::Down),
			!digitalRead((uint8_t)ControllerPin::InfoLeft),
			!digitalRead((uint8_t)ControllerPin::InfoRight)
		);

		digitalWrite((uint8_t)ControllerPin::Select, LOW);
	}
};
#endif
