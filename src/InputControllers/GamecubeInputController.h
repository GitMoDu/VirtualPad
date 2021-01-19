// GamecubeInputController.h

#ifndef _GAMECUBE_INPUT_CONTROLLER_h
#define _GAMECUBE_INPUT_CONTROLLER_h


#include <ControllerTaskTemplateInclude.h>

// https://github.com/GitMoDu/NintendoControllerReader
#include <BitBangGCController.h>


// With an official controller attached, there is a typical interval of about 6ms between successive updates.
// Each update lasts around 348us.
// in http://www.int03.co.uk/crema/hardware/gamecube/gc-control.html
template<
	typename Calibration,
	const uint8_t Pin,
	const uint32_t UpdatePeriodMillis = 15>
	class GamecubeControllerTask : public ControllerTaskTemplate<UpdatePeriodMillis>
{
private:
	using GamecubeButtons = GameCube::Buttons;

	static const uint16_t Mid = (UINT16_MAX / 2);

	// Template calibrations.
	AxisCentered<uint8_t, uint16_t, Calibration::JoyXMin, Calibration::JoyXMax, Calibration::JoyXOffset, Calibration::JoyDeadZoneRadius, 0, UINT16_MAX> AxisJoy1X;
	AxisCentered<uint8_t, uint16_t, Calibration::JoyYMin, Calibration::JoyYMax, Calibration::JoyYOffset, Calibration::JoyDeadZoneRadius, 0, UINT16_MAX> AxisJoy1Y;

	AxisCentered<uint8_t, uint16_t, Calibration::JoyCXMin, Calibration::JoyCXMax, Calibration::JoyCXOffset, Calibration::JoyCDeadZoneRadius, 0, UINT16_MAX> AxisJoy2X;
	AxisCentered<uint8_t, uint16_t, Calibration::JoyCYMin, Calibration::JoyCYMax, Calibration::JoyCYOffset, Calibration::JoyCDeadZoneRadius, 0, UINT16_MAX> AxisJoy2Y;

	AxisLinear<uint8_t, uint16_t, Calibration::TriggerLMin, Calibration::TriggerLMax, Calibration::TriggerLDeadZone, 0, UINT16_MAX > AxisTriggerL;
	AxisLinear<uint8_t, uint16_t, Calibration::TriggerRMin, Calibration::TriggerRMax, Calibration::TriggerRDeadZone, 0, UINT16_MAX> AxisTriggerR;

	// Controller reader.
	BitBangGCController Controller;

	//Template members for use in this class.
	using ControllerTaskTemplate<UpdatePeriodMillis>::Delay;
	using ControllerTaskTemplate<UpdatePeriodMillis>::Enable;
	using ControllerTaskTemplate<UpdatePeriodMillis>::OnControllerReadOk;
	using ControllerTaskTemplate<UpdatePeriodMillis>::OnControllerFail;


public:
	GamecubeControllerTask(Scheduler* scheduler)
		: ControllerTaskTemplate<UpdatePeriodMillis>(scheduler)
		, Controller(Pin)
	{
		Controller.Data.JoystickX = AxisJoy1X.GetCenter();
		Controller.Data.JoystickY = AxisJoy1Y.GetCenter();
	}

	virtual void GetDirection(bool& left, bool& right, bool& up, bool& down)
	{
		left = Controller.Data.Buttons & (1 << GamecubeButtons::Left);
		right = Controller.Data.Buttons & (1 << GamecubeButtons::Right);
		up = Controller.Data.Buttons & (1 << GamecubeButtons::Up);
		down = Controller.Data.Buttons & (1 << GamecubeButtons::Down);
	}

	virtual uint16_t GetJoy1X()
	{
		return AxisJoy1X.Parse(Controller.Data.JoystickX);
	}

	virtual uint16_t GetJoy1Y()
	{
		return AxisJoy1Y.Parse(Controller.Data.JoystickY);
	}

	virtual uint16_t GetJoy2X()
	{
		return AxisJoy2X.Parse(Controller.Data.JoystickCX);
	}

	virtual uint16_t GetJoy2Y()
	{
		return AxisJoy2Y.Parse(Controller.Data.JoystickCY);
	}

	virtual uint16_t GetTriggerL()
	{
		return AxisTriggerL.Parse(Controller.Data.SliderLeft);
	}

	virtual uint16_t GetTriggerR()
	{
		uint16_t value = AxisTriggerR.Parse(Controller.Data.SliderRight);
		if (value > Calibration::TriggerRDeadZone)
		{
			return value;
		}
		else
		{
			return value;
		}
	}

	virtual bool GetLeft() { return  Controller.Data.Buttons & (1 << GamecubeButtons::Left); }
	virtual bool GetRight() { return  Controller.Data.Buttons & (1 << GamecubeButtons::Right); }
	virtual bool GetUp() { return Controller.Data.Buttons & (1 << GamecubeButtons::Up); }
	virtual bool GetDown() { return Controller.Data.Buttons & (1 << GamecubeButtons::Down); }

	virtual bool GetButton0() { return Controller.Data.Buttons & (1 << GamecubeButtons::A); }
	virtual bool GetButton1() { return Controller.Data.Buttons & (1 << GamecubeButtons::B); }
	virtual bool GetButton2() { return Controller.Data.Buttons & (1 << GamecubeButtons::X); }
	virtual bool GetButton3() { return Controller.Data.Buttons & (1 << GamecubeButtons::Y); }
	virtual bool GetButton4() { return Controller.Data.Buttons & (1 << GamecubeButtons::Z); }
	virtual bool GetButton5() { return Controller.Data.Buttons & (1 << GamecubeButtons::L); }
	virtual bool GetButton6() { return Controller.Data.Buttons & (1 << GamecubeButtons::R); }
	virtual bool GetButton7() { return Controller.Data.Buttons & (1 << GamecubeButtons::Start); }

	// Interface controls redirection.
	virtual bool GetButtonAccept() { return GetButton0(); }
	virtual bool GetButtonReject() { return GetButton1(); }
	virtual bool GetButtonHome() { return GetButton7(); }


protected:
	bool Callback()
	{
		if (Controller.Poll())
		{
			OnControllerReadOk();
		}
		else
		{
			OnControllerFail();
		}


		return true;
	}
};

#endif

