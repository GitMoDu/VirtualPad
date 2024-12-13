// AtariJoystickVirtualPadWriter.h

#ifndef _ATARI_JOYSTICK_VIRTUAL_PAD_WRITER_h
#define _ATARI_JOYSTICK_VIRTUAL_PAD_WRITER_h

#include <stdint.h>
#include <WriteVirtualPad.h>

/// <summary>
/// Atari 2600 Joystick reader and VirtualPad writer.
/// </summary>
/// <typeparam name="Pin">Pin mapping enum.</typeparam>
template<typename Pin>
class AtariJoystickVirtualPadWriter : public WriteVirtualPad
{
public:
	static constexpr uint32_t ConfigurationCode =
		VirtualPadConfiguration::GetConfigurationCode(
			VirtualPadConfiguration::GetFeatureFlags<FeaturesEnum::DPad,
			FeaturesEnum::A>(),
			VirtualPadConfiguration::NoProperties,
			NavigationEnum::AB);

protected:
	static constexpr uint8_t PIN_INPUT_MODE = INPUT_PULLUP;

public:
	AtariJoystickVirtualPadWriter()
		: WriteVirtualPad(ConfigurationCode)
	{
	}

	void Setup()
	{
		pinMode((uint8_t)Pin::Up, PIN_INPUT_MODE);
		pinMode((uint8_t)Pin::Down, PIN_INPUT_MODE);
		pinMode((uint8_t)Pin::Left, PIN_INPUT_MODE);
		pinMode((uint8_t)Pin::Right, PIN_INPUT_MODE);
		pinMode((uint8_t)Pin::Button, PIN_INPUT_MODE);
	}

	void Read()
	{
		WriteVirtualPad::SetA(!digitalRead((uint8_t)Pin::Button));

		WriteVirtualPad::SetDPad(
			!digitalRead((uint8_t)Pin::Up),
			!digitalRead((uint8_t)Pin::Down),
			!digitalRead((uint8_t)Pin::Left),
			!digitalRead((uint8_t)Pin::Right)
		);
	}
};
#endif
