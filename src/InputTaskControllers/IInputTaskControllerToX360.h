// IInputTaskControllerToX360.h

#ifndef _IINPUT_CONTROLLER_TO_X360_TASK_h
#define _IINPUT_CONTROLLER_TO_X360_TASK_h

#if defined(ARDUINO_ARCH_STM32F1)
#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

#include <USBComposite.h>

/// <summary>
/// Abstract IInputController to USB-XBox360 controller task.
/// Maps IInputController using declared controller features.
/// Exposes virtual call to update InputController before mapping USB update.
/// Example IInputController exposed ash native composite USB controller(XBox 360).
/// Designed for STM32 using ArduinoSTM32(https://github.com/rogerclarkmelbourne/Arduino_STM32)
/// USB interface depends on https://github.com/arpruss/USBComposite_stm32f1
/// Scheduler depends on  https://github.com/arkhipenko/TaskScheduler
/// </summary>
class IInputTaskControllerToX360 : private Task
{
private:
	enum class StateEnum
	{
		UpdateInput,
		MapController,
		Synchronizing
	};

protected:
	// IInputController object.
	IInputController* InputController;

private:
	// XBox 360 Controller instance.
	USBXBox360& X360;

	const uint32_t UpdatePeriodMicros;

private:
	uint32_t LastUpdate = 0;
	StateEnum State = StateEnum::UpdateInput;

protected:
	virtual void UpdateInput() {}

public:
	IInputTaskControllerToX360(Scheduler* scheduler, USBXBox360& x360, IInputController* inputController, const uint32_t updatePeriodMicros)
		: Task(TASK_IMMEDIATE, TASK_FOREVER, scheduler, false)
		, InputController(inputController)
		, X360(x360)
		, UpdatePeriodMicros(updatePeriodMicros)
	{
		X360.setManualReportMode(true);
	}

	void Start()
	{
		Task::enable();
	}

	void Stop()
	{
		Task::disable();
	}

	bool Callback()
	{
		const uint32_t timestamp = micros();

		switch (State)
		{
		case StateEnum::UpdateInput:
			UpdateInput();
			Task::delay(0);
			State = StateEnum::MapController;
			break;
		case StateEnum::MapController:
			MapController();
			Task::delay(0);
			State = StateEnum::Synchronizing;
			break;
		case StateEnum::Synchronizing:
		default:
			if (timestamp - LastUpdate >= UpdatePeriodMicros)
			{
				LastUpdate = timestamp;
				Task::delay(0);
				State = StateEnum::UpdateInput;
			}
			else if ((micros() - LastUpdate + 1001) < UpdatePeriodMicros)
			{
				Task::delay(1);
			}
			else
			{
				Task::delay(0);
			}
			break;
		}

		return true;
	}

	void MapController()
	{
		uint16_t Buttons = 0;

		// Map D-Pad.
		Buttons |= (uint16_t)(InputController->DPadRight()) << (XBOX_DRIGHT - 1);
		Buttons |= (uint16_t)(InputController->DPadLeft()) << (XBOX_DLEFT - 1);
		Buttons |= (uint16_t)(InputController->DPadDown()) << (XBOX_DDOWN - 1);
		Buttons |= (uint16_t)(InputController->DPadUp()) << (XBOX_DUP - 1);

		// Map face buttons
		Buttons |= (uint16_t)(InputController->A()) << (XBOX_A - 1);
		Buttons |= (uint16_t)(InputController->B()) << (XBOX_B - 1);
		Buttons |= (uint16_t)(InputController->X()) << (XBOX_X - 1);
		Buttons |= (uint16_t)(InputController->Y()) << (XBOX_Y - 1);

		// Map L and R to shoulder buttons.
		Buttons |= (uint16_t)(InputController->L1()) << (XBOX_LSHOULDER - 1);
		Buttons |= (uint16_t)(InputController->R1()) << (XBOX_RSHOULDER - 1);

		// Map clicable joysticks.
		Buttons |= (uint16_t)(InputController->L3()) << (XBOX_L3 - 1);
		Buttons |= (uint16_t)(InputController->R3()) << (XBOX_R3 - 1);

		// Map Menu buttons.
		Buttons += (uint16_t)(InputController->Start()) << (XBOX_START - 1);
		if (InputController->FeatureSelect())
		{
			Buttons += (uint16_t)(InputController->Select()) << (XBOX_BACK - 1);
		}
		if (InputController->FeatureHome())
		{
			Buttons += (uint16_t)(InputController->Home()) << (XBOX_GUIDE - 1);
		}
		if (InputController->FeatureShare())
		{
			Buttons += (uint16_t)(InputController->Share()) << (XBOX_GUIDE - 1);
		}

		X360.buttons(Buttons);

		X360.sliderLeft(InputController->L2() >> 8);
		X360.sliderRight(InputController->R2() >> 8);

		X360.position(InputController->Joy1X(), InputController->Joy1Y());

		if (InputController->FeatureJoy2())
		{
			X360.positionRight(InputController->Joy2X(), InputController->Joy2Y());
		}

		X360.send();
	}
};
#endif
#endif