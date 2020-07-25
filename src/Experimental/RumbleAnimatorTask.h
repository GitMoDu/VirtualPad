// RumbleAnimator.h

#ifndef _RUMBLEANIMATOR_h
#define _RUMBLEANIMATOR_h



#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

#include <IRumbleAnimator.h>
#include <RumblerDAC.h>

template<const uint8_t AnimatorMaxCount>
class RumbleSource : public IRumbleDriver
{
private:
	IRumbleAnimator* Animators[AnimatorMaxCount];
	uint8_t AnimatorCount = 0;

public:
	RumbleSource() : IRumbleDriver()
	{
		for (uint8_t i = 0; i < AnimatorMaxCount; i++)
		{
			Animators[i] = nullptr;
		}
	}
	inline uint8_t GetAnimatorCount()
	{
		return AnimatorCount;
	}

	IRumbleAnimator* GetAnimator(const uint8_t index)
	{
		if (index < AnimatorCount)
		{
			return Animators[index];
		}
		else
		{
			return nullptr;
		}
	}

	bool AddRumbleAnimator(IRumbleAnimator* rumbleAnimator)
	{
		if (rumbleAnimator != nullptr &&
			AnimatorCount < AnimatorMaxCount)
		{
			Animators[AnimatorCount] = rumbleAnimator;
			AnimatorCount++;
			return true;
		}

		return false;
	}
};

template<const uint32_t SamplePeriodMillis,
	const uint8_t AnimatorMaxCount = 10,
	const uint8_t BufferSize = 5,
	const uint32_t TargetPeriodMillis = 30>
	class RumbleAnimatorTask : Task
{
private:
	RumblerDAC<BufferSize, SamplePeriodMillis, TargetPeriodMillis> DAC;

	RumbleSource<AnimatorMaxCount> RumbleDriver;

	uint32_t SampleTimeMillis = 0;
	uint16_t Sample = 0;


	constexpr uint32_t GetAnimatorPeriodMillis()
	{
		if (SamplePeriodMillis < 1)
		{
			return 1;
		}
		else
		{
			return SamplePeriodMillis - 1;
		}
	}

public:
	RumbleAnimatorTask(Scheduler* scheduler)
		: Task(GetAnimatorPeriodMillis(), TASK_FOREVER, scheduler, false),
		DAC(),
		RumbleDriver()
	{
	}

	IRumbleDriver* GetDriver()
	{
		return &RumbleDriver;
	}

	inline IRumblerDAC* GetDAC()
	{
		return &DAC;
	}

	bool IsClipping()
	{
		return Sample > UINT8_MAX;
	}

	bool Callback()
	{
		if (DAC.IsBufferFull())
		{
			//Buffer is full, back off.
			Task::delay(1);
			return false;
		}
		else
		{
			Sample = 0;
			//Fill buffer looking forward in time.
			SampleTimeMillis = millis() + (DAC.GetBufferCount() * SamplePeriodMillis);

			for (uint8_t i = 0; i < RumbleDriver.GetAnimatorCount(); i++)
			{
				Sample += RumbleDriver.GetAnimator(i)->GetStep(SampleTimeMillis);
			}

			//if (DAC.Append(min(Sample, UINT8_MAX)))
			if (DAC.Append(Sample))
			{
				//Buffer is full, restore task to normal scheduling.
				Task::delay(0);
			}
			else
			{
				//Buffer is not full, fill in next sample.
				forceNextIteration();
			}

			return true;
		}
	}
};
#endif

