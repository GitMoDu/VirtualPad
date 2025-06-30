#ifndef _VIRTUAL_PAD_FEEDBACK_h
#define _VIRTUAL_PAD_FEEDBACK_h

#include "../VirtualPad/VirtualPad.h"

namespace VirtualPad
{
	namespace Feedback
	{
		namespace Lights
		{

		}

		namespace Force
		{
			struct pad_force_t
			{
				int16_t joy1X;
				int16_t joy1Y;

				int16_t joy2X;
				int16_t joy2Y;

				int16_t l2;
				int16_t r2;
			};
		}

		namespace Rumble
		{
			struct pad_rumble_t
			{
				uint16_t RumbleL;
				uint16_t RumbleR;

				uint16_t RumbleTL;
				uint16_t RumbleTR;

				uint16_t Mono()
				{
					return (uint32_t(RumbleL) + RumbleR) >> 1;
				}
			};

			struct haptic_t
			{
				uint32_t Elapsed;
				uint16_t Power;
			};

			struct haptic_state_t
			{
				uint32_t StartTimestamp;
				uint16_t Power;

				uint32_t Elapsed(const uint32_t timestamp) const
				{
					return timestamp - StartTimestamp;
				}
			};

			template<uint8_t channelCount>
			struct pad_haptic_t : pad_rumble_t
			{
				haptic_state_t Channels[channelCount];

				/// <summary>
				/// Get and haptic status for a channel index.
				/// </summary>
				/// <param name="timestamp">Current timestamp.</param>
				/// <param name="index">Channel index.</param>
				/// <returns></returns>
				haptic_t Channel(const uint32_t timestamp, const uint8_t index)
				{
					if (index < channelCount
						&& Channels[index]->Power > 0)
					{
						return haptic_t{ Channels[index]->Elapsed(timestamp), Channels[index]->power };
					}
					return haptic_t{ 0,0 };
				}

				/// <summary>
				/// Set a haptic playback channel status.
				/// </summary>
				/// <param name="index">Channel index.</param>
				/// <param name="timestamp">Current timestamp.</param>
				/// <param name="power">Channel power. Set power to 0 to disable.</param>
				void Set(const uint8_t index, const uint32_t timestamp, const uint8_t power)
				{
					if (index < channelCount)
					{
						if (power > 0)
						{
							Channels[index]->Power = power;
							Channels[index]->StartTimestamp = timestamp;
						}
						else
						{
							Channels[index]->Power = 0;
						}
					}
				}
			};

			using pad_haptic_8_t = pad_haptic_t<8>;
			using pad_haptic_16_t = pad_haptic_t<16>;
			using pad_haptic_32_t = pad_haptic_t<32>;

		}
	}
}
#endif