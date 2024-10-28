// virtual_pad_state_t.h

#ifndef _VIRTUAL_PAD_STATE_h
#define _VIRTUAL_PAD_STATE_h

#include <stdint.h>
#include "Model/DigitalButtons.h"

/// <summary>
/// Virtual controller state data struct.
/// </summary>
struct virtual_pad_state_t
{
	int16_t Joy1X = 0;
	int16_t Joy1Y = 0;

	int16_t Joy2X = 0;
	int16_t Joy2Y = 0;

	uint16_t L2 = 0;
	uint16_t R2 = 0;

	// Single value state.
	DPadEnum DPad = DPadEnum::None;

	// Up to 8 main buttons.
	uint8_t MainButtons = 0;

	// Up to 8 menu buttons.
	uint8_t MenuButtons = 0;

	bool Connected = false;

	void Clear()
	{
		Joy1X = 0;
		Joy1Y = 0;
		Joy2X = 0;
		Joy2Y = 0;

		L2 = 0;
		R2 = 0;

		DPad = DPadEnum::None;
		MainButtons = 0;
		MenuButtons = 0;

		Connected = false;
	}

	void CopyTo(virtual_pad_state_t& controllerState)
	{
		controllerState.Joy1X = Joy1X;
		controllerState.Joy1Y = Joy1Y;
		controllerState.Joy2X = Joy2X;
		controllerState.Joy2Y = Joy2Y;

		controllerState.L2 = L2;
		controllerState.R2 = R2;

		controllerState.DPad = DPad;
		controllerState.MainButtons = MainButtons;
		controllerState.MenuButtons = MenuButtons;
		controllerState.Connected = Connected;
	}

	void CopyFrom(const virtual_pad_state_t& controllerState)
	{
		Joy1X = controllerState.Joy1X;
		Joy1Y = controllerState.Joy1Y;
		Joy2X = controllerState.Joy2X;
		Joy2Y = controllerState.Joy2Y;

		L2 = controllerState.L2;
		R2 = controllerState.R2;

		DPad = controllerState.DPad;
		MainButtons = controllerState.MainButtons;
		MenuButtons = controllerState.MenuButtons;
		Connected = controllerState.Connected;
	}
};

#endif