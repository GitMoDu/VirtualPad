// PadState.h

#ifndef _PAD_STATE_h
#define _PAD_STATE_h

#include <stdint.h>

/// <summary>
/// Abstract controller state. 
/// Modelled after RetroArch's RetroPad.
///			
/// Menu Buttons:
/// 
///	[Home]		[Share] 
/// [Select]	[Start]
///	
/// 
/// DPad:
/// 
///	[↖]	[↑] [↗]
///	[←]	 · 	[→]
///	[↙]	[↓]	[↘]			
/// 
/// 
/// Face Buttons:
/// 
///		[Y]
/// [X]		[B]
///		[A]
/// 
/// 
/// Joysticks 1 and 2, with respective L3 and R3:
/// 
///	[↖]	[↑]	[↗]		[↖]	[↑]	[↗]
///	[←]	L3	[→]		[←]	 R3	[→]
///	[↙]	[↓]	[↘]		[↙]	[↓]	[↘]	
/// 
/// 
/// Digital and Analog Triggers:
/// 
/// [L1]		[R1]
/// [L2]		[R2]
/// 
/// </summary>
struct PadState
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

	void CopyTo(PadState& controllerState)
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

	void CopyFrom(const PadState& controllerState)
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