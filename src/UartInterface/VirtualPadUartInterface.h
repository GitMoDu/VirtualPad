#ifndef _VIRTUAL_PAD_UART_INTERFACE_h
#define _VIRTUAL_PAD_UART_INTERFACE_h

#include <UartInterface.h>

namespace VirtualPadUartInterface
{
	enum class MessageEnum : uint8_t
	{
		UpdateState = 0,
		UpdateFeedback = 1
	};

	/// <summary>
	/// "VirtualPad" in ASCII.
	/// </summary>
	static constexpr uint8_t UartKey[] = { 86, 105, 114, 116, 117, 97, 108, 80, 97, 100 };
};
#endif