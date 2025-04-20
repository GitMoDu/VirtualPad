#ifndef _VIRTUAL_PAD_INTERFACE_h
#define _VIRTUAL_PAD_INTERFACE_h

#include "VirtualPad.h"

namespace VirtualPad
{
	template<typename state_t>
	struct StateListener
	{
		virtual void OnUpdate(state_t& state) {}
	};

	using ButtonPadStateListener = StateListener<button_pad_state_t>;
	using AnalogPadStateListener = StateListener<analog_pad_state_t>;
	using MotionPadStateListener = StateListener<motion_pad_state_t>;
}
#endif