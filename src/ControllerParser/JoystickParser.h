// JoystickParser.h

#ifndef _JOYSTICK_PARSER_h
#define _JOYSTICK_PARSER_h

#include "../Model/DigitalButtons.h"

namespace JoystickParser
{
	/// <summary>
	/// Extracts a digital direction from an analog joystick.
	/// </summary>
	/// <typeparam name="ReferenceNegative"></typeparam>
	/// <typeparam name="ReferencePositive"></typeparam>
	template<int16_t ReferenceNegative = INT16_MIN / 2,
		int16_t ReferencePositive = INT16_MAX / 2>
	struct JoystickAnalogToDigital
	{
		/// <summary>
		/// </summary>
		/// <param name="x">Joystick X.</param>
		/// <param name="y">Joystick Y.</param>
		/// <returns>Direction state in DPad format.</returns>
		static const DPadEnum Parse(const int16_t x, const int16_t y)
		{
			if (y >= ReferencePositive)
			{
				if (x >= ReferencePositive)
				{
					return DPadEnum::UpRight;
				}
				else if (x <= ReferencePositive)
				{
					return DPadEnum::UpLeft;
				}
				else
				{
					return DPadEnum::Up;
				}
			}
			else if (y <= ReferencePositive)
			{
				if (x >= ReferencePositive)
				{
					return DPadEnum::DownRight;
				}
				else if (x <= ReferencePositive)
				{
					return DPadEnum::DownLeft;
				}
				else
				{
					return DPadEnum::Down;
				}
			}
			else if (x >= ReferencePositive)
			{
				return DPadEnum::Right;
			}
			else if (x <= ReferencePositive)
			{
				return DPadEnum::Left;
			}
			else
			{
				return DPadEnum::None;
			}
		}
	};

	/// <summary>
	/// Parses digital direction actions from an analog joystick.
	/// </summary>
	/// <typeparam name="ReferenceNegative"></typeparam>
	/// <typeparam name="ReferencePositive"></typeparam>
	/// <typeparam name="Hysteresis"></typeparam>
	template<int16_t ReferenceNegative = INT16_MIN / 2,
		int16_t ReferencePositive = INT16_MAX / 2,
		uint16_t Hysteresis = UINT16_MAX / 8>
	class JoystickAnalogToDigitalAction
	{
	private:
		enum class ActionEnum : uint8_t
		{
			Down = 0,
			Up = 1,
			Left = 2,
			Right = 3
		};

		enum class StateEnum : uint8_t
		{
			Negative = 0,
			Neutral = 1,
			Positive = 2
		};

	private:
		uint8_t Actions = 0;
		StateEnum StateX = StateEnum::Neutral;
		StateEnum StateY = StateEnum::Neutral;

	public:
		/// <summary>
		/// Returns true on up action.
		/// </summary>
		const bool ActionUp() const
		{
			return Actions & (1 << (uint8_t)ActionEnum::Up);
		}

		/// <summary>
		/// Returns true on down action.
		/// </summary>
		const bool ActionDown() const
		{
			return Actions & (1 << (uint8_t)ActionEnum::Down);
		}

		/// <summary>
		/// Returns true on left action.
		/// </summary>
		const bool ActionLeft() const
		{
			return Actions & (1 << (uint8_t)ActionEnum::Left);
		}

		/// <summary>
		/// Returns true on right action.
		/// </summary>
		const bool ActionRight() const
		{
			return Actions & (1 << (uint8_t)ActionEnum::Right);
		}

		void Parse(const int16_t x, const int16_t y)
		{
			Actions = 0;

			if (x >= ReferencePositive)
			{
				switch (StateX)
				{
				case StateEnum::Neutral:
					if (x >= (ReferencePositive + Hysteresis))
					{
						StateX = StateEnum::Positive;
						Actions |= (1 << (uint8_t)ActionEnum::Right);
					}
					break;
				case StateEnum::Negative:
					if (x >= (ReferencePositive + Hysteresis))
					{
						StateX = StateEnum::Positive;
						Actions |= (1 << (uint8_t)ActionEnum::Right);
					}
					else
					{
						StateX = StateEnum::Neutral;
					}
					break;
				case StateEnum::Positive:
				default:
					break;
				}
			}
			else if (x <= ReferenceNegative)
			{
				switch (StateX)
				{
				case StateEnum::Neutral:
					if (x <= (ReferenceNegative - Hysteresis))
					{
						StateX = StateEnum::Negative;
						Actions |= (1 << (uint8_t)ActionEnum::Left);
					}
					break;
				case StateEnum::Positive:
					if (x <= (ReferenceNegative - Hysteresis))
					{
						StateX = StateEnum::Negative;
						Actions |= (1 << (uint8_t)ActionEnum::Left);
					}
					else
					{
						StateX = StateEnum::Neutral;
					}
					break;
				case StateEnum::Negative:
				default:
					break;
				}
			}
			else
			{
				switch (StateX)
				{
				case StateEnum::Negative:
					if (x >= (ReferenceNegative + Hysteresis))
					{
						StateX = StateEnum::Neutral;
					}
					break;
				case StateEnum::Positive:
					if (x < (ReferencePositive - Hysteresis))
					{
						StateX = StateEnum::Neutral;
					}
					break;
				case StateEnum::Neutral:
				default:
					break;
				}
			}
		}

		void Clear()
		{
			Actions = 0;
			StateX = StateEnum::Neutral;
			StateY = StateEnum::Neutral;
		}

		const DPadEnum ActionDPad() const
		{
			if (ActionUp())
			{
				if (ActionLeft())
				{
					return DPadEnum::UpLeft;
				}
				else if (ActionRight())
				{
					return DPadEnum::UpRight;
				}
				else
				{
					return DPadEnum::Up;
				}
			}
			else if (ActionDown())
			{
				if (ActionLeft())
				{
					return DPadEnum::DownLeft;
				}
				else if (ActionRight())
				{
					return DPadEnum::DownRight;
				}
				else
				{
					return DPadEnum::Down;
				}
			}
			else if (ActionLeft())
			{
				return DPadEnum::Left;
			}
			else if (ActionRight())
			{
				return DPadEnum::Right;
			}
			else
			{
				return DPadEnum::None;
			}
		}
	};
}
#endif