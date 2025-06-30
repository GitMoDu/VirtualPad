#ifndef _VIRTUAL_PAD_CUSTOM_FLIGHT_STICK_h
#define _VIRTUAL_PAD_CUSTOM_FLIGHT_STICK_h

#include "../Model/VirtualPad.h"

namespace VirtualPad
{
	namespace CustomFlightStick
	{
		enum class ExtraFlagEnum : uint8_t
		{
			FlapsUp,
			FlapsDown,
			EngineOn,
			EngineOff,
			MainButtonEnumCount
		};

		/// <summary>
		/// Expands on analog_pad_state_t and adds custom fields.
		/// </summary>
		struct custom_flight_pad_state_t : analog_pad_state_t
		{
			uint16_t extraFlags = 0;

			uint16_t throttle = 0;
			uint16_t brake = 0;
			uint16_t airBrake = 0;
			int16_t yaw = 0;

			DPadEnum pov1 = DPadEnum::None;
			DPadEnum pov2 = DPadEnum::None;

			bool EngineOn() const
			{
				return FlagMask::GetFlag<uint8_t(ExtraFlagEnum::EngineOn)>(extraFlags);
			}

			bool EngineOff() const
			{
				return FlagMask::GetFlag<uint8_t(ExtraFlagEnum::EngineOff)>(extraFlags);
			}
		};

		template<uint32_t configurationCode>
		using TemplateCustomFlightStick = TemplateVirtualPad<custom_flight_pad_state_t, configurationCode>;

		static void SetEngineOn(CustomFlightStick::custom_flight_pad_state_t& state, const bool value)
		{
			FlagMask::SetFlag<uint8_t(CustomFlightStick::ExtraFlagEnum::EngineOn)>(state.extraFlags, value);
		}

		static void SetEngineOff(CustomFlightStick::custom_flight_pad_state_t& state, const bool value)
		{
			FlagMask::SetFlag<uint8_t(CustomFlightStick::ExtraFlagEnum::EngineOff)>(state.extraFlags, value);
		}
	}
}
#endif