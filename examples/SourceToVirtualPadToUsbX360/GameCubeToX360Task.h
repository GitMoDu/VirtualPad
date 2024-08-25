// GameCubeToX360Task.h

#ifndef _GAMECUBE_TO_X360_TASK_h
#define _GAMECUBE_TO_X360_TASK_h

#include <GameCubeController.h>
#include <VirtualPadSources.h>
#include <VirtualPadTaskSinks.h>

template<int8_t Joy1XMin = -110, int8_t Joy1XMax = 110,
	int8_t Joy1YMin = -110, int8_t Joy1YMax = 110,
	int8_t Joy2XMin = -110, int8_t Joy2XMax = 110,
	int8_t Joy2YMin = -110, int8_t Joy2YMax = 110,
	uint8_t L2Min = 40, int8_t L2Max = 100,
	uint8_t R2Min = 40, int8_t R2Max = 100>
class GameCubeToX360Task : public VirtualPadToX360Task
{
private:
	// VirtualPad object.
	GameCubeVirtualPadWriter<Joy1XMin, Joy1XMax, Joy1YMin, Joy1YMax,
		Joy2XMin, Joy2XMax, Joy2YMin, Joy2YMax,
		L2Min, L2Max,
		R2Min, R2Max> Pad{};

	// Reference to raw controller state.
	GameCubeController::state_data_t& ControllerState;

public:
	GameCubeToX360Task(Scheduler* scheduler, USBXBox360& x360,
		GameCubeController::state_data_t& controllerState,
		const uint32_t updatePeriodMicros)
		: VirtualPadToX360Task(scheduler, x360, &Pad, updatePeriodMicros)
		, ControllerState(controllerState)
	{}

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

