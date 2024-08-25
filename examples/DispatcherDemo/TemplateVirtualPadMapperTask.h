// TemplateVirtualPadMapperTask.h

#ifndef _TEMPLATEVIRTUALPADMAPPERTASK_h
#define _TEMPLATEVIRTUALPADMAPPERTASK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


/// <summary>
/// Assumes SourceDataType is a data type and that
/// InputControllerType has a call for MapControllerData(InputStateType&).
/// </summary>
/// <typeparam name="InputControllerType"></typeparam>
/// <typeparam name="InputStateType"></typeparam>
template<typename SourceDataType,
	typename InputControllerType>
class TemplateVirtualPadMapperTask : private Task
{
private:
	// Reference to raw controller state.
	SourceDataType& ControllerState;

	// Reference to typed IInputController.
	InputControllerType& InputController;

public:
	TemplateVirtualPadMapperTask(Scheduler& scheduler,
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

