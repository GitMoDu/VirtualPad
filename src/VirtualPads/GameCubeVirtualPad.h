#ifndef _GAMECUBE_VIRTUAL_PAD_h
#define _GAMECUBE_VIRTUAL_PAD_h

#include <VirtualPad.h>
#include <GameCubeController.h>

namespace GameCubeController
{
	using namespace VirtualPad;

	static constexpr uint32_t ConfigurationCode = Configuration::GetConfigurationCode(
		Configuration::GetFeatureFlags<
		Configuration::FeaturesEnum::Start,
		Configuration::FeaturesEnum::DPad,
		Configuration::FeaturesEnum::Joy1,
		Configuration::FeaturesEnum::Joy2,
		Configuration::FeaturesEnum::A, Configuration::FeaturesEnum::B, Configuration::FeaturesEnum::X, Configuration::FeaturesEnum::Y,
		Configuration::FeaturesEnum::L1,
		Configuration::FeaturesEnum::L2, Configuration::FeaturesEnum::R2,
		Configuration::FeaturesEnum::P1, Configuration::FeaturesEnum::P2>(),
		Configuration::NoProperties,
		Configuration::NavigationEnum::AB);

	/// <summary>
	/// Nintendo GameCube VirtualPad mapper.
	/// A, B, X, Y buttons mapped directly.
	/// L2 and R2 deep-click mapped to L1 and R1.
	/// Z mapped to R3.
	/// </summary>
	template<int8_t Joy1XMin = -100, int8_t Joy1XMax = 100,
		int8_t Joy1YMin = -100, int8_t Joy1YMax = 100,
		int8_t Joy2XMin = -90, int8_t Joy2XMax = 90,
		int8_t Joy2YMin = -90, int8_t Joy2YMax = 90,
		uint8_t L2Min = 40, int8_t L2Max = 100,
		uint8_t R2Min = 40, int8_t R2Max = 100>
	class GameCubeVirtualPad : public VirtualPad::AnalogVirtualPad<ConfigurationCode>
	{
	private:
		using Base = VirtualPad::AnalogVirtualPad<ConfigurationCode>;

	public:
		using Base::joy1X;
		using Base::joy1Y;

	private:
		using Joystick1Mapper = JoystickInt8<Joy1XMin, Joy1XMax, Joy1YMin, Joy1YMax>;
		using Joystick2Mapper = JoystickInt8<Joy2XMin, Joy2XMax, Joy2YMin, Joy2YMax>;
		using L2Mapper = AxisUint8<L2Min, L2Max>;
		using R2Mapper = AxisUint8<R2Min, R2Max>;
		using ButtonsEnum = GameCubeController::ButtonsEnum;

	public:
		void Step(GameCubeController::data_t& data)
		{
			SetStart(*this, data.Button<ButtonsEnum::Start>());

			SetDPad(*this, data.Button<ButtonsEnum::Up>(), data.Button<ButtonsEnum::Down>(), data.Button<ButtonsEnum::Left>(), data.Button<ButtonsEnum::Right>());

			SetA(*this, data.Button<ButtonsEnum::A>());
			SetB(*this, data.Button<ButtonsEnum::B>());
			SetX(*this, data.Button<ButtonsEnum::X>());
			SetY(*this, data.Button<ButtonsEnum::Y>());

			SetP1(*this, data.Button<ButtonsEnum::L1>());
			SetP2(*this, data.Button<ButtonsEnum::R1>());

			SetL2(*this, L2Mapper::Map(data.L2));
			SetR2(*this, L2Mapper::Map(data.R2));

			SetL1(*this, data.Button<ButtonsEnum::Z>());

			Joystick1Mapper::Map(data.JoystickX, data.JoystickY, joy1X, joy1Y);
			Joystick2Mapper::Map(data.JoystickCX, data.JoystickCY, joy2X, joy2Y);

			if (!Base::Connected())
			{
				SetConnected(*this, true);
			}
		}
	};
}
#endif