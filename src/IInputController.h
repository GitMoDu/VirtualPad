// IInputController.h

#ifndef _I_INPUT_CONTROLLER_h
#define _I_INPUT_CONTROLLER_h

#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>




#include <IRumblerDAC.h>


class IInputController
{
public:
	virtual uint16_t GetJoy1X() { return 0; }
	virtual uint16_t GetJoy1Y() { return 0; }
	virtual uint16_t GetJoy2X() { return 0; }
	virtual uint16_t GetJoy2Y() { return 0; }
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


};

class IDispatcher {
public:
	virtual void OnDataUpdated();
	virtual void OnControllerStateChanged(const bool connected);
};

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
		}

		return false;
	}

	uint32_t GetDurationMillis()
	{
		return Time;
	}
};

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
		}

		return false;
	}

	uint32_t GetDurationMillis()
	{
		return Time;
	}
};

#endif

