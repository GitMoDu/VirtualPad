// IInputTaskControllerMap.h

#ifndef _IINPUT_TASK_CONTROLLER_MAP_h
#define _IINPUT_TASK_CONTROLLER_MAP_h

#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

/// <summary>
/// Assumes InputControllerType has a call for MapControllerData(InputStateType&).
/// </summary>
/// <typeparam name="InputControllerType"></typeparam>
/// <typeparam name="InputStateType"></typeparam>
template<typename SourceDataType,
	typename InputControllerType>
class IInputTaskControllerMap : private Task
{
private:
	// Reference to raw controller state.
	SourceDataType& ControllerState;
	
	// Reference to typed IInputController.
	InputControllerType& InputController;

public:
	IInputTaskControllerMap(Scheduler& scheduler,
		SourceDataType& controllerState,
		InputControllerType& inputController,
		const uint32_t updatePeriodMicros)
		: Task(TASK_IMMEDIATE, TASK_FOREVER, &scheduler, false)
		, ControllerState(controllerState)
		, InputController(inputController)
	{}

	void Start()
	{
		Task::enable();
	}

	void Stop()
	{
		Task::disable();
	}

	bool Callback() final
	{
		if (ControllerState.Connected)
		{
			InputController.MapControllerData(ControllerState);
		}
		else
		{
			InputController.Clear();
		}

		return true;
	}
};
#endif