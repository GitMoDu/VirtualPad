#ifndef _VIRTUAL_PAD_FEEDBACK_h
#define _VIRTUAL_PAD_FEEDBACK_h

#include "../VirtualPad/VirtualPad.h"

namespace VirtualPad
{
	struct IFeedback
	{
		virtual void Stop();
	};
}
#endif