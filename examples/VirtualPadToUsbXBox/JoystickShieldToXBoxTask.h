#ifndef _JOYSTICK_SHIELD_TO_XBOX_TASK_h
#define _JOYSTICK_SHIELD_TO_XBOX_TASK_h

#include <VirtualPads.h>
#include "VirtualPadToXBoxTask.h"

template<const uint8_t pinJoyX = UINT8_MAX, const uint8_t pinJoyY = UINT8_MAX,
	const uint8_t pinA = UINT8_MAX, const uint8_t pinB = UINT8_MAX,
	const uint8_t pinX = UINT8_MAX, const uint8_t pinY = UINT8_MAX,
	const uint8_t pinL3 = UINT8_MAX,
	const uint8_t pinStart = UINT8_MAX,
	const uint8_t pinSelect = UINT8_MAX>
class JoystickShieldToXBoxTask : public VirtualPadToXBoxTask
{
private:
	// VirtualPad object.
	JoystickShield::JoystickShieldVirtualPad<
		pinJoyX, pinJoyY, pinA, pinB, pinX, pinY, pinL3, pinStart, pinSelect> Pad{};

public:
	JoystickShieldToXBoxTask(Scheduler* scheduler, USBXBox360& x360,
		const uint32_t updatePeriodMicros)
		: VirtualPadToXBoxTask(scheduler, x360, Pad, updatePeriodMicros)
	{
	}

protected:
	void UpdateInput() final
	{
		Pad.Step();
	}
};
#endif

