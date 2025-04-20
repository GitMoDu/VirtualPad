#ifndef _NINTENDO_64_VIRTUAL_PAD_h
#define _NINTENDO_64_VIRTUAL_PAD_h

#include <VirtualPad.h>
#include <Nintendo64Controller.h>

namespace Nintendo64Controller
{
	using namespace VirtualPad;

	static constexpr uint32_t ConfigurationCode = Configuration::GetConfigurationCode(
		Configuration::GetFeatureFlags<
		Configuration::FeaturesEnum::Start,
		Configuration::FeaturesEnum::DPad,
		Configuration::FeaturesEnum::Joy1, Configuration::FeaturesEnum::Joy2,
		Configuration::FeaturesEnum::A, Configuration::FeaturesEnum::B,
		Configuration::FeaturesEnum::L1, Configuration::FeaturesEnum::R1,
		Configuration::FeaturesEnum::L2>(),
		Configuration::GetPropertyFlags<Configuration::PropertiesEnum::Joy2Digital, Configuration::PropertiesEnum::L2R2Digital>(),
		Configuration::NavigationEnum::AB);

	//static constexpr uint32_t ConfigurationCode = GetConfigurationCode();

	/// <summary>
	/// Nintendo 64 to VirtualPad.
	/// C Buttons are mapped to (digital) Joy2.
	/// Z is mapped to (digital) L2.
	/// L and R are mapped to L1 and R1.
	/// </summary>
	/// <typeparam name="JoyXMin"></typeparam>
	/// <typeparam name="JoyYMin"></typeparam>
	/// <typeparam name="JoyYMax"></typeparam>
	/// <typeparam name="JoyXMax"></typeparam>
	template<int8_t JoyXMin = -80, int8_t JoyXMax = 80,
		int8_t JoyYMin = -80, int8_t JoyYMax = 80>
	class Nintendo64VirtualPad : public VirtualPad::AnalogVirtualPad<ConfigurationCode>
	{
	private:
		using Base = VirtualPad::AnalogVirtualPad<ConfigurationCode>;

	public:
		using Base::joy1X;
		using Base::joy1Y;

	private:
		using JoystickMapper = JoystickInt8<JoyXMin, JoyXMax, JoyYMin, JoyYMax>;
		using ButtonsEnum = Nintendo64Controller::ButtonsEnum;

	public:
		Nintendo64VirtualPad() : Base() {}

		void Step(Nintendo64Controller::data_t& data)
		{
			SetA(*this, data.Button<ButtonsEnum::A>());
			SetB(*this, data.Button<ButtonsEnum::B>());

			SetL1(*this, data.Button<ButtonsEnum::L>());
			SetR1(*this, data.Button<ButtonsEnum::R>());
			SetStart(*this, data.Button<ButtonsEnum::Start>());

			SetDPad(*this, data.Button<ButtonsEnum::Up>(), data.Button<ButtonsEnum::Down>(), data.Button<ButtonsEnum::Left>(), data.Button<ButtonsEnum::Right>());

			JoystickMapper::Map(data.JoystickX, data.JoystickY, joy1X, joy1Y);

			SetJoy2(*this, data.Button<ButtonsEnum::CUp>(), data.Button<ButtonsEnum::CDown>(), data.Button<ButtonsEnum::CLeft>(), data.Button<ButtonsEnum::CRight>());

			SetL2(*this, UINT16_MAX * data.Button<ButtonsEnum::Z>());

			if (!Base::Connected())
			{
				SetConnected(*this, true);
			}
		}
	};
}

#endif