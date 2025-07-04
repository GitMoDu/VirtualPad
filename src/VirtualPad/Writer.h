#ifndef _VIRTUAL_PAD_WRITER_h
#define _VIRTUAL_PAD_WRITER_h

#include "VirtualPad.h"

/// <summary>
/// Collection of helper functions to write to VirtualPad state.
/// </summary>
namespace VirtualPad
{
	/// <summary>
	/// Set Joy1 analog values directly from an 8 bit source.
	/// </summary>
	/// <param name="state">Virtual Pad state.</param>
	/// <param name="x">[INT8_MIN ; INT8_MAX]</param>
	/// <param name="y">[INT8_MIN ; INT8_MAX]</param>
	static void SetJoy1(analog_pad_state_t& state, const int8_t x, const int8_t y)
	{
		if (x >= 0)
		{
			state.joy1X = ((int32_t)x * INT16_MAX) / INT8_MAX;
		}
		else
		{
			state.joy1X = ((int32_t)x * INT16_MIN) / INT8_MIN;
		}
		if (y >= 0)
		{
			state.joy1Y = ((int32_t)y * INT16_MAX) / INT8_MAX;
		}
		else
		{
			state.joy1Y = ((int32_t)y * INT16_MIN) / INT8_MIN;
		}
	}

	/// <summary>
	/// Set Joy1 analog values from a unsigned source.
	/// </summary>
	/// <param name="state">Virtual Pad state.</param>
	/// <param name="x">[0 ; UINT16_MAX]</param>
	/// <param name="y">[0 ; UINT16_MAX]</param>
	static void SetJoy1(analog_pad_state_t& state, const uint16_t x, const uint16_t y)
	{
		if (x >= INT16_MAX)
		{
			state.joy1X = x - INT16_MAX;
		}
		else
		{
			state.joy1X = -((int16_t)INT16_MAX - (int16_t)x);
		}

		if (y >= INT16_MAX)
		{
			state.joy1Y = y - INT16_MAX;
		}
		else
		{
			state.joy1Y = -((int16_t)INT16_MAX - (int16_t)y);
		}
	}

	/// <summary>
	/// Set Joy2 analog values from a unsigned source.
	/// </summary>
	/// <param name="state">Virtual Pad state.</param>
	/// <param name="x">[0 ; UINT16_MAX]</param>
	/// <param name="y">[0 ; UINT16_MAX]</param>
	static void SetJoy2(analog_pad_state_t& state, const uint16_t x, const uint16_t y)
	{
		if (x >= INT16_MAX)
		{
			state.joy2X = x - INT16_MAX;
		}
		else
		{
			state.joy2X = -((int16_t)INT16_MAX - (int16_t)x);
		}

		if (y >= INT16_MAX)
		{
			state.joy2Y = y - INT16_MAX;
		}
		else
		{
			state.joy2Y = -((int16_t)INT16_MAX - (int16_t)y);
		}
	}

	/// <summary>
	/// Set Joy2 analog values directly from an 8 bit source.
	/// </summary>
	/// <param name="state">Virtual Pad state.</param>
	/// <param name="x">[INT8_MIN ; INT8_MAX]</param>
	/// <param name="y">[INT8_MIN ; INT8_MAX]</param>
	static void SetJoy2(analog_pad_state_t& state, const int8_t x, const int8_t y)
	{
		if (x >= 0)
		{
			state.joy2X = ((int32_t)x * INT16_MAX) / INT8_MAX;
		}
		else
		{
			state.joy2X = ((int32_t)x * INT16_MIN) / INT8_MIN;
		}
		if (y >= 0)
		{
			state.joy2Y = ((int32_t)y * INT16_MAX) / INT8_MAX;
		}
		else
		{
			state.joy2Y = ((int32_t)y * INT16_MIN) / INT8_MIN;
		}
	}

	/// <summary>3
	/// Set joy 2 analog values from digital source.
	/// Biased to the first button detected, if multiple directions are active.
	/// </summary>
	/// <param name="state">Virtual Pad state.</param>
	/// <param name="up">Is up pressed.</param>
	/// <param name="down">Is down pressed.</param>
	/// <param name="left">Is left pressed.</param>
	/// <param name="right">Is right pressed.</param>
	static void SetJoy2(analog_pad_state_t& state, const bool up, const bool down, const bool left, const bool right)
	{
		if (right)
		{
			state.joy2X = INT16_MAX;
		}
		else if (left)
		{
			state.joy2X = INT16_MIN;
		}
		else
		{
			state.joy2X = 0;
		}
		if (up)
		{
			state.joy2Y = INT16_MAX;
		}
		else if (down)
		{
			state.joy2Y = INT16_MIN;
		}
		else
		{
			state.joy2Y = 0;
		}
	}

	template<uint8_t AxisMin = 0, uint8_t AxisMax = UINT8_MAX>
	struct AxisUint8
	{
		static const uint16_t Map(const uint8_t value)
		{
			if (value <= AxisMin)
			{
				return 0;
			}
			else if (value >= AxisMax)
			{
				return UINT16_MAX;
			}
			else if (value == 0)
			{
				return 0;
			}
			else // value > 0
			{
				return (((uint32_t)(value - AxisMin) * UINT16_MAX) / (AxisMax - AxisMin));
			}
		}
	};

	template<int8_t xMin = -80, int8_t xMax = 80,
		int8_t yMin = -80, int8_t yMax = 80,
		int8_t xOffset = 0, int8_t yOffset = 0>
	class JoystickInt8
	{
	private:
		template<int8_t AxisMin, int8_t AxisMax, int8_t offset>
		static const int8_t LimitedOffset(const int8_t value)
		{
			if (offset == 0)
			{
				return value;
			}
			else if (offset > 0)
			{
				if (value >= (AxisMax - offset))
				{
					return AxisMax;
				}
				else
				{
					return value + offset;
				}
			}
			else // offset <0
			{
				if (value <= (AxisMin + offset))
				{
					return AxisMin;
				}
				else
				{
					return value + offset;
				}
			}
		}

		template<int8_t AxisMin, int8_t AxisMax>
		static const int16_t LimitedScale(const int8_t value)
		{
			if (value <= AxisMin)
			{
				return INT16_MIN;
			}
			else if (value >= AxisMax)
			{
				return INT16_MAX;
			}
			else if (value == 0)
			{
				return 0;
			}
			else if (value > 0)
			{
				return (((int32_t)value * INT16_MAX) / AxisMax);
			}
			else // value < 0
			{
				return ((((int32_t)value) * INT16_MIN) / AxisMin);
			}
		}

	public:
		static const int16_t MapX(const int8_t x)
		{
			return LimitedScale<xMin, xMax>(LimitedOffset<xMin, xMax, xOffset>(x));
		}

		static const int16_t MapY(const int8_t y)
		{
			return LimitedScale<yMin, yMax>(LimitedOffset<yMin, yMax, yOffset>(y));
		}

		static void Map(const int8_t x, const int8_t y, int16_t& xOut, int16_t& yOut)
		{
			xOut = MapX(x);
			yOut = MapY(y);
		}
	};
}
#endif