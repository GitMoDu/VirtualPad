// IInputController.h

#ifndef _I_INPUT_CONTROLLER_h
#define _I_INPUT_CONTROLLER_h

#include <stdint.h>

// Virtual controller interface.
class IInputController
{
public:
	virtual uint16_t GetJoy1X() { return UINT16_MAX / 2; }
	virtual uint16_t GetJoy1Y() { return UINT16_MAX / 2; }
	virtual uint16_t GetJoy2X() { return UINT16_MAX / 2; }
	virtual uint16_t GetJoy2Y() { return UINT16_MAX / 2; }
	virtual uint16_t GetTriggerL() { return 0; }
	virtual uint16_t GetTriggerR() { return 0; }

	virtual void GetDirection(bool& left, bool& right, bool& up, bool& down)
	{
		left = false;
		right = false;
		up = false;
		down = false;
	}
	virtual bool GetLeft() { return false; }
	virtual bool GetRight() { return false; }
	virtual bool GetUp() { return false; }
	virtual bool GetDown() { return false; }

	virtual bool GetButton0() { return false; }
	virtual bool GetButton1() { return false; }
	virtual bool GetButton2() { return false; }
	virtual bool GetButton3() { return false; }
	virtual bool GetButton4() { return false; }
	virtual bool GetButton5() { return false; }
	virtual bool GetButton6() { return false; }
	virtual bool GetButton7() { return false; }
	virtual bool GetButton8() { return false; }
	virtual bool GetButton9() { return false; }

public:
	// General interface controls, should return one of the existing buttons.
	virtual bool GetButtonAccept() { return GetButton0(); }
	virtual bool GetButtonReject() { return GetButton1(); }
	virtual bool GetButtonHome() { return false; }
};

// Virtual dispatcher, for use in composition.
class IDispatcher {
public:
	virtual void OnDataUpdated() {}
	virtual void OnStateChanged(const bool connected) {}
};


// Axis parser.
template<typename T,
	typename OutT,
	const T Min,
	const T Max,
	const T DeadZoneRange,
	const OutT OutBottom,
	const OutT OutTop>
	class AxisLinear
{
public:
	OutT Parse(const T value)
	{
		// Clip input to min/max.
		T ValueClipped = value;

		if (value >= Max)
		{
			ValueClipped = Max;
		}
		else if (value < Min)
		{
			ValueClipped = Min;
		}

		// Deadzone low region.
		if (ValueClipped < DeadZoneRange)
		{
			return OutBottom;
		}
		else
		{
			return ((ValueClipped - DeadZoneRange) * (OutTop - OutBottom)) / (Max - DeadZoneRange);
		}
	}
};

// Joystick parser.
template<typename T,
	typename OutT,
	const T Min,
	const T Max,
	const int8_t Offset,
	const T DeadZoneRange,
	const OutT OutBottom,
	const OutT OutTop>
	class AxisCentered
{
private:
	const T Mid = ((Max + Min) / 2) - Offset;
	const T MidUpper = Mid + DeadZoneRange;
	const T MidLower = Mid - DeadZoneRange;
	const OutT OutCenter = ((OutTop + OutBottom) / 2);

public:
	T GetCenter()
	{
		return Mid;
	}

	OutT Parse(const T value)
	{
		// Clip input to min/max.
		T ValueClipped = value;

		if (value >= Max)
		{
			ValueClipped = Max;
		}
		else if (value < Min)
		{
			ValueClipped = Min;
		}

		// Deadzone center region. Only works well for small values (< 5).
		if (ValueClipped >= Mid)
		{
			if (ValueClipped < MidUpper)
			{
				return OutCenter;
			}
			else
			{
				return OutCenter + (((ValueClipped - MidUpper) * (OutTop - OutCenter)) / (Max - MidUpper));
			}
		}
		else
		{
			if (ValueClipped > MidLower)
			{
				return OutCenter;
			}
			else
			{
				return OutCenter - (((MidLower - ValueClipped) * (OutCenter - OutBottom)) / (MidLower - Min));
			}
		}
	}
};

// Button parser.
class ButtonPress
{
private:
	bool Up = false;

public:
	bool Parse(const bool button)
	{
		if (Up && !button)
		{
			Up = false;
			return true;
		}
		else if (!Up && button)
		{
			Up = true;
		}

		return false;
	}
};

// Timed button parser.
class TimedButtonPress
{
private:
	bool Up = false;
	uint32_t Time = 0;

public:
	bool Parse(const bool button)
	{
		if (Up && !button)
		{
			Up = false;
			Time = millis() - Time;

			return true;
		}
		else if (!Up && button)
		{
			Time = millis();
			Up = true;
			return false;
		}
	}

	uint32_t GetDurationMillis()
	{
		return Time;
	}
};

// Hold button parser.
template<const uint32_t MinDurationMillis>
class MinTimedButtonPress
{
private:
	bool Up = false;
	uint32_t Time = 0;

public:
	bool Parse(const bool button)
	{
		if (Up && !button)
		{
			Up = false;
			Time = millis() - Time;

			return Time > MinDurationMillis;
		}
		else if (!Up && button)
		{
			Time = millis();
			Up = true;
			return false;
		}
	}

	uint32_t GetDurationMillis()
	{
		return Time;
	}
};
#endif