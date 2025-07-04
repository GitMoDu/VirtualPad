// TestActions.h

#ifndef _TEST_ACTIONS_h
#define _TEST_ACTIONS_h

#include <VirtualPad.h>

using namespace VirtualPad;

/// <summary>
/// Maps all inputs to actions.
/// </summary>
class TestActions
{
private:
	ButtonParser::Action ActionFlags[(uint8_t)ButtonEnum::EnumCount]{};

	ButtonParser::DirectionAction ActionDPad{};

	JoystickParser::JoystickAnalogToDigitalAction<> ActionJoy1{};
	JoystickParser::JoystickAnalogToDigitalAction<> ActionJoy2{};

	AxisParser::AxisAnalogToDigitalAction<> ActionL2;
	AxisParser::AxisAnalogToDigitalAction<> ActionR2;

public:
	TestActions()
	{
	}

	void Clear()
	{
		for (uint8_t i = 0; i < (uint8_t)ButtonEnum::EnumCount; i++)
		{
			ActionFlags[i].Clear();
		}

		ActionDPad.Clear();
		ActionJoy1.Clear();
		ActionJoy2.Clear();
		ActionL2.Clear();
		ActionR2.Clear();
	}

	const bool HasDownAction() const
	{
		if (Buttons())
		{
			return true;
		}

		return ActionDPad.HasAction() || ActionJoy1.HasAction() || ActionJoy2.HasAction() || ActionL2.ActionUp() || ActionR2.ActionUp();
	}

	const bool Parse(analog_pad_state_t& state)
	{
		if (state.Connected())
		{
			bool action = false;

			action |= ActionFlags[(uint8_t)ButtonEnum::A].Parse(state.A());
			action |= ActionFlags[(uint8_t)ButtonEnum::B].Parse(state.B());
			action |= ActionFlags[(uint8_t)ButtonEnum::X].Parse(state.X());
			action |= ActionFlags[(uint8_t)ButtonEnum::Y].Parse(state.Y());

			action |= ActionFlags[(uint8_t)ButtonEnum::L1].Parse(state.L1());
			action |= ActionFlags[(uint8_t)ButtonEnum::R1].Parse(state.R1());
			action |= ActionFlags[(uint8_t)ButtonEnum::L3].Parse(state.L3());
			action |= ActionFlags[(uint8_t)ButtonEnum::R3].Parse(state.R3());

			action |= ActionFlags[(uint8_t)ButtonEnum::P1].Parse(state.P1());
			action |= ActionFlags[(uint8_t)ButtonEnum::P2].Parse(state.P2());

			action |= ActionFlags[(uint8_t)ButtonEnum::Start].Parse(state.Start());
			action |= ActionFlags[(uint8_t)ButtonEnum::Select].Parse(state.Select());
			action |= ActionFlags[(uint8_t)ButtonEnum::Home].Parse(state.Home());
			action |= ActionFlags[(uint8_t)ButtonEnum::Share].Parse(state.Share());
			action |= ActionFlags[(uint8_t)ButtonEnum::Share].Parse(state.Chat());

			action |= ActionDPad.Parse(state.DPad());

			action |= ActionJoy1.Parse(state.Joy1X(), state.Joy1Y());
			action |= ActionJoy2.Parse(state.Joy2X(), state.Joy2Y());

			action |= ActionL2.Parse(state.L2());
			action |= ActionR2.Parse(state.R2());

			return action;
		}
		else
		{
			Clear();

			return false;
		}
	}

	const bool Parse(button_pad_state_t& state)
	{
		if (state.Connected())
		{
			bool action = false;

			action |= ActionFlags[(uint8_t)ButtonEnum::A].Parse(state.A());
			action |= ActionFlags[(uint8_t)ButtonEnum::B].Parse(state.B());
			action |= ActionFlags[(uint8_t)ButtonEnum::X].Parse(state.X());
			action |= ActionFlags[(uint8_t)ButtonEnum::Y].Parse(state.Y());

			action |= ActionFlags[(uint8_t)ButtonEnum::L1].Parse(state.L1());
			action |= ActionFlags[(uint8_t)ButtonEnum::R1].Parse(state.R1());
			action |= ActionFlags[(uint8_t)ButtonEnum::L3].Parse(state.L3());
			action |= ActionFlags[(uint8_t)ButtonEnum::R3].Parse(state.R3());

			action |= ActionFlags[(uint8_t)ButtonEnum::P1].Parse(state.P1());
			action |= ActionFlags[(uint8_t)ButtonEnum::P2].Parse(state.P2());

			action |= ActionFlags[(uint8_t)ButtonEnum::Start].Parse(state.Start());
			action |= ActionFlags[(uint8_t)ButtonEnum::Select].Parse(state.Select());
			action |= ActionFlags[(uint8_t)ButtonEnum::Home].Parse(state.Home());
			action |= ActionFlags[(uint8_t)ButtonEnum::Share].Parse(state.Share());
			action |= ActionFlags[(uint8_t)ButtonEnum::Share].Parse(state.Chat());

			action |= ActionDPad.Parse(state.DPad());

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

	const bool Buttons() const
	{
		for (uint8_t i = 0; i < (uint8_t)ButtonEnum::EnumCount; i++)
		{
			if (ActionFlags[i].ActionDown())
			{
				return true;
			}
		}

		return false;
	}

	const bool A() const
	{
		return ActionFlags[(uint8_t)ButtonEnum::A].ActionDown();
	}

	const bool B() const
	{
		return ActionFlags[(uint8_t)ButtonEnum::B].ActionDown();
	}

	const bool X() const
	{
		return ActionFlags[(uint8_t)ButtonEnum::X].ActionDown();
	}

	const bool Y() const
	{
		return ActionFlags[(uint8_t)ButtonEnum::Y].ActionDown();
	}

	const bool L1() const
	{
		return ActionFlags[(uint8_t)ButtonEnum::L1].ActionDown();
	}

	const bool R1() const
	{
		return ActionFlags[(uint8_t)ButtonEnum::R1].ActionDown();
	}

	const bool L3() const
	{
		return ActionFlags[(uint8_t)ButtonEnum::L3].ActionDown();
	}

	const bool R3() const
	{
		return ActionFlags[(uint8_t)ButtonEnum::R3].ActionDown();
	}

	const bool P1() const
	{
		return ActionFlags[(uint8_t)ButtonEnum::P1].ActionDown();
	}

	const bool P2() const
	{
		return ActionFlags[(uint8_t)ButtonEnum::P2].ActionDown();
	}

	const bool Start() const
	{
		return ActionFlags[(uint8_t)ButtonEnum::Start].ActionDown();
	}

	const bool Select() const
	{
		return ActionFlags[(uint8_t)ButtonEnum::Select].ActionDown();
	}

	const bool Home() const
	{
		return ActionFlags[(uint8_t)ButtonEnum::Home].ActionDown();
	}

	const bool Share() const
	{
		return ActionFlags[(uint8_t)ButtonEnum::Share].ActionDown();
	}

	const bool Chat() const
	{
		return ActionFlags[(uint8_t)ButtonEnum::Chat].ActionDown();
	}
};

#endif

