#ifndef _ATARI_JOYSTICK_VIRTUAL_PAD_h
#define _ATARI_JOYSTICK_VIRTUAL_PAD_h

#include <stdint.h>
#include <VirtualPad.h>

namespace AtariJoystick
{
	using namespace VirtualPad;

	static constexpr uint32_t ConfigurationCode = Configuration::GetConfigurationCode(
		Configuration::GetFeatureFlags<Configuration::FeaturesEnum::DPad,
		Configuration::FeaturesEnum::A>());

	/// <summary>
	/// Atari 2600 Joystick reader and VirtualPad writer.
	/// </summary>
	/// <typeparam name="Pin">Pin mapping enum.</typeparam>
	template<typename Pin>
	class AtariJoystickVirtualPad : public ButtonVirtualPad<ConfigurationCode>
	{
	private:
		using Base = ButtonVirtualPad<ConfigurationCode>;

	protected:
		static constexpr auto PIN_INPUT_MODE = INPUT_PULLUP;

	public:
		void Setup()
		{
			Base::Clear();

			pinMode((uint8_t)Pin::Up, PIN_INPUT_MODE);
			pinMode((uint8_t)Pin::Down, PIN_INPUT_MODE);
			pinMode((uint8_t)Pin::Left, PIN_INPUT_MODE);
			pinMode((uint8_t)Pin::Right, PIN_INPUT_MODE);
			pinMode((uint8_t)Pin::Button, PIN_INPUT_MODE);
		}

		void Read()
		{
			SetA(!digitalRead((uint8_t)Pin::Button));

			SetDPad(
				!digitalRead((uint8_t)Pin::Up),
				!digitalRead((uint8_t)Pin::Down),
				!digitalRead((uint8_t)Pin::Left),
				!digitalRead((uint8_t)Pin::Right)
			);

			if (!Base::Connected())
			{
				SetConnected(true);
			}
		}
	};
}
#endif