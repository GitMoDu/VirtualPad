// VirtualPadDispatchTask.h

#ifndef _IINPUT_TASK_CONTROLLER_DISPATCH_h
#define _IINPUT_TASK_CONTROLLER_DISPATCH_h

#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

#include <VirtualPad.h>

/// <summary>
/// VirtualPad Listener dispatch task.
/// Listener is called for VirtualPad timed state updates.
/// Scheduler depends on  https://github.com/arkhipenko/TaskScheduler
/// </summary>
class VirtualPadDispatchTask : private Task
{
private:
	enum class StateEnum
	{
		FastUpdate,
		NoController
	};

private:
	VirtualPad* Pad;
	VirtualPadListener* Listener;

private:
	StateEnum State = StateEnum::FastUpdate;

public:
	VirtualPadDispatchTask(Scheduler& scheduler,
		VirtualPad* pad, const uint32_t updatePeriodMillis,
		VirtualPadListener* listener = nullptr)
		: Task(updatePeriodMillis, TASK_FOREVER, &scheduler, false)
		, Pad(pad)
		, Listener(listener)
	{}

	void Start()
	{
		State = StateEnum::FastUpdate;
		Task::enable();
	}

	void Stop()
	{
		Task::disable();
	}
public:
	virtual void SetDispatch(VirtualPadListener* dispatch)
	{
		Listener = dispatch;
	}

	bool Callback()
	{
		switch (State)
		{
		case StateEnum::FastUpdate:
			if (Listener != nullptr)
			{
				Listener->OnUpdate(Pad);
			}
			if (!Pad->Connected())
			{
				State = StateEnum::NoController;
			}
			break;
		case StateEnum::NoController:
		default:
			if (Pad->Connected())
			{
				State = StateEnum::FastUpdate;
				if (Listener != nullptr)
				{
					Listener->OnUpdate(Pad);
				}
			}
			break;
		}

		return true;
	}
};
#endif