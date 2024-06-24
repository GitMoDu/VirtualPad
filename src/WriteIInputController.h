// WriteIInputController.h

#ifndef _WRITE_I_INPUT_CONTROLLER_h
#define _WRITE_I_INPUT_CONTROLLER_h

#include "IInputController.h"

/// <summary>
/// IInputController write interface.
/// </summary>
class WriteInputController : public IInputController
{
private:
	template<const MainButtonEnum button>
	void SetMainButton(const bool value)
	{
		if (value)
		{
			State.MainButtons |= ButtonMask<(uint8_t)button>();
		}
		else
		{
			State.MainButtons &= ~ButtonMask<(uint8_t)button>();
		}
	}

	template<const MenuButtonEnum button>
	void SetMenuButton(const bool value)
	{
		if (value)
		{
			State.MenuButtons |= ButtonMask<(uint8_t)button>();
		}
		else
		{
			State.MenuButtons &= ~ButtonMask<(uint8_t)button>();
		}
	}

public:
	WriteInputController(const uint16_t features = 0)
		: IInputController(features)
	{}

public:
	void SetJoy1(const int8_t x, const int8_t y)
	{
		if (x >= 0)
		{
			State.Joy1X = ((int32_t)x * INT16_MAX) / INT8_MAX;
		}
		else
		{
			State.Joy1X = ((int32_t)x * INT16_MIN) / INT8_MIN;
		}
		if (y >= 0)
		{
			State.Joy1Y = ((int32_t)y * INT16_MAX) / INT8_MAX;
		}
		else
		{
			State.Joy1Y = ((int32_t)y * INT16_MIN) / INT8_MIN;
		}
	}

	void SetJoy1(const int16_t x, const int16_t y)
	{
		State.Joy1X = x;
		State.Joy1Y = y;
	}

	void SetJoy1(const uint16_t x, const uint16_t y)
	{
		if (x >= INT16_MAX)
		{
			State.Joy1X = x - INT16_MAX;
		}
		else
		{
			State.Joy1X = -((int16_t)INT16_MAX - (int16_t)x);
		}

		if (y >= INT16_MAX)
		{
			State.Joy1Y = y - INT16_MAX;
		}
		else
		{
			State.Joy1Y = -((int16_t)INT16_MAX - (int16_t)y);
		}
	}

	void SetJoy2(const int8_t x, const int8_t y)
	{
		if (x >= 0)
		{
			State.Joy2X = ((int32_t)x * INT16_MAX) / INT8_MAX;
		}
		else
		{
			State.Joy2X = ((int32_t)x * INT16_MIN) / INT8_MIN;
		}
		if (y >= 0)
		{
			State.Joy2Y = ((int32_t)y * INT16_MAX) / INT8_MAX;
		}
		else
		{
			State.Joy2Y = ((int32_t)y * INT16_MIN) / INT8_MIN;
		}
	}

	void SetJoy2(const int16_t x, const int16_t y)
	{
		State.Joy2X = x;
		State.Joy2Y = y;
	}

	void SetJoy2(const uint16_t x, const uint16_t y)
	{
		if (x >= INT16_MAX)
		{
			State.Joy2X = x - INT16_MAX;
		}
		else
		{
			State.Joy2X = -((int16_t)INT16_MAX - (int16_t)x);
		}

		if (y >= INT16_MAX)
		{
			State.Joy2Y = y - INT16_MAX;
		}
		else
		{
			State.Joy2Y = -((int16_t)INT16_MAX - (int16_t)y);
		}
	}

	/// <summary>
	/// Set joy 2 analog values from digital source.
	/// </summary>
	/// <param name="up"></param>
	/// <param name="down"></param>
	/// <param name="left"></param>
	/// <param name="right"></param>
	void SetJoy2(const bool up, const bool down, const bool left, const bool right)
	{
		if (right)
		{
			State.Joy2X = INT16_MAX;
		}
		else if (left)
		{
			State.Joy2X = INT16_MIN;
		}
		else
		{
			State.Joy2X = 0;
		}
		if (up)
		{
			State.Joy2Y = INT16_MAX;
		}
		else if (down)
		{
			State.Joy2Y = INT16_MIN;
		}
		else
		{
			State.Joy2Y = 0;
		}
	}

	void SetL2(const uint16_t value)
	{
		State.L2 = value;
	}

	void SetR2(const uint16_t value)
	{
		State.R2 = value;
	}

	/// <summary>
	/// Sets the D-Pad direction based on the individual button states.
	/// Biased to the first button detected, if multiple directions are active.
	/// </summary>
	/// <param name="up"></param>
	/// <param name="down"></param>
	/// <param name="left"></param>
	/// <param name="right"></param>
	void SetDPad(const bool up, const bool down, const bool left, const bool right)
	{
		if (up)
		{
			if (left)
			{
				State.DPad = DPadEnum::UpLeft;
			}
			else if (right)
			{
				State.DPad = DPadEnum::UpRight;
			}
			else
			{
				State.DPad = DPadEnum::Up;
			}
		}
		else if (down)
		{
			if (left)
			{
				State.DPad = DPadEnum::DownLeft;
			}
			else if (right)
			{
				State.DPad = DPadEnum::DownRight;
			}
			else
			{
				State.DPad = DPadEnum::Down;
			}
		}
		else if (left)
		{
			State.DPad = DPadEnum::Left;			
		}
		else if (right)
		{
			State.DPad = DPadEnum::Right;
		}
		else
		{
			State.DPad = DPadEnum::None;
		}
	}

	void SetA(const bool value)
	{
		SetMainButton<MainButtonEnum::A>(value);
	}

	void SetB(const bool value)
	{
		SetMainButton<MainButtonEnum::B>(value);
	}

	void SetX(const bool value)
	{
		SetMainButton<MainButtonEnum::X>(value);
	}

	void SetY(const bool value)
	{
		SetMainButton<MainButtonEnum::Y>(value);
	}

	void SetL1(const bool value)
	{
		SetMainButton<MainButtonEnum::L1>(value);
	}

	void SetL3(const bool value)
	{
		SetMainButton<MainButtonEnum::L3>(value);
	}

	void SetR1(const bool value)
	{
		SetMainButton<MainButtonEnum::R1>(value);
	}

	void SetR3(const bool value)
	{
		SetMainButton<MainButtonEnum::R3>(value);
	}

	void SetStart(const bool value)
	{
		SetMenuButton<MenuButtonEnum::Start>(value);
	}

	void SetSelect(const bool value)
	{
		SetMenuButton<MenuButtonEnum::Select>(value);
	}

	void SetHome(const bool value)
	{
		SetMenuButton<MenuButtonEnum::Home>(value);
	}

	void SetShare(const bool value)
	{
		SetMenuButton<MenuButtonEnum::Share>(value);
	}
};

#endif