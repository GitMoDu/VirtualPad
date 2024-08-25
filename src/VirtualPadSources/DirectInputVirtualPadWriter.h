// DirectInputVirtualPadWriter.h

#ifndef _DIRECT_INPUT_VIRTUAL_PAD_WRITER_h
#define _DIRECT_INPUT_VIRTUAL_PAD_WRITER_h

#include "../WriteVirtualPad.h"
#include <Arduino.h>

class DirectInputVirtualPadWriter : public WriteVirtualPad
{
public:
	static constexpr uint16_t Features = FeatureFlags::GetFlags();

private:
	const uint8_t PinJoyX;
	const uint8_t PinJoyY;
	const uint8_t PinA;
	const uint8_t PinB;

public:
	DirectInputVirtualPadWriter(const uint8_t pinJoyX, const uint8_t pinJoyY, const uint8_t pinA, const uint8_t pinB)
		: WriteVirtualPad(Features)
		, PinJoyX(pinJoyX)
		, PinJoyY(pinJoyY)
		, PinA(pinA)
		, PinB(pinB)
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

		return true;
	}

	void Step()
	{
		uint16_t joyX = 0;
		uint16_t joyY = 0;
		if (PinJoyX != UINT8_MAX)
		{
			joyX = (analogRead(PinJoyX) * UINT16_MAX) / (1024 - 1);
		}
		else
		{
			joyX = INT16_MAX;
		}

		if (PinJoyY != UINT8_MAX)
		{
			joyY = (analogRead(PinJoyY) * UINT16_MAX) / (1024 - 1);
		}
		else
		{
			joyY = INT16_MAX;
		}

		SetJoy1(joyX, joyY);

		SetA(digitalRead(PinA));
		SetB(digitalRead(PinB));

		SetX(false);
		SetY(false);
		SetStart(false);

		if (!State.Connected)
		{
			State.Connected = true;
		}
	}
};
#endif