#ifndef _VIRTUAL_PAD_BUTTON_PAD_h
#define _VIRTUAL_PAD_BUTTON_PAD_h

#include "Mask.h"

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
	enum class FlagEnum : uint8_t
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
		Connected = 15,
		EnumCount
	};

	/// <summary>
	/// Basic controller state with only digital inputs.
	/// </summary>
	struct button_pad_state_t
	{
		// 8 main buttons, 4 menu buttons, 1 connected flag and 3 free slots.
		uint16_t flags = 0;

		// Single value represents all possible DPad states.
		DPadEnum dPad = DPadEnum::None;

		bool Connected() const
		{
			return FlagMask::GetFlag<uint8_t(FlagEnum::Connected)>(flags);
		}

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
			return FlagMask::GetFlag<uint8_t(FlagEnum::A)>(flags);
		}

		bool B() const
		{
			return FlagMask::GetFlag<uint8_t(FlagEnum::B)>(flags);
		}

		bool X() const
		{
			return FlagMask::GetFlag<uint8_t(FlagEnum::X)>(flags);
		}

		bool Y() const
		{
			return FlagMask::GetFlag<uint8_t(FlagEnum::Y)>(flags);
		}

		bool L1() const
		{
			return FlagMask::GetFlag<uint8_t(FlagEnum::L1)>(flags);
		}

		bool R1() const
		{
			return FlagMask::GetFlag<uint8_t(FlagEnum::R1)>(flags);
		}

		bool L3() const
		{
			return FlagMask::GetFlag<uint8_t(FlagEnum::L3)>(flags);
		}

		bool R3() const
		{
			return FlagMask::GetFlag<uint8_t(FlagEnum::R3)>(flags);
		}

		bool P1() const
		{
			return FlagMask::GetFlag<uint8_t(FlagEnum::P1)>(flags);
		}

		bool P2() const
		{
			return FlagMask::GetFlag<uint8_t(FlagEnum::P2)>(flags);
		}

		bool Start() const
		{
			return FlagMask::GetFlag<uint8_t(FlagEnum::Start)>(flags);
		}

		bool Select() const
		{
			return FlagMask::GetFlag<uint8_t(FlagEnum::Select)>(flags);
		}

		bool Home() const
		{
			return FlagMask::GetFlag<uint8_t(FlagEnum::Home)>(flags);
		}

		bool Share() const
		{
			return FlagMask::GetFlag<uint8_t(FlagEnum::Share)>(flags);
		}

		bool Chat() const
		{
			return FlagMask::GetFlag<uint8_t(FlagEnum::Chat)>(flags);
		}
	};
}

#endif