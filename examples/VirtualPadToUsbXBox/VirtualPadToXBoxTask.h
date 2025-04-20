#ifndef _VIRTUAL_PAD_TO_XBOX_TASK_h
#define _VIRTUAL_PAD_TO_XBOX_TASK_h

#if defined(ARDUINO_ARCH_STM32F1)
#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

#include <USBComposite.h>

#include <VirtualPad.h>


/// <summary>
/// VirtualPad exposed as native composite USB controller(XBox 360).
/// 
/// Maps VirtualPad using declared controller features.
/// Exposes virtual call to update VirtualPad before mapping USB update.
///
/// Designed for STM32 using ArduinoSTM32(https://github.com/rogerclarkmelbourne/Arduino_STM32)
/// USB interface depends on https://github.com/arpruss/USBComposite_stm32f1
/// Scheduler depends on  https://github.com/arkhipenko/TaskScheduler
/// </summary>
class VirtualPadToXBoxTask : private Task
{
private:
	enum class StateEnum
	{
		UpdateInput,
		MapController,
		Synchronizing
	};

protected:
	// VirtualPad state.
	VirtualPad::analog_pad_state_t& Pad;

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
	VirtualPadToXBoxTask(Scheduler* scheduler, USBXBox360& x360, VirtualPad::analog_pad_state_t& virtualPad, const uint32_t updatePeriodMicros)
		: Task(TASK_IMMEDIATE, TASK_FOREVER, scheduler, false)
		, Pad(virtualPad)
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
		Buttons |= (uint16_t)(Pad.DPadRight()) << (XBOX_DRIGHT - 1);
		Buttons |= (uint16_t)(Pad.DPadLeft()) << (XBOX_DLEFT - 1);
		Buttons |= (uint16_t)(Pad.DPadDown()) << (XBOX_DDOWN - 1);
		Buttons |= (uint16_t)(Pad.DPadUp()) << (XBOX_DUP - 1);

		// Map face buttons
		Buttons |= (uint16_t)(Pad.A()) << (XBOX_A - 1);
		Buttons |= (uint16_t)(Pad.B()) << (XBOX_B - 1);
		Buttons |= (uint16_t)(Pad.X()) << (XBOX_X - 1);
		Buttons |= (uint16_t)(Pad.Y()) << (XBOX_Y - 1);

		// Map L and R to shoulder buttons.
		Buttons |= (uint16_t)(Pad.L1()) << (XBOX_LSHOULDER - 1);
		Buttons |= (uint16_t)(Pad.R1()) << (XBOX_RSHOULDER - 1);

		// Map clickable joysticks.
		Buttons |= (uint16_t)(Pad.L3()) << (XBOX_L3 - 1);
		Buttons |= (uint16_t)(Pad.R3()) << (XBOX_R3 - 1);

		// Map Menu buttons.
		Buttons += (uint16_t)(Pad.Start()) << (XBOX_START - 1);
		Buttons += (uint16_t)(Pad.Select()) << (XBOX_BACK - 1);
		Buttons += (uint16_t)(Pad.Home()) << (XBOX_GUIDE - 1);
		Buttons += (uint16_t)(Pad.Share()) << (XBOX_GUIDE - 1);

		X360.buttons(Buttons);

		X360.sliderLeft(Pad.L2() >> 8);
		X360.sliderRight(Pad.R2() >> 8);

		X360.position(Pad.Joy1X(), Pad.Joy1Y());

		X360.positionRight(Pad.Joy2X(), Pad.Joy2Y());
		X360.send();
	}
};
#endif
#endif