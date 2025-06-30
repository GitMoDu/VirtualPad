#ifndef _VIRTUAL_PAD_INFO_PAD_h
#define _VIRTUAL_PAD_INFO_PAD_h

#include "Mask.h"

namespace VirtualPad
{
	enum class PadStateEnum : uint8_t
	{
		Connected = 0,
		BatteryInfoAvailable = 1,
		Charging = 2,
		ConnectionQuality0 = 6,
		ConnectionQuality1 = 7,
		Battery0 = 8,
		Battery1 = 9,
		Battery2 = 10,
		Battery3 = 11,
		Battery4 = 12,
		Battery5 = 13,
		Battery6 = 14,
		Battery7 = 15,
		EnumCount
	};

	enum class ConnectionQualityEnum : uint8_t
	{
		Low = 0,
		Medium = 1,
		High = 2,
		Excellent = 3,
		EnumMax = Excellent
	};

	enum class BatteryChargeEnum : uint8_t
	{
		NoCharge = 0,
		FullCharge = UINT8_MAX
	};

	/// <summary>
	/// Basic controller for connection and battery state.
	/// </summary>
	struct info_pad_state_t
	{
		// Mixed container of flags and scales.
		uint16_t flags = 0;

	public:// Read API.
		bool Connected() const
		{
			return FlagMask::GetFlag<uint8_t(PadStateEnum::Connected)>(flags);
		}

		bool Charging() const
		{
			return FlagMask::GetFlag<uint8_t(PadStateEnum::Charging)>(flags);
		}

		bool BatteryInfoAvailable() const
		{
			return FlagMask::GetFlag<uint8_t(PadStateEnum::BatteryInfoAvailable)>(flags);
		}

		uint8_t Battery() const
		{
			return (flags >> uint8_t(PadStateEnum::Battery0)) & UINT8_MAX;
		}

		ConnectionQualityEnum ConnectionQuality() const
		{
			return ConnectionQualityEnum((flags >> uint8_t(PadStateEnum::ConnectionQuality0)) & uint8_t(ConnectionQualityEnum::EnumMax));
		}

	public:// Write API.
		void SetBattery(const uint8_t value)
		{
			flags &= ~(uint16_t(UINT8_MAX) << uint8_t(PadStateEnum::Battery0));
			flags |= (uint16_t)value << uint8_t(PadStateEnum::Battery0);
		}

		void SetConnectionQuality(const ConnectionQualityEnum value)
		{
			flags &= ~(uint16_t(ConnectionQualityEnum::EnumMax) << uint8_t(PadStateEnum::ConnectionQuality0));
			flags |= (uint16_t)value << uint8_t(PadStateEnum::ConnectionQuality0);
		}

		void SetConnected(const bool value)
		{
			FlagMask::SetFlag<uint8_t(PadStateEnum::Connected)>(flags, value);
		}

		void SetCharging(const bool value)
		{
			FlagMask::SetFlag<uint8_t(PadStateEnum::Charging)>(flags, value);
		}

		void SetBatteryInfoAvailable(const bool value)
		{
			FlagMask::SetFlag<uint8_t(PadStateEnum::BatteryInfoAvailable)>(flags, value);
		}
	};
}
#endif