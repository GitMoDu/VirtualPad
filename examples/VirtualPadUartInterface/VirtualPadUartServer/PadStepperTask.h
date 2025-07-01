#ifndef _PAD_STEPPER_TASK_h
#define _PAD_STEPPER_TASK_h

#define _TASK_OO_CALLBACKS
#include <TSchedulerDeclarations.hpp>

// Template task to update the Pad state from update_pad_t::Step().
template<typename update_pad_t>
class PadStepperTask : private TS::Task
{
private:
	update_pad_t& Pad;

public:
	PadStepperTask(TS::Scheduler& scheduler, update_pad_t& pad)
		: TS::Task(10, TASK_FOREVER, &scheduler, true)
	{
	}

	bool Callback() final
	{
		Pad.Step();

		if (Pad.Connected())
		{
			Pad.SetConnected(true);
		}
	}
};

#endif

