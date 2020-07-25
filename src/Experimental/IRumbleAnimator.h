// IRumbleAnimator.h

#ifndef _IRUMBLEANIMATOR_h
#define _IRUMBLEANIMATOR_h

#include <stdint.h>


//Interface for a rumble source animator.
class IRumbleAnimator
{
public:
	virtual uint8_t GetStep(const uint32_t sampleTimeMillis)
	{
		return 0;
	}

	virtual bool Play(const uint8_t level, const uint32_t duration, const uint8_t pitch, const uint8_t pwm)
	{
		return false;
	}
};

//Interface for rumble source collection.
class IRumbleDriver
{
public:
	virtual bool AddRumbleAnimator(IRumbleAnimator* rumbleAnimator)
	{
		return false;
	}
};

//Template polyphonic player for multiple concurrent animators, auto balanced (ish).
template<typename T,
	const uint8_t VoiceCount>
	class TemplatePolyphonicAnimator : public IRumbleAnimator
{
private:
	uint16_t Output = 0;

protected:
	T Voices[VoiceCount];

public:
	virtual uint8_t GetStep(const uint32_t sampleTimeMillis)
	{
		// Mix all voices to output.
		Output = 0;
		for (uint8_t i = 0; i < VoiceCount; i++)
		{
			Output += Voices[i].GetStep(sampleTimeMillis);
		}

		if (Output > UINT8_MAX)
		{
			return UINT8_MAX;
		}
		else
		{
			return Output;
		}
	}

	bool Play(const uint8_t level, const uint32_t duration, const uint8_t pitch, const uint8_t pwm)
	{
		for (uint8_t i = 0; i < VoiceCount; i++)
		{
			//Play on free voice, otherwise fail.
			if (!Voices[i].IsPlaying())
			{
				Voices[i].Play(level, duration, pitch, pwm);
				return true;
			}
		}

		return false;
	}
};

class AbstractRumbleAnimator : public IRumbleAnimator
{
protected:
	uint32_t Start = 0;
	uint32_t End = 0;

	uint8_t Level = 0;
	uint8_t Pitch = 0;
	uint8_t PWM = 0;

public:
	virtual uint8_t GetStep(const uint32_t sampleTimeMillis)
	{
		return 0;
	}

public:
	virtual bool Play(const uint8_t level, const uint32_t duration, const uint8_t pitch, const uint8_t pwm)
	{
		Start = millis();
		End = Start + duration;

		Level = level;
		Pitch = pitch;
		PWM = pwm;

		return true;
	}

	void Stop()
	{
		Start = millis();
		End = Start;
	}

	bool IsPlaying()
	{
		return millis() < End;
	}
};


template<const bool Invert = false>
class PulseAnimator : public AbstractRumbleAnimator
{
public:
	virtual uint8_t GetStep(const uint32_t sampleTimeMillis)
	{
		if (!IsPlaying())
		{
			return 0;
		}

		uint8_t pwm = 1;
		if (PWM > pwm)
		{
			pwm = PWM;
		}
		return UINT8_MAX / map(sampleTimeMillis, Start, End, 1, (UINT8_MAX / (pwm)));
	}
};

template<const bool Invert = false>
class SawAnimator : public AbstractRumbleAnimator
{
public:
	virtual uint8_t GetStep(const uint32_t sampleTimeMillis)
	{
		if (!IsPlaying())
		{
			return 0;
		}

		if (Invert)
		{
			return map(sampleTimeMillis, Start, End, 0, Level);
		}
		else
		{
			return map(sampleTimeMillis, Start, End, Level, 0);
		}
	}
};

template<const bool Invert = false>
class SquareAnimator : public AbstractRumbleAnimator
{
public:
	virtual uint8_t GetStep(const uint32_t sampleTimeMillis)
	{
		if (!IsPlaying())
		{
			return 0;
		}

		if (Invert)
		{
			if (map(sampleTimeMillis, Start, End, 0, UINT8_MAX) <= PWM)
			{
				return Level;
			}
		}
		else
		{
			if (map(sampleTimeMillis, Start, End, 0, UINT8_MAX) >= PWM)
			{
				return Level;
			}
		}

		return 0;
	}
};


template<const bool Invert = false>
class TriangleAnimator : public AbstractRumbleAnimator
{
private:
	uint32_t Middle = 0;

public:
	virtual bool Play(const uint8_t level, const uint32_t duration, const uint8_t pitch, const uint8_t pwm)
	{
		AbstractRumbleAnimator::Play(level, duration, pitch, pwm);

		Middle = map(PWM, 0, UINT8_MAX, Start, End);

		return true;
	}

	virtual uint8_t GetStep(const uint32_t sampleTimeMillis)
	{
		if (!IsPlaying())
		{
			return 0;
		}

		if (Invert)
		{
			if (sampleTimeMillis >= Middle)
			{
				return map(sampleTimeMillis, Middle, End, 0, Level);
			}
			else
			{
				return map(sampleTimeMillis, Start, Middle - 1, Level, 0);
			}
		}
		else
		{
			if (sampleTimeMillis >= Middle)
			{
				return map(sampleTimeMillis, Middle, End, Level, 0);
			}
			else
			{
				return map(sampleTimeMillis, Start, Middle - 1, 0, Level);
			}
		}
	}
};

#endif

