﻿// VirtualPad.h

#ifndef _VIRTUAL_PAD_h
#define _VIRTUAL_PAD_h

#include "Model/virtual_pad_state_t.h"
#include "Model/DigitalButtons.h"
#include "Model/Features.h"
#include "ControllerParser/ButtonParser.h"
#include "ControllerParser/AxisParser.h"
#include "ControllerParser/JoystickParser.h"

/// <summary>
/// VirtualPad (read-only) interface.
/// Use of button masks is slower than boolean fields, but uses way less RAM and speeds up state copies significantly.
/// Modelled after RetroArch's RetroPad.
///			
/// Menu Buttons:
/// 
///	[Home]		[Share] 
/// [Select]	[Start]
///	
/// 
/// DPad:
/// 
///	[↖]	[↑] [↗]
///	[←]	 · 	[→]
///	[↙]	[↓]	[↘]			
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
///	[↖]	[↑]	[↗]		[↖]	[↑]	[↗]
///	[←]	L3	[→]		[←]	 R3	[→]
///	[↙]	[↓]	[↘]		[↙]	[↓]	[↘]	
/// 
/// 
/// Digital and Analog Triggers:
/// 
/// [L1]		[R1]
/// [L2]		[R2]
///
/// </summary>
class VirtualPad
{
protected:
	virtual_pad_state_t State{};

private:
	const uint32_t ConfigurationCode;

protected:
	template<const uint8_t bitIndex>
	static constexpr uint8_t ButtonMask()
	{
		return ((uint8_t)1) << ((uint8_t)bitIndex);
	}

	template<const uint8_t buttonIndex>
	static constexpr bool GetButton(const uint8_t buttonsState)
	{
		return buttonsState & ButtonMask<buttonIndex>();
	}

public:
	VirtualPad(const uint32_t configurationCode = 0) : ConfigurationCode(configurationCode)
	{}

	/// <summary>
	/// State interface.
	/// </summary>
public:
	const bool Connected() const
	{
		return State.Connected;
	}

	void Clear()
	{
		State.Clear();
	}

	void CopyStateTo(virtual_pad_state_t& controllerState)
	{
		State.CopyTo(controllerState);
	}

	void CopyStateTo(VirtualPad& controllerState)
	{
		State.CopyTo(controllerState.State);
	}

	void CopyStateFrom(const virtual_pad_state_t& controllerState)
	{
		State.CopyFrom(controllerState);
	}

	void CopyStateFrom(const VirtualPad& controllerState)
	{
		State.CopyFrom(controllerState.State);
	}

	/// <summary>
	/// Navigation interface, based on configuration code.
	/// </summary>
public:
	const bool GetAccept() const
	{
		switch (VirtualPadConfiguration::GetNavigation(ConfigurationCode))
		{
		case NavigationEnum::BA:
		case NavigationEnum::BX:
		case NavigationEnum::BY:
			return B();
			break;
		case NavigationEnum::XA:
		case NavigationEnum::XB:
			return X();
			break;
		default:
		case NavigationEnum::AB:
		case NavigationEnum::AX:
		case NavigationEnum::AY:
			return A();
			break;
		}
	}

	const bool GetReject() const
	{
		switch (VirtualPadConfiguration::GetNavigation(ConfigurationCode))
		{
		case NavigationEnum::BA:
		case NavigationEnum::XA:
			return A();
			break;
		case NavigationEnum::BY:
		case NavigationEnum::AY:
			return Y();
			break;
		case NavigationEnum::BX:
		case NavigationEnum::AX:
			return X();
			break;
		default:
		case NavigationEnum::XB:
		case NavigationEnum::AB:
			return B();
			break;
		}
	}

	/// <summary>
	/// Controller Joysticks, Triggers and Buttons read interface.
	/// </summary>
public:
	const DPadEnum DPad() const
	{
		return State.DPad;
	}

	const bool DPadUp() const
	{
		return State.DPad == DPadEnum::Up || State.DPad == DPadEnum::UpLeft || State.DPad == DPadEnum::UpRight;
	}

	const bool DPadDown() const
	{
		return State.DPad == DPadEnum::Down || State.DPad == DPadEnum::DownLeft || State.DPad == DPadEnum::DownRight;
	}

	const bool DPadLeft() const
	{
		return State.DPad == DPadEnum::Left || State.DPad == DPadEnum::DownLeft || State.DPad == DPadEnum::UpLeft;
	}

	const bool DPadRight() const
	{
		return State.DPad == DPadEnum::Right || State.DPad == DPadEnum::DownRight || State.DPad == DPadEnum::UpRight;
	}

	const int16_t Joy1X() const
	{
		return State.Joy1X;
	}

	const int16_t Joy1Y() const
	{
		return State.Joy1Y;
	}

	const int16_t Joy2X() const
	{
		return State.Joy2X;
	}

	const int16_t Joy2Y() const
	{
		return State.Joy2Y;
	}

	const uint16_t L2() const
	{
		return State.L2;
	}

	const uint16_t R2() const
	{
		return State.R2;
	}

	const bool A() const
	{
		return GetButton<(uint8_t)MainButtonEnum::A>(State.MainButtons);
	}

	const bool B() const
	{
		return GetButton<(uint8_t)MainButtonEnum::B>(State.MainButtons);
	}

	const bool X() const
	{
		return GetButton<(uint8_t)MainButtonEnum::X>(State.MainButtons);
	}

	const bool Y() const
	{
		return GetButton<(uint8_t)MainButtonEnum::Y>(State.MainButtons);
	}

	const bool L1() const
	{
		return GetButton<(uint8_t)MainButtonEnum::L1>(State.MainButtons);
	}

	const bool L3() const
	{
		return GetButton<(uint8_t)MainButtonEnum::L3>(State.MainButtons);
	}

	const bool R1() const
	{
		return GetButton<(uint8_t)MainButtonEnum::R1>(State.MainButtons);
	}

	const bool R3() const
	{
		return GetButton<(uint8_t)MainButtonEnum::R3>(State.MainButtons);
	}

	const bool Start() const
	{
		return GetButton<(uint8_t)MenuButtonEnum::Start>(State.MenuButtons);
	}

	const bool Select() const
	{
		return GetButton<(uint8_t)MenuButtonEnum::Select>(State.MenuButtons);
	}

	const bool Home() const
	{
		return GetButton<(uint8_t)MenuButtonEnum::Home>(State.MenuButtons);
	}

	const bool Share() const
	{
		return GetButton<(uint8_t)MenuButtonEnum::Share>(State.MenuButtons);
	}

	/// <summary>
	/// Features interface, based on configuration code.
	/// </summary>
public:
	const bool FeatureJoy1() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::Joy1>(ConfigurationCode);
	}

	const bool FeatureJoy2() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::Joy2>(ConfigurationCode);
	}

	const bool FeatureA() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::A>(ConfigurationCode);
	}

	const bool FeatureB() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::B>(ConfigurationCode);
	}

	const bool FeatureX() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::X>(ConfigurationCode);
	}

	const bool FeatureY() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::Y>(ConfigurationCode);
	}

	const bool FeatureStart() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::Start>(ConfigurationCode);
	}

	const bool FeatureSelect() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::Select>(ConfigurationCode);
	}

	const bool FeatureHome() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::Home>(ConfigurationCode);
	}

	const bool FeatureShare() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::Share>(ConfigurationCode);
	}

	const bool FeatureL1() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::L1>(ConfigurationCode);
	}

	const bool FeatureR1() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::R1>(ConfigurationCode);
	}

	const bool FeatureL2() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::L2>(ConfigurationCode);
	}

	const bool FeatureR2() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::R2>(ConfigurationCode);
	}

	const bool FeatureL3() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::L3>(ConfigurationCode);
	}

	const bool FeatureR3() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::R3>(ConfigurationCode);
	}

	const bool FeatureDPad() const
	{
		return VirtualPadConfiguration::GetFeature<FeaturesEnum::DPad>(ConfigurationCode);
	}

	/// <summary>
	/// Properties interface, based on configuration code.
	/// </summary>
public:
	const bool PropertyJoy1Digital() const
	{
		return VirtualPadConfiguration::GetProperty<PropertiesEnum::Joy1Digital>(ConfigurationCode);
	}

	const bool PropertyJoy2Digital() const
	{
		return VirtualPadConfiguration::GetProperty<PropertiesEnum::Joy2Digital>(ConfigurationCode);
	}

	const bool PropertyL2R2Digital() const
	{
		return VirtualPadConfiguration::GetProperty<PropertiesEnum::L2R2Digital>(ConfigurationCode);
	}
};
#endif