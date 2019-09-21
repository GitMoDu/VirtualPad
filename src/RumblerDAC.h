// RumblerDAC.h

#ifndef _RUMBLERDAC_h
#define _RUMBLERDAC_h

#include <IInputController.h>
#include <RingBufCPP.h>
#include <IRumblerDAC.h>

template<const uint8_t BufferSize,
	const uint8_t SamplePeriodMillis,
	const uint32_t TargetPeriodMillis = 30>
class RumblerDAC : public IRumblerDAC
{
private:
	uint8_t Step = 0;

	RingBufCPP<uint8_t, BufferSize> Buffer;
	uint8_t TargetPressure = 0;

	constexpr uint8_t GetQuantizationSteps()
	{
		if (SamplePeriodMillis > TargetPeriodMillis)
		{
			return 1;
		}
		else
		{
			return TargetPeriodMillis / SamplePeriodMillis;
		}
	}

public:
	RumblerDAC() : IRumblerDAC(), Buffer()
	{
	}

	bool Append(const uint8_t value)
	{
		Buffer.addForce(value);

		return Buffer.isFull();
	}

	inline uint8_t GetBufferCount()
	{
		return Buffer.numElements();
	}

	inline bool IsBufferFull()
	{
		return Buffer.isFull();
	}

	virtual bool Output()
	{
		if (Buffer.isEmpty())
		{
			return false;
		}
		else
		{
			if (Step >= GetQuantizationSteps())
			{
				Buffer.pull(&TargetPressure);
				Step = 0;
			}
			else
			{
				Step++;
			}

			if (Step <= map(TargetPressure, 0, UINT8_MAX, 0, GetQuantizationSteps()))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
};


#endif

