// IInputController.h

#ifndef _I_INPUT_CONTROLLER_h
#define _I_INPUT_CONTROLLER_h

#include "Model/DigitalButtons.h"
#include "Model/Features.h"
#include "Model/controller_state.h"
#include "ControllerParser/ButtonParser.h"
#include "ControllerParser/AxisParser.h"
#include "ControllerParser/JoystickParser.h"

/// <summary>
/// IInputController (read) interface.
/// Use of button masks is slower than boolean fields, but uses way less RAM and speeds up state copies significantly.
/// Modelled after RetroArch's RetroPad.
///			
/// Menu Buttons:
/// 
///			[Guide]
/// [Select]		[Start]
///			[Share] 
///	
/// 
/// DPad:
/// 
///		[?]
///	[?]		[?]
///		[?]				
/// 
/// 
/// Face Buttons:
/// 
///		[X]
/// [Y]		[A]
///		[B]
/// 
/// 
/// Joysticks 1 and 2, with respective L3 and R3:
/// 
///		[?]				[?]
///	[?]	[3] [?]		[?]	[3] [?]
///		[?]				[?]
/// 
/// 
/// Digital and Analog Triggers:
/// 
/// [L1]		[R1]
/// [L2]		[R2]
/// 
/// </summary>
class IInputController
{
public:
	controller_state_t State{};

private:
	const uint16_t Features;

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
	IInputController(const uint16_t features = 0) : Features(features)
	{}

	/// <summary>
	/// State interface.
	/// </summary>
public:
	void Clear()
	{
		State.Clear();
	}

	void CopyStateTo(controller_state_t& controllerState)
	{
		State.CopyTo(controllerState);
	}

	void CopyStateTo(IInputController& controllerState)
	{
		State.CopyTo(controllerState.State);
	}

	void CopyStateFrom(const controller_state_t& controllerState)
	{
		State.CopyFrom(controllerState);
	}

	void CopyStateFrom(const IInputController& controllerState)
	{
		State.CopyFrom(controllerState.State);
	}

	/// <summary>
	/// Navigation interface, based on declared features.
	/// </summary>
public:
	const bool Connected() const
	{
		return State.Connected;
	}

	const bool GetHome() const
	{
		if (FeatureHome())
		{
			return Home();
		}
		else
		{
			return Start();
		}
	}

	const bool GetAccept() const
	{
		if (FeatureBAccept())
		{
			return B();
		}
		else
		{
			return A();
		}
	}

	const bool GetReject() const
	{
		if (FeatureYReject())
		{
			return Y();
		}
		else if (FeatureBAccept())
		{
			return A();
		}
		else
		{
			return B();
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
		return State.DPad == DPadEnum::Right || State.DPad == DPadEnum::DownRight || State.DPad == DPadEnum::UpRight;;
	}

	const int16_t Joy1X()
	{
		return State.Joy1X;
	}

	const int16_t Joy1Y()
	{
		return State.Joy1Y;
	}

	const int16_t Joy2X()
	{
		return State.Joy2X;
	}

	const int16_t Joy2Y()
	{
		return State.Joy2Y;
	}

	const uint16_t L2()
	{
		return State.L2;
	}

	const uint16_t R2()
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
	/// Features interface.
	/// </summary>
public:
	const bool FeatureJoy2() const
	{
		return FeatureFlags::GetFeatureEnabled<FeaturesEnum::Joy2>(Features);
	}

	const bool FeatureJoy2Digital() const
	{
		return FeatureFlags::GetFeatureEnabled<FeaturesEnum::Joy2Digital>(Features);
	}

	const bool FeatureL2R2Digital() const
	{
		return FeatureFlags::GetFeatureEnabled<FeaturesEnum::L2R2Digital>(Features);
	}

	const bool FeatureBAccept() const
	{
		return FeatureFlags::GetFeatureEnabled<FeaturesEnum::BAccept>(Features);
	}

	const bool FeatureYReject() const
	{
		return FeatureFlags::GetFeatureEnabled<FeaturesEnum::YReject>(Features);
	}

	const bool FeatureShare() const
	{
		return FeatureFlags::GetFeatureEnabled<FeaturesEnum::Share>(Features);
	}

	const bool FeatureSelect() const
	{
		return FeatureFlags::GetFeatureEnabled<FeaturesEnum::Select>(Features);
	}

	const bool FeatureHome() const
	{
		return FeatureFlags::GetFeatureEnabled<FeaturesEnum::Home>(Features);
	}

	const bool FeatureL1() const
	{
		return FeatureFlags::GetFeatureEnabled<FeaturesEnum::L1>(Features);
	}

	const bool FeatureR1() const
	{
		return FeatureFlags::GetFeatureEnabled<FeaturesEnum::R1>(Features);
	}

	const bool FeatureL2() const
	{
		return FeatureFlags::GetFeatureEnabled<FeaturesEnum::L2>(Features);
	}

	const bool FeatureR2() const
	{
		return FeatureFlags::GetFeatureEnabled<FeaturesEnum::R2>(Features);
	}

	const bool FeatureL3() const
	{
		return FeatureFlags::GetFeatureEnabled<FeaturesEnum::L3>(Features);
	}

	const bool FeatureR3() const
	{
		return FeatureFlags::GetFeatureEnabled<FeaturesEnum::R3>(Features);
	}
};
#endif