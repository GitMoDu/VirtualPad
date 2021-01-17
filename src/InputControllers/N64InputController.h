// N64InputController.h

#ifndef _N64_INPUT_CONTROLLER_h
#define _N64_INPUT_CONTROLLER_h


#include <ControllerTaskTemplateInclude.h>

// https://github.com/GitMoDu/NintendoControllerReader
#include <SerialJoyBusN64Controller.h>



template<
	typename Calibration,
	const uint8_t Pin,
	const uint32_t UpdatePeriodMillis = 15>
	class N64ControllerTask : public ControllerTaskTemplate<UpdatePeriodMillis>
{
private:
	// Short hand for buttons enumerator.
	using Buttons = Nintendo64::Buttons;

	static const uint16_t Mid = (UINT16_MAX / 2);

	// Template calibrations.
	AxisCentered<int8_t, uint16_t, Calibration::JoyXMin, Calibration::JoyXMax, Calibration::JoyXOffset, Calibration::JoyDeadZoneRadius, 0, UINT16_MAX> AxisJoy1X;
	AxisCentered<int8_t, uint16_t, Calibration::JoyYMin, Calibration::JoyYMax, Calibration::JoyYOffset, Calibration::JoyDeadZoneRadius, 0, UINT16_MAX> AxisJoy1Y;

	// Controller reader.
	SerialJoyBusN64Controller Controller;

	//Template members for use in this class.
	using BaseClass = ControllerTaskTemplate<UpdatePeriodMillis>;

	using BaseClass::Delay;
	using BaseClass::Enable;
	using BaseClass::OnControllerReadOk;
	using BaseClass::OnControllerFail;


public:
	N64ControllerTask(Scheduler* scheduler, HardwareSerial* serial)
		: ControllerTaskTemplate<UpdatePeriodMillis>(scheduler)
		, Controller(serial)
	{
		Controller.Data.JoystickX = AxisJoy1X.GetCenter();
		Controller.Data.JoystickY = AxisJoy1Y.GetCenter();
	}

	virtual void StartController()
	{
		Controller.Start();
		BaseClass::StartController();
	}

	virtual void StopController()
	{
		BaseClass::StopController();
		Controller.Stop();
	}

	virtual void GetDirection(bool& left, bool& right, bool& up, bool& down)
	{
		left = Controller.Data.Buttons & (1 << Buttons::Left);
		right = Controller.Data.Buttons & (1 << Buttons::Right);
		up = Controller.Data.Buttons & (1 << Buttons::Up);
		down = Controller.Data.Buttons & (1 << Buttons::Down);
	}

	virtual uint16_t GetJoy2X()
	{
		if ((Controller.Data.Buttons & (1 << Buttons::CLeft)) && !(Controller.Data.Buttons & (1 << Buttons::CRight)))
		{
			return 0;
		}
		else if ((Controller.Data.Buttons & (1 << Buttons::CRight)) && !(Controller.Data.Buttons & (1 << Buttons::CLeft)))
		{
			return UINT16_MAX;
		}
		else
		{
			return UINT16_MAX / 2;
		}
	}

	virtual uint16_t GetJoy2Y()
	{
		if ((Controller.Data.Buttons & (1 << Buttons::CDown)) && !(Controller.Data.Buttons & (1 << Buttons::CUp)))
		{
			return 0;
		}
		else if ((Controller.Data.Buttons & (1 << Buttons::CUp)) && !(Controller.Data.Buttons & (1 << Buttons::CDown)))
		{
			return UINT16_MAX;
		}
		else
		{
			return UINT16_MAX / 2;
		}
	}

	virtual uint16_t GetJoy1X()
	{
		uint16_t value = AxisJoy1X.Parse(Controller.Data.JoystickX);

		if ((value > (Mid - Calibration::JoyDeadZoneRadius))
			&&
			(value < (Mid + Calibration::JoyDeadZoneRadius)))
		{
			return Mid;
		}
		else
		{
			return value;
		}
	}

	virtual uint16_t GetJoy1Y()
	{
		uint16_t value = AxisJoy1Y.Parse(Controller.Data.JoystickY);
		if ((value > (Mid - Calibration::JoyDeadZoneRadius))
			&&
			(value < (Mid + Calibration::JoyDeadZoneRadius)))
		{
			return Mid;
		}
		else
		{
			return value;
		}
	}

	virtual uint16_t GetTriggerL()
	{
		if (Controller.Data.Buttons & (1 << Buttons::L))
		{
			return UINT16_MAX;
		}
		else
		{
			return 0;
		}
	}

	virtual uint16_t GetTriggerR()
	{
		if (Controller.Data.Buttons & (1 << Buttons::R))
		{
			return UINT16_MAX;
		}
		else
		{
			return 0;
		}
	}

	virtual bool GetLeft() { return  Controller.Data.Buttons & (1 << Buttons::Left); }
	virtual bool GetRight() { return  Controller.Data.Buttons & (1 << Buttons::Right); }
	virtual bool GetUp() { return Controller.Data.Buttons & (1 << Buttons::Up); }
	virtual bool GetDown() { return Controller.Data.Buttons & (1 << Buttons::Down); }

	virtual bool GetButton0() { return Controller.Data.Buttons & (1 << Buttons::A); }
	virtual bool GetButton1() { return Controller.Data.Buttons & (1 << Buttons::B); }
	virtual bool GetButton4() { return Controller.Data.Buttons & (1 << Buttons::Z); }
	virtual bool GetButton5() { return Controller.Data.Buttons & (1 << Buttons::L); }
	virtual bool GetButton6() { return Controller.Data.Buttons & (1 << Buttons::R); }
	virtual bool GetButton7() { return Controller.Data.Buttons & (1 << Buttons::Start); }

	// Interface controls redirection.
	virtual bool GetButtonAccept() { return GetButton0(); }
	virtual bool GetButtonReject() { return GetButton1(); }
	virtual bool GetButtonHome() { return GetButton7(); }

protected:
	bool Callback()
	{
		if (Controller.Read())
		{
			OnControllerReadOk();
		}
		else
		{
			OnControllerFail();
		}

		Controller.Poll();


		return true;
	}
};
#endif