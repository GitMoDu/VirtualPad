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

#endif

