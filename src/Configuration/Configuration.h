#ifndef _VIRTUAL_PAD_CONFIGURATION_
#define _VIRTUAL_PAD_CONFIGURATION_

#include "Features.h"
#include "Properties.h"
#include "Navigation.h"
#include "Flag.h"

namespace VirtualPad
{
	/// <summary>
	/// Configuration code is a composite value with the features, properties and navigation settings of the Virtual Pad encoded in a single 32 bit value.
	/// || Navigation (3b) | Properties (3b)  | Features (26b) ||
	/// </summary>
	namespace Configuration
	{
		/// <summary>
		///
		/// </summary>
		/// <param name="featureFlags">Use GetFeatureFlags() to generate.</param>
		/// <param name="propertyFlags">Use NoProperties or GetPropertyFlags() to generate.</param>
		/// <param name="navigationCode">NavigationEnum value.</param>
		/// <returns>Virtual Pad configuration code.</returns>
		/// <returns></returns>
		static constexpr uint32_t GetConfigurationCode(const uint32_t featureFlags,
			const uint16_t propertyFlags = Configuration::NoProperties,
			const NavigationEnum navigationCode = NavigationEnum::AB)
		{
			return ((uint32_t)navigationCode << 0)
				| ((uint32_t)propertyFlags << 3)
				| ((uint32_t)featureFlags << 6);
		}

		static constexpr uint32_t GetFeatureFlags(const uint32_t configurationCode)
		{
			return (configurationCode >> 6);
		}

		static constexpr uint8_t GetPropertyFlags(const uint32_t configurationCode)
		{
			return (configurationCode >> 3) & 0b111;
		}

		static constexpr uint8_t GetNavigationFlags(const uint32_t configurationCode)
		{
			return configurationCode & 0b111;
		}

		/// <summary>
		/// Read the Virtual Pad navigation setting from the configuration code.
		/// </summary>
		/// <param name="configurationCode">Virtual Pad configuration code.</param>
		/// <returns></returns>
		static constexpr NavigationEnum GetNavigation(const uint32_t configurationCode)
		{
			return NavigationEnum(GetNavigationFlags(configurationCode));
		}

		/// <summary>
		/// Read a property flag from the configuration code.
		/// </summary>
		/// <typeparam name="property">PropertyEnum value.</typeparam>
		/// <param name="configurationCode"></param>
		/// <returns>True if property is enabled.</returns>
		template<const PropertiesEnum propertyEnum>
		static constexpr bool GetProperty(const uint32_t configurationCode)
		{
			return TemplateFlag::GetFlagEnabled<uint8_t(propertyEnum)>(GetPropertyFlags(configurationCode));
		}

		/// <summary>
		/// Read a feature flag from the configuration code.
		/// </summary>
		/// <typeparam name="feature">FeatureEnum value.</typeparam>
		/// <param name="configurationCode">Virtual Pad configuration code.</param>
		/// <returns>True if feature is enabled.</returns>
		template<const FeaturesEnum featureEnum>
		static constexpr bool GetFeature(const uint32_t configurationCode)
		{
			return TemplateFlag::GetFlagEnabled<uint8_t(featureEnum)>(GetFeatureFlags(configurationCode));
		}

		template<uint8_t properties = Configuration::NoProperties>
		static constexpr uint32_t XBoxControllerConfigCode()
		{
			return GetConfigurationCode(
				Configuration::GetFeatureFlags<FeaturesEnum::DPad,
				FeaturesEnum::Joy1,
				FeaturesEnum::Joy2,
				FeaturesEnum::Start, FeaturesEnum::Select, FeaturesEnum::Home, FeaturesEnum::Share,
				FeaturesEnum::A, FeaturesEnum::B, FeaturesEnum::X, FeaturesEnum::Y,
				FeaturesEnum::L1, FeaturesEnum::R1,
				FeaturesEnum::L2, FeaturesEnum::R2,
				FeaturesEnum::L3, FeaturesEnum::R3>(),
				properties,
				NavigationEnum::AB);
		}
	}
}
#endif