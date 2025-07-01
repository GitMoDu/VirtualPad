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
	/// </summary>
	static constexpr uint8_t UartKey[] = { 'V', 'i', 'r', 't', 'u', 'a', 'l', 'P', 'a', 'd' };
};
#endif