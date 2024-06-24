// IInputTaskControllerDispatch.h

#ifndef _IINPUT_TASK_CONTROLLER_DISPATCH_h
#define _IINPUT_TASK_CONTROLLER_DISPATCH_h

#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

#include "../IInputController.h"

class IInputControllerDispatch
{
public:
	virtual void OnControllerUpdate(IInputController* controller) {}
};

/// <summary>
/// IInputController dispatch task.
/// Dispatch is a callback for controller state update.
/// Scheduler depends on  https://github.com/arkhipenko/TaskScheduler
/// </summary>
class IInputTaskControllerDispatch : private Task
{
private:
	enum class StateEnum
	{
		FastUpdate,
		NoController
	};

private:
	IInputController* InputController;
	IInputControllerDispatch* Dispatch;

private:
	StateEnum State = StateEnum::FastUpdate;

protected:
	virtual void UpdateInput() {}

public:
	IInputTaskControllerDispatch(Scheduler* scheduler,
		IInputController* inputController, const uint32_t updatePeriodMillis,
		IInputControllerDispatch* dispatch = nullptr)
		: Task(updatePeriodMillis, TASK_FOREVER, scheduler, false)
		, InputController(inputController)
		, Dispatch(dispatch)
	{}

	void Start()
	{
		Task::enable();
	}

	void Stop()
	{
		Task::disable();
	}
public:
	virtual void SetDispatch(IInputControllerDispatch* dispatch)
	{
		Dispatch = dispatch;
	}

	bool Callback()
	{
		switch (State)
		{
		case StateEnum::FastUpdate:
			if (Dispatch != nullptr)
			{
				Dispatch->OnControllerUpdate(InputController);
			}
			if (!InputController->Connected())
			{
				State = StateEnum::NoController;
			}
			break;
		case StateEnum::NoController:
		default:
			if (InputController->Connected())
			{
				State = StateEnum::FastUpdate;
				if (Dispatch != nullptr)
				{
					Dispatch->OnControllerUpdate(InputController);
				}
			}
			break;
		}

		return true;
	}
};
#endif