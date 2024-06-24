// Nintendo64InputController.h

#ifndef _NINTENDO_64_INPUT_CONTROLLER_h
#define _NINTENDO_64_INPUT_CONTROLLER_h

#include <Nintendo64Controller.h>
#include "../WriteIInputController.h"
#include "../Model/IInputMapper.h"


/// <summary>
/// Nintendo 64 to IInputController mapper.
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
class Nintendo64InputController : public WriteInputController
{
public:
	static constexpr uint16_t Features = FeatureFlags::GetFlags<
		FeaturesEnum::Joy2, FeaturesEnum::Joy2Digital,
		FeaturesEnum::L1, FeaturesEnum::R1,
		FeaturesEnum::L2, FeaturesEnum::L2R2Digital>();

private:
	using JoystickMapper = IInputMapper::JoystickInt8<JoyXMin, JoyXMax, JoyYMin, JoyYMax>;
	using ButtonsEnum = Nintendo64Controller::ButtonsEnum;

public:
	Nintendo64InputController() : WriteInputController(Features)
	{}

	void MapControllerData(Nintendo64Controller::data_t& data)
	{
		SetA(data.Button<ButtonsEnum::A>());
		SetB(data.Button<ButtonsEnum::B>());

		SetL1(data.Button<ButtonsEnum::L>());
		SetR1(data.Button<ButtonsEnum::R>());
		SetStart(data.Button<ButtonsEnum::Start>());

		SetDPad(data.Button<ButtonsEnum::Up>(), data.Button<ButtonsEnum::Down>(), data.Button<ButtonsEnum::Left>(), data.Button<ButtonsEnum::Right>());

		JoystickMapper::Map(data.JoystickX, data.JoystickY, State.Joy1X, State.Joy1Y);

		SetJoy2(data.Button<ButtonsEnum::CUp>(), data.Button<ButtonsEnum::CDown>(), data.Button<ButtonsEnum::CLeft>(), data.Button<ButtonsEnum::CRight>());

		SetL2(UINT16_MAX * data.Button<ButtonsEnum::Z>());

		if (!State.Connected)
		{
			State.Connected = true;
		}
	}
};
#endif