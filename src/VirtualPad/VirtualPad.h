#ifndef _VIRTUAL_PAD_h
#define _VIRTUAL_PAD_h

#include "../Model/ButtonPad.h"
#include "../Model/AnalogPad.h"
#include "../Model/MotionPad.h"

#include "../Configuration/Features.h"
#include "../Configuration/Properties.h"
#include "../Configuration/Navigation.h"

#include "../Configuration/Configuration.h"

/// <summary>
/// Virtual Pad Layout.
///			
/// Menu Buttons:
/// 
///				[Home] 
/// [Select]	[Share]		[Start]
///				[Chat]
///	
/// 
/// DPad:
/// 
///	[UpLeft]	 [Up]	[UpRight]
///	[Left]		[None]	[Right]
///	[DownLeft]	[Down]	[DownRight]			
/// 
/// 
/// Face Buttons:
/// 
///		[Y]
/// [X]		[B]
///		[A]
/// 
/// 
/// Joysticks 1 and 2, with respective L3 and R3:
/// 
///		[^]				[^]	
///	[<]	L3	[>]		[<]	R3	[>]
///		[v]				[v]		
/// 
/// 
/// Digital and Analog Triggers:
/// 
///		[L1]			[R1]
///		[L2]			[R2]
///
/// Back paddles 1 and 2:
/// 
///			P1		P2
/// 
/// </summary>
namespace VirtualPad
{
	/// <summary>
	/// Template class for provided pad_state_t.
	/// 
	/// </summary>
	/// <typeparam name="pad_state_t">The underlying virtual pad state type (motion_pad_state_t, analog_pad_state_t, button_pad_state_t, custom_pad_state_t)</typeparam>
	/// <typeparam name="configurationCode"></typeparam>
	template<typename pad_state_t, uint32_t configurationCode>
	struct TemplateVirtualPad : pad_state_t
	{
	public:
		using FeaturesEnum = Configuration::FeaturesEnum;
		using PropertiesEnum = Configuration::PropertiesEnum;
		using NavigationEnum = Configuration::NavigationEnum;

		/// <summary>
		/// State interface.
		/// </summary>
	public:
		void CopyTo(TemplateVirtualPad& to)
		{
			memcpy(&to, this, sizeof(pad_state_t));
		}

		void CopyFrom(const TemplateVirtualPad& from)
		{
			memcpy(this, &from, sizeof(pad_state_t));
		}

		void Clear()
		{
			memset(this, uint8_t(0), sizeof(pad_state_t));
		}

		/// <summary>
		/// Navigation interface, based on configuration code.
		/// </summary>
	public:
		bool Accept() const
		{
			switch (Configuration::GetNavigation(configurationCode))
			{
			case NavigationEnum::BA:
			case NavigationEnum::BX:
			case NavigationEnum::BY:
				return pad_state_t::B();
				break;
			case NavigationEnum::XA:
			case NavigationEnum::XB:
				return pad_state_t::X();
				break;
			default:
			case NavigationEnum::AB:
			case NavigationEnum::AX:
			case NavigationEnum::AY:
				return pad_state_t::A();
				break;
			}
		}

		bool Reject() const
		{
			switch (Configuration::GetNavigation(configurationCode))
			{
			case NavigationEnum::BA:
			case NavigationEnum::XA:
				return pad_state_t::A();
				break;
			case NavigationEnum::BY:
			case NavigationEnum::AY:
				return pad_state_t::Y();
				break;
			case NavigationEnum::BX:
			case NavigationEnum::AX:
				return pad_state_t::X();
				break;
			default:
			case NavigationEnum::XB:
			case NavigationEnum::AB:
				return pad_state_t::B();
				break;
			}
		}

		/// <summary>
		/// Features interface, based on configuration code.
		/// </summary>
	public:
		static constexpr bool FeatureDPad()
		{
			return Configuration::GetFeature<FeaturesEnum::DPad>(configurationCode);
		}

		static constexpr bool FeatureA()
		{
			return Configuration::GetFeature<FeaturesEnum::A>(configurationCode);
		}

		static constexpr bool FeatureB()
		{
			return Configuration::GetFeature<FeaturesEnum::B>(configurationCode);
		}

		static constexpr bool FeatureX()
		{
			return Configuration::GetFeature<FeaturesEnum::X>(configurationCode);
		}

		static constexpr bool FeatureY()
		{
			return Configuration::GetFeature<FeaturesEnum::Y>(configurationCode);
		}

		static constexpr bool FeatureL1()
		{
			return Configuration::GetFeature<FeaturesEnum::L1>(configurationCode);
		}

		static constexpr bool FeatureR1()
		{
			return Configuration::GetFeature<FeaturesEnum::R1>(configurationCode);
		}

		static constexpr bool FeatureL3()
		{
			return Configuration::GetFeature<FeaturesEnum::L3>(configurationCode);
		}

		static constexpr bool FeatureR3()
		{
			return Configuration::GetFeature<FeaturesEnum::R3>(configurationCode);
		}

		static constexpr bool FeatureStart()
		{
			return Configuration::GetFeature<FeaturesEnum::Start>(configurationCode);
		}

		static constexpr bool FeatureSelect()
		{
			return Configuration::GetFeature<FeaturesEnum::Select>(configurationCode);
		}

		static constexpr bool FeatureHome()
		{
			return Configuration::GetFeature<FeaturesEnum::Home>(configurationCode);
		}

		static constexpr bool FeatureShare()
		{
			return Configuration::GetFeature<FeaturesEnum::Share>(configurationCode);
		}

		static constexpr bool FeatureChat()
		{
			return Configuration::GetFeature<FeaturesEnum::Chat>(configurationCode);
		}

		static constexpr bool FeatureJoy1()
		{
			return Configuration::GetFeature<FeaturesEnum::Joy1>(configurationCode);
		}

		static constexpr bool FeatureJoy2()
		{
			return Configuration::GetFeature<FeaturesEnum::Joy2>(configurationCode);
		}

		static constexpr bool FeatureL2()
		{
			return Configuration::GetFeature<FeaturesEnum::L2>(configurationCode);
		}

		static constexpr bool FeatureR2()
		{
			return Configuration::GetFeature<FeaturesEnum::R2>(configurationCode);
		}

		static constexpr bool FeatureMotion()
		{
			return VirtualPad::Configuration::GetFeature<FeaturesEnum::Motion>(configurationCode);
		}

		static constexpr NavigationEnum ConfigurationNavigation()
		{
			return VirtualPad::Configuration::GetNavigation(configurationCode);
		}

		/// <summary>
		/// Properties interface, based on configuration code.
		/// </summary>
	public:
		static constexpr bool PropertyJoy1Digital()
		{
			return VirtualPad::Configuration::GetProperty<PropertiesEnum::Joy1Digital>(configurationCode);
		}

		static constexpr bool PropertyJoy2Digital()
		{
			return VirtualPad::Configuration::GetProperty<PropertiesEnum::Joy2Digital>(configurationCode);
		}

		static constexpr bool PropertyL2R2Digital()
		{
			return VirtualPad::Configuration::GetProperty<PropertiesEnum::L2R2Digital>(configurationCode);
		}
	};


	template<uint32_t configurationCode>
	using ButtonVirtualPad = TemplateVirtualPad<button_pad_state_t, configurationCode>;

	template<uint32_t configurationCode>
	using AnalogVirtualPad = TemplateVirtualPad<analog_pad_state_t, configurationCode>;

	template<uint32_t configurationCode>
	using MotionVirtualPad = TemplateVirtualPad<motion_pad_state_t, configurationCode>;
}
#endif