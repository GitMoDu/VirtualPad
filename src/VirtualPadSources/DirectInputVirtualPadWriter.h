// DirectInputVirtualPadWriter.h

#ifndef _DIRECT_INPUT_VIRTUAL_PAD_WRITER_h
#define _DIRECT_INPUT_VIRTUAL_PAD_WRITER_h

#include "../WriteVirtualPad.h"
#include <Arduino.h>

class DirectInputVirtualPadWriter : public WriteVirtualPad
{
public:
	static constexpr uint16_t AdcReference = (1024 - 1);
	static constexpr uint16_t AdcMiddle = ((1024 / 2) + 1);


private:
	static constexpr bool IsPinActive(const uint8_t pin)
	{
		return pin != UINT8_MAX;
	}

	static constexpr uint32_t GetFeatures(const uint8_t pinJoyX, const uint8_t pinJoyY,
		const uint8_t pinA, const uint8_t pinB,
		const uint8_t pinX, const uint8_t pinY,
		const uint8_t pinL3,
		const uint8_t pinStart,
		const uint8_t pinSelect)
	{
		return (FeatureFlags::GetFlags<FeaturesEnum::Joy1>() * (IsPinActive(pinJoyX) && IsPinActive(pinJoyY)))
			| (FeatureFlags::GetFlags<FeaturesEnum::A>() * IsPinActive(pinA))
			| (FeatureFlags::GetFlags<FeaturesEnum::B>() * IsPinActive(pinB))
			| (FeatureFlags::GetFlags<FeaturesEnum::X>() * IsPinActive(pinX))
			| (FeatureFlags::GetFlags<FeaturesEnum::Y>() * IsPinActive(pinY))
			| (FeatureFlags::GetFlags<FeaturesEnum::L3>() * IsPinActive(pinL3))
			| (FeatureFlags::GetFlags<FeaturesEnum::Start>() * IsPinActive(pinStart))
			| (FeatureFlags::GetFlags<FeaturesEnum::Select>() * IsPinActive(pinSelect))
			;
	}

private:
	const uint8_t PinJoyX;
	const uint8_t PinJoyY;
	const uint8_t PinA;
	const uint8_t PinB;
	const uint8_t PinX;
	const uint8_t PinY;
	const uint8_t PinL3;
	const uint8_t PinStart;
	const uint8_t PinSelect;

public:
	DirectInputVirtualPadWriter(const uint8_t pinJoyX = UINT8_MAX, const uint8_t pinJoyY = UINT8_MAX,
		const uint8_t pinA = UINT8_MAX, const uint8_t pinB = UINT8_MAX,
		const uint8_t pinX = UINT8_MAX, const uint8_t pinY = UINT8_MAX,
		const uint8_t pinL3 = UINT8_MAX,
		const uint8_t pinStart = UINT8_MAX,
		const uint8_t pinSelect = UINT8_MAX
	)
		: WriteVirtualPad(GetFeatures(pinJoyX, pinJoyY, pinA, pinB, pinX, pinY, pinL3, pinStart, pinSelect))
		, PinJoyX(pinJoyX)
		, PinJoyY(pinJoyY)
		, PinA(pinA)
		, PinB(pinB)
		, PinX(pinX)
		, PinY(pinY)
		, PinL3(pinL3)
		, PinStart(pinStart)
		, PinSelect(pinSelect)

	{
	}

	const bool Setup()
	{
		Clear();

		if (PinJoyX != UINT8_MAX)
		{
			pinMode(PinJoyX, INPUT);
		}

		if (PinJoyY != UINT8_MAX)
		{
			pinMode(PinJoyY, INPUT);
		}

		if (PinA != UINT8_MAX)
		{
			pinMode(PinA, INPUT);
		}

		if (PinB != UINT8_MAX)
		{
			pinMode(PinB, INPUT);
		}

		if (PinX != UINT8_MAX)
		{
			pinMode(PinX, INPUT);
		}

		if (PinY != UINT8_MAX)
		{
			pinMode(PinY, INPUT);
		}

		return true;
	}

	void Step()
	{
		uint16_t x = AdcMiddle;
		uint16_t y = AdcMiddle;

		int16_t joyX = 0;
		int16_t joyY = 0;

		if (PinJoyX != UINT8_MAX)
		{
			x = analogRead(PinJoyX);
		}

		if (PinJoyY != UINT8_MAX)
		{
			y = analogRead(PinJoyY);
		}

		if (x >= AdcMiddle)
		{
			joyX = ((uint32_t)(x - AdcMiddle) * INT16_MAX) / (AdcReference - AdcMiddle);
		}
		else
		{
			joyX = ((int32_t)(AdcMiddle - x) * INT16_MIN) / (AdcMiddle);
		}

		if (y >= AdcMiddle)
		{
			joyY = ((uint32_t)(y - AdcMiddle) * INT16_MAX) / (AdcReference - AdcMiddle);
		}
		else
		{
			joyY = ((int32_t)(AdcMiddle - y) * INT16_MIN) / (AdcMiddle);
		}
		SetJoy1(joyX, joyY);

		if (PinA != UINT8_MAX)
		{
			SetA(!digitalRead(PinA));
		}
		else
		{
			SetA(false);
		}

		if (PinB != UINT8_MAX)
		{
			SetB(!digitalRead(PinB));
		}
		else
		{
			SetB(false);
		}

		if (PinX != UINT8_MAX)
		{
			SetX(!digitalRead(PinX));
		}
		else
		{
			SetX(false);
		}

		if (PinY != UINT8_MAX)
		{
			SetY(!digitalRead(PinY));
		}
		else
		{
			SetY(false);
		}

		if (PinL3 != UINT8_MAX)
		{
			SetL3(!digitalRead(PinL3));
		}
		else
		{
			SetL3(false);
		}

		if (PinStart != UINT8_MAX)
		{
			SetStart(!digitalRead(PinStart));
		}
		else
		{
			SetStart(false);
		}

		if (PinSelect != UINT8_MAX)
		{
			SetSelect(!digitalRead(PinSelect));
		}
		else
		{
			SetSelect(false);
		}

		if (!State.Connected)
		{
			State.Connected = true;
		}
	}
};
#endif