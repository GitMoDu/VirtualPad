// ControllerTaskTemplates.h

#ifndef _CONTROLLER_TASK_TEMPLATE_h
#define _CONTROLLER_TASK_TEMPLATE_h

#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

#include <IInputController.h>

template<const uint32_t UpdatePeriodMillis = 20>
class ControllerTaskTemplate : protected Task, public virtual IInputController
{
protected:
	bool Connected = false;

	IDispatcher* Dispatcher = nullptr;

public:
	ControllerTaskTemplate(Scheduler* scheduler)
		: Task(UpdatePeriodMillis, TASK_FOREVER, scheduler, false)
		, IInputController()
	{
		Connected = false;
	}

	bool IsConnected()
	{
		return Connected;
	}

	virtual void OnDisable()
	{
		Connected = false;

#ifdef DEBUG_IINPUT_CONTROLLER
		Serial.println(F("Controller disabled."));
#endif
		if (Dispatcher != nullptr)
		{
			Dispatcher->OnStateChanged(false);
		}
	}

	virtual bool OnEnable()
	{
		Connected = false;

		return true;
	}

	virtual void StartController()
	{
		Task::enableIfNot();
	}

	virtual void StopController()
	{
		if (Connected)
		{
			if (Dispatcher != nullptr)
			{
				Dispatcher->OnStateChanged(true);
			}
		}

		Task::disable();
	}

	virtual bool Callback()
	{
		return false;
	}

	void SetDispatcher(IDispatcher* dispatcher)
	{
		Dispatcher = dispatcher;
	}

protected:
	void Enable()
	{
		Task::enableIfNot();
	}

	void Disable()
	{
		Task::disable();
	}

	void Delay(const uint32_t millis)
	{
		Task::delay(millis);
	}

	void OnControllerReadOk()
	{
		if (!Connected)
		{
			Connected = true;
#ifdef DEBUG_IINPUT_CONTROLLER
			Serial.println(F("Controller plugged in"));
#endif
			if (Dispatcher != nullptr)
			{
				Dispatcher->OnStateChanged(true);
			}
		}
		else
		{
			if (Dispatcher != nullptr)
			{
				Dispatcher->OnDataUpdated();
			}
		}
	}

	void OnControllerFail()
	{
		if (Connected)
		{
			Connected = false;
#ifdef DEBUG_IINPUT_CONTROLLER
			Serial.println(F("Error reading controller"));
#endif
			if (Dispatcher != nullptr)
			{
				Dispatcher->OnStateChanged(false);
			}
		}
	}
};
#endif