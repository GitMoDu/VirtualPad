// Features.h

#ifndef _FEATURES_h
#define _FEATURES_h

#include <stdint.h>

/// <summary>
/// Available features of the Virtual Pad.
/// All features can be on/off.
/// </summary>
enum class FeaturesEnum : uint8_t
{
	/// <summary>
	/// Pad has a digital directional pad (DPad).
	/// </summary>
	DPad = 0,

	/// <summary>
	/// Pad has a main analog joystick.
	/// </summary>
	Joy1 = 1,

	/// <summary>
	/// Pad has a secondary analog joystick.
	/// </summary>
	Joy2 = 2,

	/// <summary>
	/// Pad has a left analog trigger.
	/// </summary>
	L2 = 3,

	/// <summary>
	/// Pad has a right analog trigger.
	/// </summary>
	R2 = 4,

	/// <summary>
	/// Pad has face button A.
	/// </summary>
	A = 5,

	/// <summary>
	/// Pad has face button B.
	/// </summary>
	B = 6,

	/// <summary>
	/// Pad has face button X.
	/// </summary>
	X = 7,

	/// <summary>
	/// Pad has face button Y.
	/// </summary>
	Y = 8,

	/// <summary>
	/// Pad has a left bumper.
	/// </summary>
	L1 = 9,

	/// <summary>
	/// Pad has a right bumper.
	/// </summary>
	R1 = 10,

	/// <summary>
	/// Pad has clicky main joystick button.
	/// </summary>
	L3 = 11,

	/// <summary>
	/// Pad has clicky secondary joystick button.
	/// </summary>
	R3 = 12,

	/// <summary>
	/// Pad has a start button.
	/// </summary>
	Start = 13,

	/// <summary>
	/// Pad has a select button or equivalent.
	/// </summary>
	Select = 14,

	/// <summary>
	/// Pad has a home button or equivalent.
	/// </summary>
	Home = 15,

	/// <summary>
	/// Pad has a share button or equivalent.
	/// </summary>
	Share = 16,

	FeatureEnumCount
};

/// <summary>
/// Accept and Reject buttons configuration.
/// All combinations of face buttons can be used.
/// </summary>
enum class NavigationEnum : uint8_t
{
	/// <summary>
	/// A accept, B reject.
	/// </summary>
	AB,

	/// <summary>
	/// A accept, X reject.
	/// </summary>
	AX,

	/// <summary>
	/// A accept, Y reject.
	/// </summary>
	AY,

	/// <summary>
	/// B accept, A reject.
	/// </summary>
	BA,

	/// <summary>
	/// B accept, X reject.
	/// </summary>
	BX,

	/// <summary>
	/// B accept, Y reject.
	/// </summary>
	BY,

	/// <summary>
	/// X accept, A reject.
	/// </summary>
	XA,

	/// <summary>
	/// X accept, B reject.
	/// </summary>
	XB,

	NavigationEnumCount
};

enum class PropertiesEnum : uint8_t
{
	/// <summary>
	/// Analog triggers are actually digital outputs.
	/// </summary>
	L2R2Digital,

	/// <summary>
	/// Main analog joystick is actually a digital output.
	/// </summary>
	Joy1Digital,

	/// <summary>
	/// Secondary analog joystick is actually a digital output.
	/// </summary>
	Joy2Digital,

	PropertiesEnumCount
};


class TemplateFlag
{
private:
	template<typename... Parameters>
	static constexpr uint32_t FlagsOrIndex(const uint8_t first, Parameters... parameters)
	{
		return ((uint32_t)1 << (uint8_t)first) | FlagsOrIndex(parameters...);
	}

	template<typename... Parameters>
	static constexpr uint32_t FlagsOrIndex(const uint8_t first)
	{
		return ((uint32_t)1 << (uint8_t)first);
	}


	static constexpr uint32_t GetFlags()
	{
		return 0;
	}

public:
	template<uint8_t...flags>
	static constexpr uint32_t GetFlags()
	{
		return FlagsOrIndex(flags...);
	}

	template<const uint8_t flag>
	static constexpr bool GetFlagEnabled(const uint32_t flags)
	{
		return flags & (((uint32_t)1) << ((uint8_t)flag));
	}
};


/// <summary>
/// Constexpr features reader and flag calculator.
/// All features can be true/false.
/// The Pad source can generate a features flag statically.
/// The Pad consumer can query features statically.
/// </summary>
class FeatureFlags
{
private:
	template<typename... Parameters>
	static constexpr uint32_t FeaturesOrIndex(const FeaturesEnum first, Parameters... parameters)
	{
		return ((uint32_t)1 << (uint8_t)first) | FeaturesOrIndex(parameters...);
	}

	template<typename... Parameters>
	static constexpr uint32_t FeaturesOrIndex(const FeaturesEnum first)
	{
		return ((uint32_t)1 << (uint8_t)first);
	}

	static constexpr uint32_t GetFlags()
	{
		return 0;
	}

public:
	template<FeaturesEnum...features>
	static constexpr uint32_t GetFlags()
	{
		return TemplateFlag::GetFlags<(uint8_t)features...>();
		//return FeaturesOrIndex(features...);
	}

	template<const FeaturesEnum feature>
	static constexpr bool GetFeatureEnabled(const uint32_t featureFlags)
	{
		return featureFlags & (((uint32_t)1) << ((uint8_t)feature));
	}
};



/// <summary>
/// Virtual Pad configuration code.
/// Composite value with the configuration properties of the Virtual Pad encoded in a single 32 bit value.
/// || Navigation (3b) | Features (17b) | Properties (3b) | Padding (xb) ||
/// </summary>
class VirtualPadConfiguration
{
public:
	static constexpr uint32_t GetFeatureFlags(const uint32_t configurationCode)
	{
		return (configurationCode >> 3) & 0b00011111111111111111;
	}

	static constexpr uint32_t GetPropertyFlags(const uint32_t configurationCode)
	{
		return (configurationCode >> (3 + (uint8_t)FeaturesEnum::FeatureEnumCount)) & 0b00000111;
	}

public:
	template<FeaturesEnum...features>
	static constexpr uint32_t GetFeatureFlags()
	{
		return TemplateFlag::GetFlags<(uint8_t)features...>();
	}

	template<PropertiesEnum... properties>
	static constexpr uint32_t GetPropertyFlags()
	{
		return TemplateFlag::GetFlags<(uint8_t)properties...>();
	}

	static constexpr uint32_t GetConfigurationCode(const uint32_t featureFlags,
		const uint8_t propertyFlags,
		const NavigationEnum navigationCode = NavigationEnum::AB)
	{
		return
			((uint32_t)navigationCode << 0)
			| ((uint32_t)featureFlags << 3)
			| ((uint32_t)propertyFlags << (3 + (uint8_t)FeaturesEnum::FeatureEnumCount))
			;
	}


	static constexpr NavigationEnum GetNavigation(const uint32_t configurationCode)
	{
		return (NavigationEnum)(uint8_t)(configurationCode & 0b00000111);
	}

	template<const FeaturesEnum feature>
	static constexpr bool GetFeature(const uint32_t configurationCode)
	{
		return GetFeatureFlags(configurationCode) & (((uint32_t)1) << ((uint8_t)feature));
	}

	template<const PropertiesEnum property>
	static constexpr bool GetProperty(const uint32_t configurationCode)
	{
		return GetPropertyFlags(configurationCode) & (((uint32_t)1) << ((uint8_t)property));
	}
};
#endif