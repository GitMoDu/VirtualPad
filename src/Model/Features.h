// Features.h

#ifndef _FEATURES_h
#define _FEATURES_h

#include <stdint.h>

enum class FeaturesEnum : uint16_t
{
	L1 = 0,
	R1 = 1,
	L2 = 2,
	R2 = 3,
	L3 = 4,
	R3 = 5,
	Joy2 = 6,
	Select = 7,
	Home = 8,
	Share = 9,
	Joy2Digital = 10,
	L2R2Digital = 11,
	BAccept = 12,
	YReject = 13
};

class FeatureFlags
{
private:
	template<typename... Parameters>
	static constexpr uint16_t FeaturesOrIndex(const FeaturesEnum first, Parameters... parameters)
	{
		return ((uint16_t)1 << (uint8_t)first) | FeaturesOrIndex(parameters...);
	}

	template<typename... Parameters>
	static constexpr uint8_t FeaturesOrIndex(const FeaturesEnum first)
	{
		return ((uint16_t)1 << (uint8_t)first);
	}

public:
	static constexpr uint16_t GetFlags()
	{
		return 0;
	}

	template<FeaturesEnum...features>
	static constexpr uint16_t GetFlags()
	{
		return FeaturesOrIndex(features...);
	}

	template<const FeaturesEnum feature>
	static constexpr bool GetFeatureEnabled(const uint16_t featureFlags)
	{
		return featureFlags & (((uint16_t)1) << ((uint8_t)feature));
	}
};

#endif