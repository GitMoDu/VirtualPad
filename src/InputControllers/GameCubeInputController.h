// GameCubeInputController.h

#ifndef _GAMECUBE_INPUT_CONTROLLER_h
#define _GAMECUBE_INPUT_CONTROLLER_h

#include <GameCubeController.h>
#include "../WriteIInputController.h"
#include "../Model/IInputMapper.h"

template<int8_t Joy1XMin = -100, int8_t Joy1XMax = 100,
	int8_t Joy1YMin = -100, int8_t Joy1YMax = 100,
	int8_t Joy2XMin = -90, int8_t Joy2XMax = 90,
	int8_t Joy2YMin = -90, int8_t Joy2YMax = 90,
	uint8_t L2Min = 40, int8_t L2Max = 180,
	uint8_t R2Min = 40, int8_t R2Max = 180>
class GameCubeInputController : public WriteInputController
{
public:
	static constexpr uint16_t Features = FeatureFlags::GetFlags<
		FeaturesEnum::Joy2,
		FeaturesEnum::L1, FeaturesEnum::R1,
		FeaturesEnum::L2, FeaturesEnum::R2,
		FeaturesEnum::R3>();

private:
	using Joystick1Mapper = IInputMapper::JoystickInt8<Joy1XMin, Joy1XMax, Joy1YMin, Joy1YMax>;
	using Joystick2Mapper = IInputMapper::JoystickInt8<Joy2XMin, Joy2XMax, Joy2YMin, Joy2YMax>;
	using L2Mapper = IInputMapper::AxisUint8<L2Min, L2Max>;
	using R2Mapper = IInputMapper::AxisUint8<R2Min, R2Max>;
	using ButtonsEnum = GameCubeController::ButtonsEnum;

public:
	GameCubeInputController() : WriteInputController(Features)
	{}

	void MapControllerData(GameCubeController::data_t& data)
	{
		SetA(data.Button<ButtonsEnum::A>());
		SetB(data.Button<ButtonsEnum::B>());
		SetX(data.Button<ButtonsEnum::X>());
		SetY(data.Button<ButtonsEnum::Y>());

		SetL1(data.Button<ButtonsEnum::L1>());
		SetR1(data.Button<ButtonsEnum::R1>());
		SetStart(data.Button<ButtonsEnum::Start>());

		SetDPad(data.Button<ButtonsEnum::Up>(), data.Button<ButtonsEnum::Down>(), data.Button<ButtonsEnum::Left>(), data.Button<ButtonsEnum::Right>());

		Joystick1Mapper::Map(data.JoystickX, data.JoystickY, State.Joy1X, State.Joy1Y);
		Joystick2Mapper::Map(data.JoystickCX, data.JoystickCY, State.Joy2X, State.Joy2Y);

		SetL2(L2Mapper::Map(data.L2));
		SetR2(L2Mapper::Map(data.R2));

		if (!State.Connected)
		{
			State.Connected = true;
		}
	}
};
#endif