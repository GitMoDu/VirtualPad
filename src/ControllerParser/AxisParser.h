// AxisParser.h

#ifndef _AXIS_PARSER_h
#define _AXIS_PARSER_h

#include <stdint.h>

namespace AxisParser
{
	/// <summary>
	/// Extracts a digital output from an analog slider.
	/// </summary>
	/// <typeparam name="Reference"></typeparam>
	template<uint16_t Reference = INT16_MAX>
	struct AxisAnalogToDigital
	{
		static const bool Parse(const uint16_t value)
		{
			return value >= Reference;
		}
	};

	/// <summary>
	/// Parses Up/Down actions from an analog slider.
	/// </summary>
	/// <typeparam name="Reference"></typeparam>
	/// <typeparam name="Hysteresis"></typeparam>
	template<uint16_t Reference = UINT16_MAX / 2,
		uint16_t Hysteresis = UINT16_MAX / 8>
	class AxisAnalogToDigitalAction
	{
	private:
		enum class ActionEnum : uint8_t
		{
			Down = 0,
			Up = 1
		};

	private:
		uint8_t Actions = 0;
		bool State = false;

	public:
		void Clear()
		{
			Actions = 0;
			State = false;
		}

		/// <summary>
		/// Returns true on button up action.
		/// </summary>
		const bool ActionUp() const
		{
			return Actions & (1 << (uint8_t)ActionEnum::Up);
		}

		/// <summary>
		/// Returns true on button down action.
		/// </summary>
		const bool ActionDown()
		{
			return Actions & (1 << (uint8_t)ActionEnum::Down);
		}

		void Parse(const uint16_t value)
		{
			Actions = 0;

			if (value >= Reference)
			{
				if (!State
					&& value >= (Reference + Hysteresis))
				{
					State = true;
					Actions |= (1 << (uint8_t)ActionEnum::Up);
				}
			}
			else
			{
				if (State
					&& value <= (Reference - Hysteresis))
				{
					State = false;
					Actions |= (1 << (uint8_t)ActionEnum::Down);
				}
			}
		}
	};
}
#endif