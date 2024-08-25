// ButtonParser.h

#ifndef _BUTTON_PARSER_h
#define _BUTTON_PARSER_h

#include <stdint.h>

namespace ButtonParser
{
	/// <summary>
	/// Parses Up/Down actions.
	/// </summary>
	class Action
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
			State = false;
			Actions = 0;
		}

		/// <summary>
		/// True if any action is flagged.
		/// </summary>
		/// <returns></returns>
		const bool HasAction() const
		{
			return Actions != 0;
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
		const bool ActionDown() const
		{
			return Actions & (1 << (uint8_t)ActionEnum::Down);
		}

		/// <summary>
		/// Parse the current state of the button.
		/// Updates all active Actions.
		/// </summary>
		/// <param name="pressed"></param>
		/// <returns>True when parsing results in pending action.</returns>
		const bool Parse(const bool pressed)
		{
			Actions = 0;

			if (State)
			{
				if (!pressed)
				{
					State = false;
					Actions |= (1 << (uint8_t)ActionEnum::Up);

					return true;
				}
			}
			else
			{
				if (pressed)
				{
					State = true;
					Actions |= (1 << (uint8_t)ActionEnum::Down);

					return true;
				}
			}

			return false;
		}
	};

	/// <summary>
	/// Parses Up/Down actions.
	/// </summary>
	class ActionTimed
	{
	private:
		enum class ActionEnum : uint8_t
		{
			Down = 0,
			Up = 1
		};

	private:
		uint32_t DownTimestamp = 0;
		uint32_t DownElapsed = 0;
		uint8_t Actions = 0;
		bool State = false;

	public:
		void Clear()
		{
			State = false;
			Actions = 0;
			DownTimestamp = 0;
		}

		/// <summary>
		/// True if any action is flagged.
		/// </summary>
		/// <returns></returns>
		const bool HasAction() const
		{
			return Actions != 0;
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
		const bool ActionDown() const
		{
			return Actions & (1 << (uint8_t)ActionEnum::Down);
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="downDuration"></param>
		/// <returns>True on button down with at least downDuration elapsed.</returns>
		const bool ActionDown(const uint32_t downDuration) const
		{
			if (State)
			{
				return DownElapsed >= downDuration;
			}
			else
			{
				return false;
			}
		}

		/// <summary>
		/// </summary>
		/// <param name="holdDuration">Minimum duration to trigger an action.</param>
		/// <returns>True on action up with at least holdDuration elapsed.</returns>
		const bool ActionUp(const uint32_t holdDuration) const
		{
			if (ActionUp())
			{
				return DownElapsed >= holdDuration;
			}
			else
			{
				return false;
			}
		}

		/// <summary>
		/// Parse the current state of the button.
		/// Updates all active Actions.
		/// </summary>
		/// <param name="pressed"></param>
		void Parse(const uint32_t timestamp, const bool pressed)
		{
			Actions = 0;

			if (State)
			{
				if (pressed)
				{
					DownElapsed = timestamp - DownTimestamp;
				}
				else
				{
					State = false;
					Actions |= (1 << (uint8_t)ActionEnum::Up);
				}
			}
			else
			{
				if (pressed)
				{
					State = true;
					Actions |= (1 << (uint8_t)ActionEnum::Down);
					DownTimestamp = timestamp;
					DownElapsed = 0;
				}
			}
		}
	};

	/// <summary>
	/// Parses digital direction actions from digital direction source.
	/// </summary>
	class DirectionAction
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

		const bool Parse(const DPadEnum dpad)
		{
			switch (dpad)
			{
			case DPadEnum::None:
				return Parse(false, false, false, false);
			case DPadEnum::Up:
				return Parse(true, false, false, false);
			case DPadEnum::UpRight:
				return Parse(true, false, false, true);
			case DPadEnum::UpLeft:
				return Parse(true, false, true, false);
			case DPadEnum::Down:
				return Parse(false, true, false, false);
			case DPadEnum::DownLeft:
				return Parse(false, true, true, false);
			case DPadEnum::DownRight:
				return Parse(false, true, false, true);
			case DPadEnum::Left:
				return Parse(false, false, true, false);
			case DPadEnum::Right:
				return Parse(false, false, false, true);
			case DPadEnum::DPadEnumCount:
				break;
			default:
				break;
			}

			return false;
		}

		const bool Parse(const bool up, const bool down, const bool left, const bool right)
		{
			Actions = 0;
			bool action = false;

			if (right)
			{
				switch (StateX)
				{
				case StateEnum::Neutral:
				case StateEnum::Negative:
					StateX = StateEnum::Positive;
					Actions |= (1 << (uint8_t)ActionEnum::Right);
					action = true;
					break;
				case StateEnum::Positive:
				default:
					break;
				}
			}
			else if (left)
			{
				switch (StateX)
				{
				case StateEnum::Neutral:
				case StateEnum::Positive:
					StateX = StateEnum::Negative;
					Actions |= (1 << (uint8_t)ActionEnum::Left);
					action = true;
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
				case StateEnum::Positive:
					StateX = StateEnum::Neutral;
					break;
				case StateEnum::Neutral:
				default:
					break;
				}
			}

			if (up)
			{
				switch (StateY)
				{
				case StateEnum::Neutral:
				case StateEnum::Negative:
					StateY = StateEnum::Positive;
					Actions |= (1 << (uint8_t)ActionEnum::Up);
					action = true;
					break;
				case StateEnum::Positive:
				default:
					break;
				}
			}
			else if (down)
			{
				switch (StateY)
				{
				case StateEnum::Neutral:
				case StateEnum::Positive:
					StateY = StateEnum::Negative;
					Actions |= (1 << (uint8_t)ActionEnum::Down);
					action = true;
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
				case StateEnum::Positive:
					StateY = StateEnum::Neutral;
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
#endif