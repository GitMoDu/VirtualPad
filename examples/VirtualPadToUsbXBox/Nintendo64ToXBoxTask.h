#ifndef _NINTENDO_64_TO_XBOX_TASK_h
#define _NINTENDO_64_TO_XBOX_TASK_h

#include <Nintendo64Controller.h>
#include <VirtualPads.h>
#include "VirtualPadToXBoxTask.h"

template<int8_t JoyXMin = -80, int8_t JoyXMax = 80,
	int8_t JoyYMin = -80, int8_t JoyYMax = 80>
class Nintendo64ToXBoxTask : public VirtualPadToXBoxTask
{
private:
	// VirtualPad object.
	Nintendo64Controller::Nintendo64VirtualPad<JoyXMin, JoyXMax, JoyYMin, JoyYMax> Pad{};

	// Reference to raw controller state.
	Nintendo64Controller::state_data_t& ControllerState;

public:
	Nintendo64ToXBoxTask(Scheduler* scheduler, USBXBox360& x360,
		Nintendo64Controller::state_data_t& controllerState,
		const uint32_t updatePeriodMicros)
		: VirtualPadToXBoxTask(scheduler, x360, Pad, updatePeriodMicros)
		, ControllerState(controllerState)
	{
	}

protected:
	void UpdateInput() final
	{
		if (ControllerState.Connected)
		{
			Pad.Step(ControllerState);
		}
		else
		{
			Pad.Clear();
		}
	}
};
#endif

