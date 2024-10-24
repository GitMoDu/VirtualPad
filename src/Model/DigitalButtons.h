// DigitalButtons.h

#ifndef _DIGITAL_BUTTONS_h
#define _DIGITAL_BUTTONS_h

#include <stdint.h>

/// <summary>
/// Same codes as HID Gamepad.
/// See hid_gamepad_hat_t reference.
/// </summary>
enum class DPadEnum : uint8_t
{
	None = 0,
	Up = 1,
	UpRight = 2,
	Right = 3,
	DownRight = 4,
	Down = 5,
	DownLeft = 6,
	Left = 7,
	UpLeft = 8,
	DPadEnumCount
};

enum class MainButtonEnum : uint8_t
{
	A = 0,
	B = 1,
	X = 2,
	Y = 3,
	L1 = 4,
	R1 = 5,
	L3 = 6,
	R3 = 7,
	MainButtonEnumCount
};

enum class MenuButtonEnum : uint8_t
{
	Start = 0,
	Select = 1,
	Home = 2,
	Share = 3,
	MenuButtonEnumCount
};

#endif