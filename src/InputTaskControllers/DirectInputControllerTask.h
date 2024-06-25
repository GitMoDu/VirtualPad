// DirectInputControllerTask.h

#ifndef _DIRECT_INPUT_CONTROLLER_TASK_h
#define _DIRECT_INPUT_CONTROLLER_TASK_h

#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

#include <InputControllers.h>

class DirectInputControllerTask : private Task
{
private:
	DirectInputController* InputController;

public:
	DirectInputControllerTask(Scheduler* scheduler,
		DirectInputController* inputController, const uint32_t updatePeriodMillis)
		: Task(updatePeriodMillis, TASK_FOREVER, scheduler, false)
		, InputController(inputController)
	{}

	bool Callback()
	{
		if (InputController != nullptr)
		{
			InputController->Step();
		}

		return true;
	}

	void Start()
	{
		if (!Task::isEnabled())
		{
			if (InputController != nullptr)
			{
				InputController->Clear();
			}
			Task::enable();
		}
	}

	void Stop()
	{
		if (InputController != nullptr)
		{
			InputController->Clear();
		}
		Task::disable();
	}
};

#endif