// ControllerRumbleTester.h

#ifndef _CONTROLLERRUMBLETESTER_h
#define _CONTROLLERRUMBLETESTER_h


#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

#include <IRumbleAnimator.h>

class ControllerRumbleTester : public Task
{
private:
	TemplatePolyphonicAnimator<PulseAnimator<false>, 10> Pulser;
	TemplatePolyphonicAnimator<SawAnimator<false>, 10> Sawer;
	TemplatePolyphonicAnimator<TriangleAnimator<false>, 10> Triangle;
	TemplatePolyphonicAnimator<SquareAnimator<false>, 10> Squarer;

public:
	ControllerRumbleTester(Scheduler* scheduler)
		: Task(500, TASK_FOREVER, scheduler, false)
	{

	}

	bool Setup(IRumbleDriver* rumbleDriver)
	{
		if (rumbleDriver->AddRumbleAnimator(&Pulser) &&
			rumbleDriver->AddRumbleAnimator(&Sawer) &&
			rumbleDriver->AddRumbleAnimator(&Triangle) &&
			rumbleDriver->AddRumbleAnimator(&Squarer))
		{
			enableIfNot();

			return true;
		}

		return false;
	}

	bool Callback()
	{
		Pulser.Play(70, 300, 0, 0);
		Sawer.Play(100, 300, 0, 0);

		return true;
	}
};


#endif

