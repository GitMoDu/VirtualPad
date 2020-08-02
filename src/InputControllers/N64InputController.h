// N64InputController.h

#ifndef _N64_INPUT_CONTROLLER_h
#define _N64_INPUT_CONTROLLER_h


#include <ControllerTaskTemplateInclude.h>

// https://github.com/GitMoDu/NintendoControllerReaderSTM32
#include <NintendoControllerReaderSTM32.h>


template<
	typename Calibration,
	const uint8_t Pin,
	const uint32_t UpdatePeriodMillis = 3>
	class N64ControllerTask : public ControllerTaskTemplate<UpdatePeriodMillis>
{
private:
	static const uint16_t Mid = (UINT16_MAX / 2);

	// Template calibrations.
	AxisCentered<int8_t, uint16_t, Calibration::JoyXMin, Calibration::JoyXMax, Calibration::JoyXOffset, Calibration::JoyDeadZoneRadius, 0, UINT16_MAX> AxisJoy1X;
	AxisCentered<int8_t, uint16_t, Calibration::JoyYMin, Calibration::JoyYMax, Calibration::JoyYOffset, Calibration::JoyDeadZoneRadius, 0, UINT16_MAX> AxisJoy1Y;

	// Controller reader.
	N64Controller Controller;

	// Raw controller data.
	N64Data_t RawData;

	//Template members for use in this class.
	using ControllerTaskTemplate<UpdatePeriodMillis>::Delay;
	using ControllerTaskTemplate<UpdatePeriodMillis>::Enable;
	using ControllerTaskTemplate<UpdatePeriodMillis>::OnControllerReadOk;
	using ControllerTaskTemplate<UpdatePeriodMillis>::OnControllerFail;

	// Short hand for buttons enumerator.
	using N64Buttons = N64Controller::N64Buttons;

public:
	N64ControllerTask(Scheduler* scheduler)
		: ControllerTaskTemplate<UpdatePeriodMillis>(scheduler)
		, Controller(Pin)
	{
		RawData.JoystickX = AxisJoy1X.GetCenter();
		RawData.JoystickY = AxisJoy1Y.GetCenter();
	}

	virtual void GetDirection(bool& left, bool& right, bool& up, bool& down)
	{
		left = RawData.Buttons & (1 << N64Buttons::Left);
		right = RawData.Buttons & (1 << N64Buttons::Right);
		up = RawData.Buttons & (1 << N64Buttons::Up);
		down = RawData.Buttons & (1 << N64Buttons::Down);
	}

	virtual uint16_t GetJoy2X()
	{
		if ((RawData.Buttons & (1 << N64Buttons::CLeft)) && !(RawData.Buttons & (1 << N64Buttons::CRight)))
		{
			return 0;
		}
		else if ((RawData.Buttons & (1 << N64Buttons::CRight)) && !(RawData.Buttons & (1 << N64Buttons::CLeft)))
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
		if ((RawData.Buttons & (1 << N64Buttons::CDown)) && !(RawData.Buttons & (1 << N64Buttons::CUp)))
		{
			return 0;
		}
		else if ((RawData.Buttons & (1 << N64Buttons::CUp)) && !(RawData.Buttons & (1 << N64Buttons::CDown)))
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
		uint16_t value = AxisJoy1X.Parse(RawData.JoystickX);

		if ((value > (Mid - Calibration::JoyDeadZoneRadius))
			&&
			(value < (Mid + Calibration::JoyDeadZoneRadius)))
		{
			return value;
		}
		else
		{
			return Mid;
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
			return Mid;
		}
	}

	virtual uint16_t GetTriggerL()
	{
		if (RawData.Buttons & (1 << N64Buttons::L))
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
		if (RawData.Buttons & (1 << N64Buttons::R))
		{
			return UINT16_MAX;
		}
		else
		{
			return 0;
		}
	}

	virtual bool GetLeft() { return  RawData.Buttons & (1 << N64Buttons::Left); }
	virtual bool GetRight() { return  RawData.Buttons & (1 << N64Buttons::Right); }
	virtual bool GetUp() { return RawData.Buttons & (1 << N64Buttons::Up); }
	virtual bool GetDown() { return RawData.Buttons & (1 << N64Buttons::Down); }

	virtual bool GetButton0() { return RawData.Buttons & (1 << N64Buttons::A); }
	virtual bool GetButton1() { return RawData.Buttons & (1 << N64Buttons::B); }
	virtual bool GetButton4() { return RawData.Buttons & (1 << N64Buttons::Z); }
	virtual bool GetButton5() { return RawData.Buttons & (1 << N64Buttons::L); }
	virtual bool GetButton6() { return RawData.Buttons & (1 << N64Buttons::R); }
	virtual bool GetButton7() { return RawData.Buttons & (1 << N64Buttons::Start); }

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
};
#endif