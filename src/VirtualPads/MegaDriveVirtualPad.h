#ifndef _MEGA_DRIVE_VIRTUAL_PAD_h
#define _MEGA_DRIVE_VIRTUAL_PAD_h

#include <Arduino.h>
#include <VirtualPad.h>

namespace MegaDriveController
{
	using namespace VirtualPad;

	static constexpr uint32_t ConfigurationCode = Configuration::GetConfigurationCode(
		Configuration::GetFeatureFlags<Configuration::FeaturesEnum::DPad,
		Configuration::FeaturesEnum::Start,
		Configuration::FeaturesEnum::A, Configuration::FeaturesEnum::B,
		Configuration::FeaturesEnum::R3>()
		, 0, Configuration::NavigationEnum::AB);

	/// <summary>
	/// Mega Drive Controller Pad (3 Button) reader and VirtualPad writer.
	/// A, B buttons mapped directly.
	/// C is mapped to R3.
	/// </summary>
	/// <typeparam name="ControllerPin">Pin mapping enum.</typeparam>
	template<typename ControllerPin>
	class MegaDriveControllerVirtualPad : public ButtonVirtualPad<ConfigurationCode>
	{
	private:
		using Base = ButtonVirtualPad<ConfigurationCode>;

	protected:
		static constexpr auto PIN_INPUT_MODE = INPUT_PULLUP;

	public:
		MegaDriveControllerVirtualPad() : Base() {}

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
				if (Base::Connected())
				{
					SetConnected(false);
				}
			}
			else if (!Base::Connected())
			{
				SetConnected(true);
			}

			SetA(!digitalRead((uint8_t)ControllerPin::AB));
			SetStart(!digitalRead((uint8_t)ControllerPin::StartC));

			digitalWrite((uint8_t)ControllerPin::Select, HIGH);
			SetB(!digitalRead((uint8_t)ControllerPin::AB));
			SetR3(!digitalRead((uint8_t)ControllerPin::StartC));

			SetDPad(
				!digitalRead((uint8_t)ControllerPin::Up),
				!digitalRead((uint8_t)ControllerPin::Down),
				!digitalRead((uint8_t)ControllerPin::InfoLeft),
				!digitalRead((uint8_t)ControllerPin::InfoRight)
			);

			digitalWrite((uint8_t)ControllerPin::Select, LOW);
		}
	};
}

#endif
