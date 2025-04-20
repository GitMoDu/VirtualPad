#ifndef _VIRTUAL_PAD_JOYSTICK_PARSER_h
#define _VIRTUAL_PAD_JOYSTICK_PARSER_h

#include "../VirtualPad/VirtualPad.h"

namespace VirtualPad
{
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
			uint16_t Hysteresis = UINT16_MAX / 16>
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
			/// True if any action is flagged.
			/// </summary>
			/// <returns></returns>
			const bool HasAction() const
			{
				return Actions != 0;
			}

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

			const bool Parse(const int16_t x, const int16_t y)
			{
				Actions = 0;
				bool action = false;

				if (x >= ReferencePositive)
				{
					switch (StateX)
					{
					case StateEnum::Neutral:
						if (x >= (int16_t)(ReferencePositive + Hysteresis))
						{
							StateX = StateEnum::Positive;
							Actions |= (1 << (uint8_t)ActionEnum::Right);
							action = true;
						}
						break;
					case StateEnum::Negative:
						if (x >= (int16_t)(ReferencePositive + Hysteresis))
						{
							StateX = StateEnum::Positive;
							Actions |= (1 << (uint8_t)ActionEnum::Right);
							action = true;
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
						if (x <= (int16_t)(ReferenceNegative - Hysteresis))
						{
							StateX = StateEnum::Negative;
							Actions |= (1 << (uint8_t)ActionEnum::Left);
							action = true;
						}
						break;
					case StateEnum::Positive:
						if (x <= (int16_t)(ReferenceNegative - Hysteresis))
						{
							StateX = StateEnum::Negative;
							Actions |= (1 << (uint8_t)ActionEnum::Left);
							action = true;
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
						if (x >= (int16_t)(ReferenceNegative + Hysteresis))
						{
							StateX = StateEnum::Neutral;
						}
						break;
					case StateEnum::Positive:
						if (x < (int16_t)(ReferencePositive - Hysteresis))
						{
							StateX = StateEnum::Neutral;
						}
						break;
					case StateEnum::Neutral:
					default:
						break;
					}
				}

				if (y >= ReferencePositive)
				{
					switch (StateY)
					{
					case StateEnum::Neutral:
						if (y >= (int16_t)(ReferencePositive + Hysteresis))
						{
							StateY = StateEnum::Positive;
							Actions |= (1 << (uint8_t)ActionEnum::Up);
							action = true;
						}
						break;
					case StateEnum::Negative:
						if (y >= (int16_t)(ReferencePositive + Hysteresis))
						{
							StateY = StateEnum::Positive;
							Actions |= (1 << (uint8_t)ActionEnum::Up);
							action = true;
						}
						else
						{
							StateY = StateEnum::Neutral;
						}
						break;
					case StateEnum::Positive:
					default:
						break;
					}
				}
				else if (y <= ReferenceNegative)
				{
					switch (StateY)
					{
					case StateEnum::Neutral:
						if (y <= (int16_t)(ReferenceNegative - Hysteresis))
						{
							StateY = StateEnum::Negative;
							Actions |= (1 << (uint8_t)ActionEnum::Down);
							action = true;
						}
						break;
					case StateEnum::Positive:
						if (y <= (int16_t)(ReferenceNegative - Hysteresis))
						{
							StateY = StateEnum::Negative;
							Actions |= (1 << (uint8_t)ActionEnum::Down);
							action = true;
						}
						else
						{
							StateY = StateEnum::Neutral;
						}
						break;
					case StateEnum::Negative:
					default:
						break;
					}
				}
				else
				{
					switch (StateY)
					{
					case StateEnum::Negative:
						if (y >= (int16_t)(ReferenceNegative + Hysteresis))
						{
							StateY = StateEnum::Neutral;
						}
						break;
					case StateEnum::Positive:
						if (y < (int16_t)(ReferencePositive - Hysteresis))
						{
							StateY = StateEnum::Neutral;
						}
						break;
					case StateEnum::Neutral:
					default:
						break;
					}
				}

				return action;
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
}
#endif