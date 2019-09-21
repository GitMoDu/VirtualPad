// IFilterStepperDispatcherTask.h

#ifndef _IFILTER_STEPPER_DISPATCHER_TASK_h
#define _IFILTER_STEPPER_DISPATCHER_TASK_h

#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

#include <IFilterTaskInclude.h>
#include <IInputController.h>


//Templated dispatcher, fires an light-weight, non-typed event to the Dispacther,
// as soon as the data is updated.
template <const uint32_t UpdatePeriod,
	const uint8_t MaxCount = 5>
	class IFilterStepperDispatcherTask : public IFilterStepperTask<UpdatePeriod, MaxCount>
{

protected:
	IDispatcher* Dispatcher = nullptr;

public:
	IFilterStepperDispatcherTask(Scheduler* scheduler, IDispatcher* dispatcher = nullptr)
		: IFilterStepperTask<UpdatePeriod, MaxCount>(scheduler, false)
	{
		Dispatcher = dispatcher;
	}

	void SetDispatcher(IDispatcher* dispatcher)
	{
		Dispatcher = dispatcher;
		this->enableIfNot();
	}

protected:
	void OnDataUpdated()
	{
		if (Dispatcher != nullptr)
		{
			Dispatcher->OnDataUpdated();
		}
	}
};
#endif