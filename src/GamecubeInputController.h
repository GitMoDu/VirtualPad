// GamecubeInputController.h

#ifndef _GAMECUBE_INPUT_CONTROLLER_h
#define _GAMECUBE_INPUT_CONTROLLER_h


#include <InputControllerTaskTemplates.h>
#include <AxisTemplates.h>

#include <GamecubeControllerReaderSTM32.h>



// With an official controller attached, there is a typical interval of about 6ms between successive updates.
// Each update lasts around 348us.
// in http://www.int03.co.uk/crema/hardware/gamecube/gc-control.html



template<
	typename Calibration,
	const uint8_t Pin,
	const uint32_t SamplePeriodMillis = 5,
	const uint32_t UpdatePeriodMillis = 5>
	class GamecubeControllerCarInputControllerTask : public TemplateControllerTask<SamplePeriodMillis, UpdatePeriodMillis>
{
private:
	//Template members for use in this class.
	using IInputController::DACOutput;
	using IInputController::Delay;
	using IInputController::Enable;
	using TemplateControllerTask<SamplePeriodMillis, UpdatePeriodMillis>::OnControllerReadOk;
	using TemplateControllerTask<SamplePeriodMillis, UpdatePeriodMillis>::OnControllerFail;


	using TemplateControllerTask<SamplePeriodMillis, UpdatePeriodMillis>::FilterJoy1X;
	using TemplateControllerTask<SamplePeriodMillis, UpdatePeriodMillis>::FilterJoy1Y;
	using TemplateControllerTask<SamplePeriodMillis, UpdatePeriodMillis>::FilterJoy2X;
	using TemplateControllerTask<SamplePeriodMillis, UpdatePeriodMillis>::FilterJoy2Y;

	using TemplateControllerTask<SamplePeriodMillis, UpdatePeriodMillis>::FilterTriggerL;
	using TemplateControllerTask<SamplePeriodMillis, UpdatePeriodMillis>::FilterTriggerR;

	using TemplateControllerTask<SamplePeriodMillis, UpdatePeriodMillis>::RumbleAnimator;
	//

	//Constants.
	enum GamecubeButtons : uint8_t
	{
		A = 0,
		B = 1,
		X = 2,
		Y = 3,
		Start = 4,
		Left = 8,
		Right = 9,
		Down = 10,
		Up = 11,
		Z = 12,
		R = 13,
		L = 14
	};

	//Template calibrations.
	AxisCentered<uint8_t, uint16_t, Calibration::JoyXMin, Calibration::JoyXMax, Calibration::JoyXOffset, Calibration::JoyDeadZoneRadius, 0, UINT16_MAX> AxisJoy1X;
	AxisCentered<uint8_t, uint16_t, Calibration::JoyYMin, Calibration::JoyYMax, Calibration::JoyYOffset, Calibration::JoyDeadZoneRadius, 0, UINT16_MAX> AxisJoy1Y;

	AxisCentered<uint8_t, uint16_t, Calibration::JoyCXMin, Calibration::JoyCXMax, Calibration::JoyCXOffset, Calibration::JoyCDeadZoneRadius, 0, UINT16_MAX> AxisJoy2X;
	AxisCentered<uint8_t, uint16_t, Calibration::JoyCYMin, Calibration::JoyCYMax, Calibration::JoyCYOffset, Calibration::JoyCDeadZoneRadius, 0, UINT16_MAX> AxisJoy2Y;

	AxisLinear<uint8_t, uint16_t, Calibration::TriggerLMin, Calibration::TriggerLMax, Calibration::TriggerLDeadZone, 0, UINT16_MAX > AxisTriggerL;
	AxisLinear<uint8_t, uint16_t, Calibration::TriggerRMin, Calibration::TriggerRMax, Calibration::TriggerRDeadZone, 0, UINT16_MAX> AxisTriggerR;

	//Controller reader.
	GameCubeController Controller;

public:
	GamecubeControllerCarInputControllerTask(Scheduler* scheduler, IDispatcher* dispatcher = nullptr)
		: TemplateControllerTask<SamplePeriodMillis, UpdatePeriodMillis>(scheduler, dispatcher)
		, Controller(Pin)
	{
		FilterJoy1X.SetSaturation(Calibration::JoyFilterSaturation);
		FilterJoy1Y.SetSaturation(Calibration::JoyFilterSaturation);
		FilterJoy2X.SetSaturation(Calibration::JoyCFilterSaturation);
		FilterJoy2Y.SetSaturation(Calibration::JoyCFilterSaturation);
		FilterTriggerL.SetSaturation(Calibration::TriggerLFilterSaturation);
		FilterTriggerR.SetSaturation(Calibration::TriggerRFilterSaturation);
	}

	bool Setup()
	{
		if (Controller.begin())
		{
			Enable();

			return true;
		}
		return false;
	}

	virtual void GetDirection(bool& left, bool& right, bool& up, bool& down)
	{
		left = RawData.buttons & (1 << GamecubeButtons::Left);
		right = RawData.buttons & (1 << GamecubeButtons::Right);
		up = RawData.buttons & (1 << GamecubeButtons::Up);
		down = RawData.buttons & (1 << GamecubeButtons::Down);

	}
	virtual bool GetButton0() { return RawData.buttons & (1 << GamecubeButtons::A); }
	virtual bool GetButton1() { return RawData.buttons & (1 << GamecubeButtons::B); }
	virtual bool GetButton2() { return RawData.buttons & (1 << GamecubeButtons::X); }
	virtual bool GetButton3() { return RawData.buttons & (1 << GamecubeButtons::Y); }
	virtual bool GetButton4() { return RawData.buttons & (1 << GamecubeButtons::Z); }
	virtual bool GetButton5() { return RawData.buttons & (1 << GamecubeButtons::L); }
	virtual bool GetButton6() { return RawData.buttons & (1 << GamecubeButtons::R); }
	virtual bool GetButton7() { return RawData.buttons & (1 << GamecubeButtons::Start); }

private:
	GameCubeData_t RawData;

protected:
	bool Callback()
	{
#ifdef DEBUG_IINPUT_CONTROLLER
		DACOutput = RumbleAnimator.GetDAC()->Output();
		if (Controller.readWithRumble(&RawData, DACOutput))
#else
		if (Controller.readWithRumble(&RawData, RumbleAnimator.GetDAC()->Output()))
#endif
		{
			OnControllerReadOk();
		}
		else
		{
			OnControllerFail();
		}

		FilterJoy1X.Set(AxisJoy1X.Parse(RawData.joystickX));
		FilterJoy1Y.Set(AxisJoy1Y.Parse(RawData.joystickY));
		FilterJoy2X.Set(AxisJoy2X.Parse(RawData.cX));
		FilterJoy2Y.Set(AxisJoy2Y.Parse(RawData.cY));
		FilterTriggerL.Set(AxisTriggerL.Parse(RawData.shoulderLeft));
		FilterTriggerR.Set(AxisTriggerR.Parse(RawData.shoulderRight));

		return true;
	}

	void StopForceFeedback()
	{
		// Send command to stop rumble.
		Controller.readWithRumble(&RawData, false);
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

