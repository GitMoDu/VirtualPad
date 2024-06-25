// Actions.h

#ifndef _ACTIONS_h
#define _ACTIONS_h

#include <IInputController.h>

/// <summary>
/// Maps all inputs to actions.
/// </summary>
class Actions
{
private:
	ButtonParser::Action ActionMainButtons[(uint8_t)MainButtonEnum::MainButtonEnumCount]{};
	ButtonParser::Action ActionMenuButtons[(uint8_t)MenuButtonEnum::MenuButtonEnumCount]{};

	ButtonParser::DirectionAction ActionDPad{};

	JoystickParser::JoystickAnalogToDigitalAction<> ActionJoy1{};
	JoystickParser::JoystickAnalogToDigitalAction<> ActionJoy2{};

	AxisParser::AxisAnalogToDigitalAction<> ActionL2;
	AxisParser::AxisAnalogToDigitalAction<> ActionR2;

public:
	Actions()
	{}

	void Clear()
	{
		for (uint8_t i = 0; i < (uint8_t)MainButtonEnum::MainButtonEnumCount; i++)
		{
			ActionMainButtons[i].Clear();
		}

		for (uint8_t i = 0; i < (uint8_t)MenuButtonEnum::MenuButtonEnumCount; i++)
		{
			ActionMenuButtons[i].Clear();
		}

		ActionDPad.Clear();
		ActionJoy1.Clear();
		ActionJoy2.Clear();
		ActionL2.Clear();
		ActionR2.Clear();
	}

	const bool HasDownAction() const
	{
		if (MainButtons())
		{
			return true;
		}

		if (MenuButtons())
		{
			return true;
		}

		return ActionDPad.HasAction() || ActionJoy1.HasAction() || ActionJoy2.HasAction() || ActionL2.ActionUp() || ActionR2.ActionUp();
	}

	const bool Parse(IInputController* inputController)
	{
		if (inputController != nullptr
			&& inputController->Connected())
		{
			bool action = false;

			action |= ActionMainButtons[(uint8_t)MainButtonEnum::A].Parse(inputController->A());
			action |= ActionMainButtons[(uint8_t)MainButtonEnum::B].Parse(inputController->B());
			action |= ActionMainButtons[(uint8_t)MainButtonEnum::X].Parse(inputController->X());
			action |= ActionMainButtons[(uint8_t)MainButtonEnum::Y].Parse(inputController->Y());

			action |= ActionMainButtons[(uint8_t)MainButtonEnum::L1].Parse(inputController->L1());
			action |= ActionMainButtons[(uint8_t)MainButtonEnum::R1].Parse(inputController->R1());
			action |= ActionMainButtons[(uint8_t)MainButtonEnum::L3].Parse(inputController->L3());
			action |= ActionMainButtons[(uint8_t)MainButtonEnum::R3].Parse(inputController->R3());

			action |= ActionMenuButtons[(uint8_t)MenuButtonEnum::Start].Parse(inputController->Start());
			action |= ActionMenuButtons[(uint8_t)MenuButtonEnum::Select].Parse(inputController->Select());
			action |= ActionMenuButtons[(uint8_t)MenuButtonEnum::Home].Parse(inputController->Home());
			action |= ActionMenuButtons[(uint8_t)MenuButtonEnum::Share].Parse(inputController->Share());

			action |= ActionDPad.Parse(inputController->DPad());

			action |= ActionJoy1.Parse(inputController->Joy1X(), inputController->Joy1Y());
			action |= ActionJoy2.Parse(inputController->Joy2X(), inputController->Joy2Y());

			action |= ActionL2.Parse(inputController->L2());
			action |= ActionR2.Parse(inputController->R2());

			return action;
		}
		else
		{
			Clear();

			return false;
		}
	}

	const DPadEnum DPad() const
	{
		return ActionDPad.ActionDPad();
	}

	const DPadEnum Joy1() const
	{
		return ActionJoy1.ActionDPad();
	}

	const DPadEnum Joy2() const
	{
		return ActionJoy2.ActionDPad();
	}

	const bool L2() const
	{
		return ActionL2.ActionUp();
	}

	const bool R2() const
	{
		return ActionR2.ActionUp();
	}

	const bool MenuButtons() const
	{
		for (uint8_t i = 0; i < (uint8_t)MenuButtonEnum::MenuButtonEnumCount; i++)
		{
			if (ActionMenuButtons[i].ActionDown())
			{
				return true;
			}
		}

		return false;
	}

	const bool MainButtons() const
	{
		for (uint8_t i = 0; i < (uint8_t)MainButtonEnum::MainButtonEnumCount; i++)
		{
			if (ActionMainButtons[i].ActionDown())
			{
				return true;
			}
		}

		return false;
	}

	const bool A() const
	{
		return ActionMainButtons[(uint8_t)MainButtonEnum::A].ActionDown();
	}

	const bool B() const
	{
		return ActionMainButtons[(uint8_t)MainButtonEnum::B].ActionDown();
	}

	const bool X() const
	{
		return ActionMainButtons[(uint8_t)MainButtonEnum::X].ActionDown();
	}

	const bool Y() const
	{
		return ActionMainButtons[(uint8_t)MainButtonEnum::Y].ActionDown();
	}

	const bool L1() const
	{
		return ActionMainButtons[(uint8_t)MainButtonEnum::L1].ActionDown();
	}

	const bool R1() const
	{
		return ActionMainButtons[(uint8_t)MainButtonEnum::R1].ActionDown();
	}

	const bool L3() const
	{
		return ActionMainButtons[(uint8_t)MainButtonEnum::L3].ActionDown();
	}

	const bool R3() const
	{
		return ActionMainButtons[(uint8_t)MainButtonEnum::R3].ActionDown();
	}

	const bool Start() const
	{
		return ActionMenuButtons[(uint8_t)MenuButtonEnum::Start].ActionDown();
	}

	const bool Select() const
	{
		return ActionMenuButtons[(uint8_t)MenuButtonEnum::Select].ActionDown();
	}

	const bool Home() const
	{
		return ActionMenuButtons[(uint8_t)MenuButtonEnum::Home].ActionDown();
	}

	const bool Share() const
	{
		return ActionMenuButtons[(uint8_t)MenuButtonEnum::Share].ActionDown();
	}
};

#endif

