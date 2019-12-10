// InputController.h

#ifndef _INPUTCONTROLLER_h
#define _INPUTCONTROLLER_h

#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>


#include <EMAFilter.h>
#include <IFilterTaskInclude.h>
#include <AxisTemplates.h>
#include <IInputController.h>
#include <IFilterStepperDispatcherTask.h>


#include <IRumblerDAC.h>
#include <IRumbleAnimator.h>
#include <RumbleAnimatorTask.h>
#include <RumbleAnimatorTask.h>

template<const uint32_t SamplePeriodMillis = 5,
	const uint32_t FilterPeriodMillis = 8>
	class TemplateControllerTask : public Task, public virtual IInputController
{
protected:
	EMAFilter FilterJoy1X;
	EMAFilter FilterJoy1Y;
	EMAFilter FilterJoy2X;
	EMAFilter FilterJoy2Y;

	EMAFilter FilterTriggerL;
	EMAFilter FilterTriggerR;
	static const uint8_t FilterCount = 6;

	RumbleAnimatorTask<SamplePeriodMillis> RumbleAnimator;

	bool Connected = false;

	const uint32_t ReconnectPeriodMillis = 50;

private:
	IFilterStepperDispatcherTask<FilterPeriodMillis, FilterCount> StepperTask;

public:
	TemplateControllerTask(Scheduler* scheduler, IDispatcher* dispatcher = nullptr)
		: Task(SamplePeriodMillis, TASK_FOREVER, scheduler, false)
		, IInputController()
		, StepperTask(scheduler, dispatcher)
		, RumbleAnimator(scheduler)
		, FilterJoy1X()
		, FilterJoy1Y()
		, FilterJoy2X()
		, FilterJoy2Y()
		, FilterTriggerL()
		, FilterTriggerR()
	{
		StepperTask.AddFilter(FilterJoy1X);
		StepperTask.AddFilter(FilterJoy1Y);
		StepperTask.AddFilter(FilterJoy2X);
		StepperTask.AddFilter(FilterJoy2Y);
		StepperTask.AddFilter(FilterTriggerL);
		StepperTask.AddFilter(FilterTriggerR);

		Connected = false;
	}

	virtual void OnDisable()
	{
		Connected = false;
		StopForceFeedback();

#ifdef DEBUG_IINPUT_CONTROLLER
		Serial.println(F("Controller disabled."));
#endif
		PauseStepper();
		StepperTask.OnStateChanged(false);
	}

	virtual bool OnEnable()
	{
		Connected = false;
		StepperTask.enableIfNot();

		return true;
	}

	virtual bool Callback()
	{
		return false;
	}

	void SetDispatcher(IDispatcher* dispatcher)
	{
		StepperTask.SetDispatcher(dispatcher);
	}

	inline uint16_t GetJoy1X()
	{
		return FilterJoy1X.Get();
	}

	inline uint16_t GetJoy1Y()
	{
		return FilterJoy1Y.Get();
	}

	inline uint16_t GetJoy2X()
	{
		return FilterJoy2X.Get();
	}

	inline uint16_t GetJoy2Y()
	{
		return FilterJoy2Y.Get();
	}

	inline uint16_t GetTriggerL()
	{
		return FilterTriggerL.Get();
	}

	inline uint16_t GetTriggerR()
	{
		return FilterTriggerR.Get();
	}

	IRumbleDriver* GetDriver()
	{
		return RumbleAnimator.GetDriver();
	}


protected:
	void Enable()
	{
		enableIfNot();
	}

	void Disable()
	{
		disable();
	}

	void Delay(const uint32_t millis)
	{
		Task::delay(millis);
	}

	void PauseStepper()
	{
		StepperTask.disable();
	}

	void OnControllerReadOk()
	{
		if (!Connected)
		{
			Connected = true;
#ifdef DEBUG_IINPUT_CONTROLLER
			Serial.println(F("Controller plugged in"));
#endif
			StepperTask.enableIfNot();
			StepperTask.OnStateChanged(true);
			Delay(0);
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
			PauseStepper();
			StepperTask.OnStateChanged(false);
		}

		Delay(ReconnectPeriodMillis);
	}

protected:
	virtual void StopForceFeedback() {}
};
#endif

