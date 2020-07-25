// IFilterStepperDispatcherTask.h

#ifndef _IFILTER_STEPPER_DISPATCHER_TASK_h
#define _IFILTER_STEPPER_DISPATCHER_TASK_h

#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

#include <IFilterStepperTask.h>
#include <IInputController.h>


//Templated dispatcher, fires an light-weight, non-typed event to the Dispacther,
// as soon as the data is updated.
template <const uint32_t UpdatePeriod,
	const uint8_t MaxCount = 5>
	class IFilterStepperDispatcherTask : public IFilterStepperTask<UpdatePeriod, MaxCount>
{
private:
	class DispatcherTaskClass : private Task
	{

	private:
		IDispatcher* Dispatcher = nullptr;

		bool DataUpdatedPending = false;

		bool ControllerStatePending = false;
		bool ControllerState = false;

	public:
		DispatcherTaskClass(Scheduler* scheduler)
			: Task(0, TASK_FOREVER, scheduler, false)
		{}

		void SetDispatcher(IDispatcher* dispatcher)
		{
			Dispatcher = dispatcher;
			DataUpdatedPending = false;
			ControllerStatePending = false;
		}

		bool OnEnable()
		{
			return Dispatcher != nullptr;
		}

		bool Callback()
		{
			//if (Dispatcher != nullptr)
			{
				if (DataUpdatedPending)
				{
					Dispatcher->OnDataUpdated();
					DataUpdatedPending = false;

					return true;
				}
				else if (ControllerStatePending)
				{
					Dispatcher->OnStateChanged(ControllerState);
					ControllerStatePending = false;

					return true;
				}
			}

			disable();

			return false;
		}


		void OnStateChanged(const bool enabled)
		{
			ControllerStatePending = true;
			ControllerState = enabled;
			DataUpdatedPending = false;
			Task::enableIfNot();
			Task::forceNextIteration();
		}

		void OnDataUpdated()
		{
			DataUpdatedPending = true;
			Task::enableIfNot();
			Task::forceNextIteration();
		}
	};

	DispatcherTaskClass AsyncDispatcher;

public:
	IFilterStepperDispatcherTask(Scheduler* scheduler)
		: IFilterStepperTask<UpdatePeriod, MaxCount>(scheduler)
		, AsyncDispatcher(scheduler)
	{}

	void SetDispatcher(IDispatcher* dispatcher)
	{
		AsyncDispatcher.SetDispatcher(dispatcher);
	}

public:
	void OnStateChanged(const bool enabled)
	{
		AsyncDispatcher.OnStateChanged(enabled);
	}

	void OnDataUpdated()
	{
		AsyncDispatcher.OnDataUpdated();
	}

};
#endif