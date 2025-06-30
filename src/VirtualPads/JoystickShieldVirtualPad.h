#ifndef _JOYSTICK_SHIELD_VIRTUAL_PAD_h
#define _JOYSTICK_SHIELD_VIRTUAL_PAD_h

#include <VirtualPad.h>
#include <Arduino.h>

namespace JoystickShield
{
	using namespace VirtualPad;

	static constexpr uint32_t ConfigurationCode = Configuration::GetConfigurationCode(
		Configuration::GetFeatureFlags<Configuration::FeaturesEnum::Joy1,
		Configuration::FeaturesEnum::Start, Configuration::FeaturesEnum::Select,
		Configuration::FeaturesEnum::A, Configuration::FeaturesEnum::B,
		Configuration::FeaturesEnum::X, Configuration::FeaturesEnum::Y,
		Configuration::FeaturesEnum::L3>()
		, 0, Configuration::NavigationEnum::AB);

	template<const uint8_t pinJoyX = UINT8_MAX, const uint8_t pinJoyY = UINT8_MAX,
		const uint8_t pinA = UINT8_MAX, const uint8_t pinB = UINT8_MAX,
		const uint8_t pinX = UINT8_MAX, const uint8_t pinY = UINT8_MAX,
		const uint8_t pinL3 = UINT8_MAX,
		const uint8_t pinStart = UINT8_MAX,
		const uint8_t pinSelect = UINT8_MAX>
	class JoystickShieldVirtualPad : public VirtualPad::AnalogVirtualPad<ConfigurationCode>
	{
	private:
		using Base = VirtualPad::AnalogVirtualPad<ConfigurationCode>;

	private:
		static constexpr uint16_t AdcReference = (1024 - 1);
		static constexpr uint16_t AdcMiddle = ((1024 / 2) + 1);

	public:
		JoystickShieldVirtualPad() : Base() {}

		const bool Setup()
		{
			Base::Clear();

			if (pinJoyX != UINT8_MAX)
			{
				pinMode(pinJoyX, INPUT);
			}

			if (pinJoyY != UINT8_MAX)
			{
				pinMode(pinJoyY, INPUT);
			}

			if (pinA != UINT8_MAX)
			{
				pinMode(pinA, INPUT);
			}

			if (pinB != UINT8_MAX)
			{
				pinMode(pinB, INPUT);
			}

			if (pinX != UINT8_MAX)
			{
				pinMode(pinX, INPUT);
			}

			if (pinY != UINT8_MAX)
			{
				pinMode(pinY, INPUT);
			}

			return true;
		}

		void Step()
		{
			uint16_t x = AdcMiddle;
			uint16_t y = AdcMiddle;

			int16_t joyX = 0;
			int16_t joyY = 0;

			if (pinJoyX != UINT8_MAX)
			{
				x = analogRead(pinJoyX);
			}

			if (pinJoyY != UINT8_MAX)
			{
				y = analogRead(pinJoyY);
			}

			if (x >= AdcMiddle)
			{
				joyX = ((uint32_t)(x - AdcMiddle) * INT16_MAX) / (AdcReference - AdcMiddle);
			}
			else
			{
				joyX = ((int32_t)(AdcMiddle - x) * INT16_MIN) / (AdcMiddle);
			}

			if (y >= AdcMiddle)
			{
				joyY = ((uint32_t)(y - AdcMiddle) * INT16_MAX) / (AdcReference - AdcMiddle);
			}
			else
			{
				joyY = ((int32_t)(AdcMiddle - y) * INT16_MIN) / (AdcMiddle);
			}
			SetJoy1(joyX, joyY);

			if (pinA != UINT8_MAX)
			{
				SetA(!digitalRead(pinA));
			}
			else
			{
				SetA(false);
			}

			if (pinB != UINT8_MAX)
			{
				SetB(!digitalRead(pinB));
			}
			else
			{
				SetB(false);
			}

			if (pinX != UINT8_MAX)
			{
				SetX(!digitalRead(pinX));
			}
			else
			{
				SetX(false);
			}

			if (pinY != UINT8_MAX)
			{
				SetY(!digitalRead(pinY));
			}
			else
			{
				SetY(false);
			}

			if (pinL3 != UINT8_MAX)
			{
				SetL3(!digitalRead(pinL3));
			}
			else
			{
				SetL3(false);
			}

			if (pinStart != UINT8_MAX)
			{
				SetStart(!digitalRead(pinStart));
			}
			else
			{
				SetStart(false);
			}

			if (pinSelect != UINT8_MAX)
			{
				SetSelect(!digitalRead(pinSelect));
			}
			else
			{
				SetSelect(false);
			}

			if (!Base::Connected())
			{
				SetConnected(true);
			}
		}
	};
}
#endif