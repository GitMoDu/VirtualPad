// Nintendo64ToX360Task.h

#ifndef _NINTENDO_64_TO_X360_TASK_h
#define _NINTENDO_64_TO_X360_TASK_h

#include <Nintendo64Controller.h>
#include <VirtualPadSources.h>
#include <VirtualPadTaskSinks.h>

template<int8_t JoyXMin = -80, int8_t JoyXMax = 80,
	int8_t JoyYMin = -80, int8_t JoyYMax = 80>
class Nintendo64ToX360Task : public VirtualPadToX360Task
{
private:
	// VirtualPad object.
	Nintendo64VirtualPadWriter<JoyXMin, JoyXMax, JoyYMin, JoyYMax> Pad{};

	// Reference to raw controller state.
	Nintendo64Controller::state_data_t& ControllerState;

public:
	Nintendo64ToX360Task(Scheduler* scheduler, USBXBox360& x360,
		Nintendo64Controller::state_data_t& controllerState,
		const uint32_t updatePeriodMicros)
		: VirtualPadToX360Task(scheduler, x360, &Pad, updatePeriodMicros)
		, ControllerState(controllerState)
	{
	}

protected:
	void UpdateInput() final
	{
		if (ControllerState.Connected)
		{
			Pad.MapControllerData(ControllerState);
		}
		else
		{
			Pad.Clear();
		}
	}
};
#endif

