// GamecubeInputController.h

#ifndef _GAMECUBE_INPUT_CONTROLLER_h
#define _GAMECUBE_INPUT_CONTROLLER_h


#include <ControllerTaskTemplateInclude.h>
#include <NintendoControllerReaderSTM32.h>


// With an official controller attached, there is a typical interval of about 6ms between successive updates.
// Each update lasts around 348us.
// in http://www.int03.co.uk/crema/hardware/gamecube/gc-control.html
template<
	typename Calibration,
	const uint8_t Pin,
	const uint32_t UpdatePeriodMillis = 3>
	class GamecubeControllerTask : public ControllerTaskTemplate<UpdatePeriodMillis>
{
private:
	static const uint16_t Mid = (UINT16_MAX / 2);

	// Template calibrations.
	AxisCentered<uint8_t, uint16_t, Calibration::JoyXMin, Calibration::JoyXMax, Calibration::JoyXOffset, Calibration::JoyDeadZoneRadius, 0, UINT16_MAX> AxisJoy1X;
	AxisCentered<uint8_t, uint16_t, Calibration::JoyYMin, Calibration::JoyYMax, Calibration::JoyYOffset, Calibration::JoyDeadZoneRadius, 0, UINT16_MAX> AxisJoy1Y;

	AxisCentered<uint8_t, uint16_t, Calibration::JoyCXMin, Calibration::JoyCXMax, Calibration::JoyCXOffset, Calibration::JoyCDeadZoneRadius, 0, UINT16_MAX> AxisJoy2X;
	AxisCentered<uint8_t, uint16_t, Calibration::JoyCYMin, Calibration::JoyCYMax, Calibration::JoyCYOffset, Calibration::JoyCDeadZoneRadius, 0, UINT16_MAX> AxisJoy2Y;

	AxisLinear<uint8_t, uint16_t, Calibration::TriggerLMin, Calibration::TriggerLMax, Calibration::TriggerLDeadZone, 0, UINT16_MAX > AxisTriggerL;
	AxisLinear<uint8_t, uint16_t, Calibration::TriggerRMin, Calibration::TriggerRMax, Calibration::TriggerRDeadZone, 0, UINT16_MAX> AxisTriggerR;

	// Controller reader.
	GameCubeController Controller;

	//Template members for use in this class.
	using ControllerTaskTemplate<UpdatePeriodMillis>::Delay;
	using ControllerTaskTemplate<UpdatePeriodMillis>::Enable;
	using ControllerTaskTemplate<UpdatePeriodMillis>::OnControllerReadOk;
	using ControllerTaskTemplate<UpdatePeriodMillis>::OnControllerFail;

	using GamecubeButtons = GameCubeController::GamecubeButtons;

	GameCubeData_t RawData;

public:
	GamecubeControllerTask(Scheduler* scheduler)
		: ControllerTaskTemplate<UpdatePeriodMillis>(scheduler)
		, Controller(Pin)
	{
		RawData.JoystickX = AxisJoy1X.GetCenter();
		RawData.JoystickY = AxisJoy1Y.GetCenter();
	}

	virtual void GetDirection(bool& left, bool& right, bool& up, bool& down)
	{
		left = RawData.Buttons & (1 << GamecubeButtons::Left);
		right = RawData.Buttons & (1 << GamecubeButtons::Right);
		up = RawData.Buttons & (1 << GamecubeButtons::Up);
		down = RawData.Buttons & (1 << GamecubeButtons::Down);
	}

	virtual uint16_t GetJoy1X()
	{
		uint16_t value = AxisJoy1X.Parse(RawData.JoystickX);

		if ((value > (Mid - Calibration::JoyDeadZoneRadius))
			&&
			(value < (Mid + Calibration::JoyDeadZoneRadius)))
		{
			return value;
		}
		else
		{
			return value;
		}
	}

	virtual uint16_t GetJoy1Y()
	{
		uint16_t value = AxisJoy1Y.Parse(RawData.JoystickY);
		if ((value > (Mid - Calibration::JoyDeadZoneRadius))
			&&
			(value < (Mid + Calibration::JoyDeadZoneRadius)))
		{
			return value;
		}
		else
		{
			return value;
		}

	}

	virtual uint16_t GetJoy2X()
	{
		uint16_t value = AxisJoy2X.Parse(RawData.JoystickCX);
		if ((value > (Mid - Calibration::JoyCDeadZoneRadius))
			&&
			(value < (Mid + Calibration::JoyCDeadZoneRadius)))
		{
			return value;
		}
		else
		{
			return value;
		}
	}

	virtual uint16_t GetJoy2Y()
	{
		uint16_t value = AxisJoy2Y.Parse(RawData.JoystickCY);
		if ((value > (Mid - Calibration::JoyCDeadZoneRadius))
			&&
			(value < (Mid + Calibration::JoyCDeadZoneRadius)))
		{
			return value;
		}
		else
		{
			return value;
		}
	}

	virtual uint16_t GetTriggerL()
	{
		uint16_t value = AxisTriggerL.Parse(RawData.SliderLeft);
		if (value > Calibration::TriggerLDeadZone)
		{
			return value;
		}
		else
		{
			return value;
		}
	}

	virtual uint16_t GetTriggerR()
	{
		uint16_t value = AxisTriggerR.Parse(RawData.SliderRight);
		if (value > Calibration::TriggerRDeadZone)
		{
			return value;
		}
		else
		{
			return value;
		}
	}

	virtual bool GetLeft() { return  RawData.Buttons & (1 << GamecubeButtons::Left); }
	virtual bool GetRight() { return  RawData.Buttons & (1 << GamecubeButtons::Right); }
	virtual bool GetUp() { return RawData.Buttons & (1 << GamecubeButtons::Up); }
	virtual bool GetDown() { return RawData.Buttons & (1 << GamecubeButtons::Down); }

	virtual bool GetButton0() { return RawData.Buttons & (1 << GamecubeButtons::A); }
	virtual bool GetButton1() { return RawData.Buttons & (1 << GamecubeButtons::B); }
	virtual bool GetButton2() { return RawData.Buttons & (1 << GamecubeButtons::X); }
	virtual bool GetButton3() { return RawData.Buttons & (1 << GamecubeButtons::Y); }
	virtual bool GetButton4() { return RawData.Buttons & (1 << GamecubeButtons::Z); }
	virtual bool GetButton5() { return RawData.Buttons & (1 << GamecubeButtons::L); }
	virtual bool GetButton6() { return RawData.Buttons & (1 << GamecubeButtons::R); }
	virtual bool GetButton7() { return RawData.Buttons & (1 << GamecubeButtons::Start); }

	// Interface controls redirection.
	virtual bool GetButtonAccept() { return GetButton0(); }
	virtual bool GetButtonReject() { return GetButton1(); }
	virtual bool GetButtonHome() { return GetButton7(); }


protected:
	bool Callback()
	{
		if (Controller.Read(&RawData))
		{
			OnControllerReadOk();
		}
		else
		{
			OnControllerFail();
		}


		return true;
	}

	void StopForceFeedback()
	{
		// Send command to stop rumble.
		Controller.ReadWithRumble(&RawData, false);
	}

#ifdef DEBUG_IINPUT_CONTROLLER
public:
	GameCubeData_t* GetRawData()
	{
		return &RawData;
	}

#endif
};

#endif

