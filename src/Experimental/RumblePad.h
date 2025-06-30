#ifndef _VIRTUAL_PAD_RUMBLE_PAD_h
#define _VIRTUAL_PAD_RUMBLE_PAD_h

#include <stdint.h>

namespace VirtualPad
{
	/// <summary>
	/// Rumble state extends any base pad state.
	/// </summary>
	/// <typeparam name="base_pad_state_t"></typeparam>
	template<typename base_pad_state_t>
	struct rumble_pad_extension_t : base_pad_state_t
	{
		uint8_t rumbleLow = 0;
		uint8_t rumbleHigh = 0;

		uint8_t rumbleLeft = 0;
		uint8_t rumbleRight = 0;

		uint8_t RumbleLow() const
		{
			return rumbleLow;
		}

		uint8_t RumbleHigh() const
		{
			return rumbleHigh;
		}

		uint8_t RumbleLeft() const
		{
			return rumbleLeft;
		}

		uint8_t RumbleRight() const
		{
			return rumbleRight;
		}
	};

	template<typename base_pad_state_t>
	struct lights_pad_extension_t : base_pad_state_t
	{
	};
}
#endif