// DirectInputControllerTask.h

#ifndef _DIRECT_INPUT_CONTROLLER_TASK_h
#define _DIRECT_INPUT_CONTROLLER_TASK_h

#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

#include "../VirtualPadSources/DirectInputVirtualPadWriter.h"

class DirectInputVirtualPadWriterTask : private Task
{
private:
	DirectInputVirtualPadWriter* Pad;

public:
	DirectInputVirtualPadWriterTask(Scheduler* scheduler,
		DirectInputVirtualPadWriter* pad, const uint32_t updatePeriodMillis)
		: Task(updatePeriodMillis, TASK_FOREVER, scheduler, false)
		, Pad(pad)
	{}

	bool Callback()
	{
		if (Pad != nullptr)
		{
			Pad->Step();
		}

		return true;
	}

	void Start()
	{
		if (!Task::isEnabled())
		{
			if (Pad != nullptr)
			{
				Pad->Clear();
			}
			Task::enable();
		}
	}

	void Stop()
	{
		if (Pad != nullptr)
		{
			Pad->Clear();
		}
		Task::disable();
	}
};
#endif