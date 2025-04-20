#ifndef _VIRTUAL_PAD_ANALOG_PAD_h
#define _VIRTUAL_PAD_ANALOG_PAD_h

#include "ButtonPad.h"

namespace VirtualPad
{
	/// <summary>
	/// Expands on button_pad_state_t and adds 2 joysticks and 2 triggers.
	/// </summary>
	struct analog_pad_state_t : button_pad_state_t
	{
		int16_t joy1X = 0;
		int16_t joy1Y = 0;

		int16_t joy2X = 0;
		int16_t joy2Y = 0;

		uint16_t l2 = 0;
		uint16_t r2 = 0;

		int16_t Joy1X() const
		{
			return joy1X;
		}

		int16_t Joy1Y() const
		{
			return joy1Y;
		}

		int16_t Joy2X() const
		{
			return joy2X;
		}

		int16_t Joy2Y() const
		{
			return joy2Y;
		}

		uint16_t L2() const
		{
			return l2;
		}

		uint16_t R2() const
		{
			return r2;
		}
	};
}
#endif