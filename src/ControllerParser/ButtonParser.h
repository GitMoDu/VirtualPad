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
		void Parse(const bool pressed)
		{
			Actions = 0;

			if (State)
			{
				if (!pressed)
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
				}
			}
		}
	};

	/// <summary>
	/// Parses Up/Down actions.
	/// </summary>
	class ButtonActionTimed
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

		void Clear()
		{
			State = false;
			Actions = 0;
			DownTimestamp = 0;
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
		/// </summary>
		/// <param name="holdDuration">Minimum duration to trigger an action.</param>
		/// <returns>True on action up with at least holdDuration elapsed.</returns>
		const bool ActionUp(const uint32_t holdDuration) const
		{
			if (Actions & (1 << (uint8_t)ActionEnum::Up))
			{
				return DownElapsed > holdDuration;
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
				}
			}
		}
	};
}
#endif