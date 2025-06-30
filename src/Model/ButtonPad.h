#ifndef _VIRTUAL_PAD_BUTTON_PAD_h
#define _VIRTUAL_PAD_BUTTON_PAD_h

#include "InfoPad.h"

namespace VirtualPad
{
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

	/// <summary>
	/// Pad and all buttons state are stored in a single 16 bit value.
	/// Use of flags is slower than boolean fields, but uses way less RAM and speeds up state copies significantly.
	/// </summary>
	enum class ButtonEnum : uint8_t
	{
		A = 0,
		B = 1,
		X = 2,
		Y = 3,
		L1 = 4,
		R1 = 5,
		L3 = 6,
		R3 = 7,
		P1 = 8,
		P2 = 9,
		Chat = 10,
		Home = 11,
		Share = 12,
		Select = 13,
		Start = 14,
		EnumCount
	};


	/// <summary>
	/// Basic controller state with only digital inputs.
	/// </summary>
	struct button_pad_state_t : info_pad_state_t
	{
		// 8 main buttons, 4 menu buttons, 2 extra buttons and 1 free slot.
		uint16_t buttons = 0;

		// Single value represents all possible DPad states.
		DPadEnum dPad = DPadEnum::None;

	public:// Read API.
		DPadEnum DPad() const
		{
			return dPad;
		}

		bool DPadUp() const
		{
			return dPad == DPadEnum::Up || dPad == DPadEnum::UpLeft || dPad == DPadEnum::UpRight;
		}

		bool DPadDown() const
		{
			return dPad == DPadEnum::Down || dPad == DPadEnum::DownLeft || dPad == DPadEnum::DownRight;
		}

		bool DPadLeft() const
		{
			return dPad == DPadEnum::Left || dPad == DPadEnum::DownLeft || dPad == DPadEnum::UpLeft;
		}

		bool DPadRight() const
		{
			return dPad == DPadEnum::Right || dPad == DPadEnum::DownRight || dPad == DPadEnum::UpRight;
		}

		bool A() const
		{
			return FlagMask::GetFlag<uint8_t(ButtonEnum::A)>(buttons);
		}

		bool B() const
		{
			return FlagMask::GetFlag<uint8_t(ButtonEnum::B)>(buttons);
		}

		bool X() const
		{
			return FlagMask::GetFlag<uint8_t(ButtonEnum::X)>(buttons);
		}

		bool Y() const
		{
			return FlagMask::GetFlag<uint8_t(ButtonEnum::Y)>(buttons);
		}

		bool L1() const
		{
			return FlagMask::GetFlag<uint8_t(ButtonEnum::L1)>(buttons);
		}

		bool R1() const
		{
			return FlagMask::GetFlag<uint8_t(ButtonEnum::R1)>(buttons);
		}

		bool L3() const
		{
			return FlagMask::GetFlag<uint8_t(ButtonEnum::L3)>(buttons);
		}

		bool R3() const
		{
			return FlagMask::GetFlag<uint8_t(ButtonEnum::R3)>(buttons);
		}

		bool P1() const
		{
			return FlagMask::GetFlag<uint8_t(ButtonEnum::P1)>(buttons);
		}

		bool P2() const
		{
			return FlagMask::GetFlag<uint8_t(ButtonEnum::P2)>(buttons);
		}

		bool Start() const
		{
			return FlagMask::GetFlag<uint8_t(ButtonEnum::Start)>(buttons);
		}

		bool Select() const
		{
			return FlagMask::GetFlag<uint8_t(ButtonEnum::Select)>(buttons);
		}

		bool Home() const
		{
			return FlagMask::GetFlag<uint8_t(ButtonEnum::Home)>(buttons);
		}

		bool Share() const
		{
			return FlagMask::GetFlag<uint8_t(ButtonEnum::Share)>(buttons);
		}

		bool Chat() const
		{
			return FlagMask::GetFlag<uint8_t(ButtonEnum::Chat)>(buttons);
		}

	public:// Write API.
		void SetDPad(const DPadEnum value)
		{
			dPad = value;
		}

		/// <summary>
		/// Sets the D-Pad direction based on the individual button states.
		/// Biased to the first button detected, if multiple directions are active.
		/// </summary>
		/// <param name="state">Virtual Pad state.</param>
		/// <param name="up">Is up pressed.</param>
		/// <param name="down">Is down pressed.</param>
		/// <param name="left">Is left pressed.</param>
		/// <param name="right">Is right pressed.</param>
		void SetDPad(const bool up, const bool down, const bool left, const bool right)
		{
			if (up)
			{
				if (left)
				{
					dPad = DPadEnum::UpLeft;
				}
				else if (right)
				{
					dPad = DPadEnum::UpRight;
				}
				else
				{
					dPad = DPadEnum::Up;
				}
			}
			else if (down)
			{
				if (left)
				{
					dPad = DPadEnum::DownLeft;
				}
				else if (right)
				{
					dPad = DPadEnum::DownRight;
				}
				else
				{
					dPad = DPadEnum::Down;
				}
			}
			else if (left)
			{
				dPad = DPadEnum::Left;
			}
			else if (right)
			{
				dPad = DPadEnum::Right;
			}
			else
			{
				dPad = DPadEnum::None;
			}
		}

		void SetA(const bool value)
		{
			FlagMask::SetFlag<uint8_t(ButtonEnum::A)>(buttons, value);
		}

		void SetB(const bool value)
		{
			FlagMask::SetFlag<uint8_t(ButtonEnum::B)>(buttons, value);
		}

		void SetX(const bool value)
		{
			FlagMask::SetFlag<uint8_t(ButtonEnum::X)>(buttons, value);
		}

		void SetY(const bool value)
		{
			FlagMask::SetFlag<uint8_t(ButtonEnum::Y)>(buttons, value);
		}

		void SetL1(const bool value)
		{
			FlagMask::SetFlag<uint8_t(ButtonEnum::L1)>(buttons, value);
		}

		void SetR1(const bool value)
		{
			FlagMask::SetFlag<uint8_t(ButtonEnum::R1)>(buttons, value);
		}

		void SetL3(const bool value)
		{
			FlagMask::SetFlag<uint8_t(ButtonEnum::L3)>(buttons, value);
		}

		void SetR3(const bool value)
		{
			FlagMask::SetFlag<uint8_t(ButtonEnum::R3)>(buttons, value);
		}

		void SetP1(const bool value)
		{
			FlagMask::SetFlag<uint8_t(ButtonEnum::P1)>(buttons, value);
		}

		void SetP2(const bool value)
		{
			FlagMask::SetFlag<uint8_t(ButtonEnum::P2)>(buttons, value);
		}

		void SetChat(const bool value)
		{
			FlagMask::SetFlag<uint8_t(ButtonEnum::Chat)>(buttons, value);
		}

		void SetHome(const bool value)
		{
			FlagMask::SetFlag<uint8_t(ButtonEnum::Home)>(buttons, value);
		}

		void SetShare(const bool value)
		{
			FlagMask::SetFlag<uint8_t(ButtonEnum::Share)>(buttons, value);
		}

		void SetSelect(const bool value)
		{
			FlagMask::SetFlag<uint8_t(ButtonEnum::Select)>(buttons, value);
		}

		void SetStart(const bool value)
		{
			FlagMask::SetFlag<uint8_t(ButtonEnum::Start)>(buttons, value);
		}
	};
}

#endif