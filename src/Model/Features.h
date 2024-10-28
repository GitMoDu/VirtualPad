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
/// Accept and Reject buttons configuration for navigation.
/// All combinations of face buttons can be used.
/// </summary>
enum class NavigationEnum : uint8_t
{
	/// <summary>
	/// A accept, B reject.
	/// </summary>
	AB = 0,

	/// <summary>
	/// A accept, X reject.
	/// </summary>
	AX = 1,

	/// <summary>
	/// A accept, Y reject.
	/// </summary>
	AY = 2,

	/// <summary>
	/// B accept, A reject.
	/// </summary>
	BA = 3,

	/// <summary>
	/// B accept, X reject.
	/// </summary>
	BX = 4,

	/// <summary>
	/// B accept, Y reject.
	/// </summary>
	BY = 5,

	/// <summary>
	/// X accept, A reject.
	/// </summary>
	XA = 6,

	/// <summary>
	/// X accept, B reject.
	/// </summary>
	XB = 7,

	NavigationEnumCount
};

enum class PropertiesEnum : uint8_t
{
	/*/// <summary>
	/// Full controller, no special properties. Default field.
	/// </summary>
	Default = 0,*/

	/// <summary>
	/// Main analog joystick is actually a digital output.
	/// </summary>
	Joy1Digital = 0,

	/// <summary>
	/// Secondary analog joystick is actually a digital output.
	/// </summary>
	Joy2Digital = 1,

	/// <summary>
	/// Analog triggers are actually digital outputs.
	/// </summary>
	L2R2Digital = 2,

	PropertiesEnumCount
};

///// <summary>
///// Constexpr features reader and flag calculator.
///// All features can be true/false.
///// The Pad source can generate a features flag statically.
///// The Pad consumer can query features statically.
///// </summary>

/// <summary>
/// Virtual Pad constexpr configuration..
/// Reader and flag calculator.
/// 
/// Configuration code is a composite value with the features, properties and navigation settings of the Virtual Pad encoded in a single 32 bit value.
/// || Navigation (3b) | Features (17b) | Properties (3b) | Padding (xb) ||
/// </summary>
class VirtualPadConfiguration
{
private:
	/// <summary>
	/// Variadic template helper class.
	/// </summary>
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

private:
	static constexpr uint32_t GetFeatureFlagsFromCode(const uint32_t configurationCode)
	{
		return (configurationCode >> 3) & 0b00011111111111111111;
	}

	static constexpr uint8_t GetPropertyFlagsFromCode(const uint32_t configurationCode)
	{
		return (configurationCode >> (3 + (uint8_t)FeaturesEnum::FeatureEnumCount)) & 0b00000111;
	}

public:
	/// <summary>
	/// Default value for PropertyFlags.
	/// </summary>
	static constexpr uint16_t NoProperties = 0;

public:
	/// <summary>
	/// Template parameter generator for FeatureFlags.
	/// </summary>
	/// <typeparam name="...features">Feature list.</typeparam>
	/// <returns>FeatureFlags code.</returns>
	template<FeaturesEnum...features>
	static constexpr uint32_t GetFeatureFlags()
	{
		return TemplateFlag::GetFlags<(uint8_t)features...>();
	}

	/// <summary>
	/// Template parameter generator for PropertyFlags.
	/// </summary>
	/// <typeparam name="...properties">Property list.</typeparam>
	/// <returns>PropertyFlags code.</returns>
	template<PropertiesEnum... properties>
	static constexpr uint16_t GetPropertyFlags()
	{
		return TemplateFlag::GetFlags<(uint8_t)properties...>();
	}

	/// <summary>
	/// Configuration code is a composite value with the features, properties and navigation settings of the Virtual Pad encoded in a single 32 bit value.
	/// || Navigation (3b) | Features (17b) | Properties (3b) | Padding (xb) ||
	/// </summary>
	/// <param name="featureFlags">Use GetFeatureFlags() to generate.</param>
	/// <param name="propertyFlags">Use NoProperties or GetPropertyFlags() to generate.</param>
	/// <param name="navigationCode">NavigationEnum value.</param>
	/// <returns>Virtual Pad configuration code.</returns>
	static constexpr uint32_t GetConfigurationCode(const uint32_t featureFlags,
		const uint16_t propertyFlags = NoProperties,
		const NavigationEnum navigationCode = NavigationEnum::AB)
	{
		return ((uint32_t)navigationCode << 0)
			| ((uint32_t)featureFlags << 3)
			| ((uint32_t)propertyFlags << (3 + (uint8_t)FeaturesEnum::FeatureEnumCount))
			;
	}

	/// <summary>
	/// Read the Virtual Pad navigation setting from the configuration code.
	/// </summary>
	/// <param name="configurationCode">Virtual Pad configuration code.</param>
	/// <returns></returns>
	static constexpr NavigationEnum GetNavigation(const uint32_t configurationCode)
	{
		return (NavigationEnum)(uint8_t)(configurationCode & 0b00000111);
	}

	/// <summary>
	/// Read a feature flag from the configuration code.
	/// </summary>
	/// <typeparam name="feature">FeatureEnum value.</typeparam>
	/// <param name="configurationCode">Virtual Pad configuration code.</param>
	/// <returns>True if feature is enabled.</returns>
	template<const FeaturesEnum feature>
	static constexpr bool GetFeature(const uint32_t configurationCode)
	{
		return TemplateFlag::GetFlagEnabled<(uint8_t)feature>(GetFeatureFlagsFromCode(configurationCode));
	}

	/// <summary>
	/// Read a property flag from the configuration code.
	/// </summary>
	/// <typeparam name="property">PropertyEnum value.</typeparam>
	/// <param name="configurationCode"></param>
	/// <returns>True if property is enabled.</returns>
	template<const PropertiesEnum property>
	static constexpr bool GetProperty(const uint32_t configurationCode)
	{
		return TemplateFlag::GetFlagEnabled<(uint8_t)property>(GetPropertyFlagsFromCode(configurationCode));
	}
};
#endif