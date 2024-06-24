// IInputMapper.h

#ifndef _I_INPUT_MAPPER_h
#define _I_INPUT_MAPPER_h

#include <stdint.h>

namespace IInputMapper
{
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