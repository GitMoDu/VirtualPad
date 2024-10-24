// VirtualPadUartInterface.h

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

	struct UartDefinitions
	{
		static const uint32_t Baudrate = 921600;
		static constexpr uint32_t MessageSizeMax = 32;
		static constexpr uint8_t MaxSerialStepOut = 32;
		static constexpr uint8_t MaxSerialStepIn = 32;
	};

	/// <summary>
	/// "VirtualPad" in ASCII.
	/// </summary>
	static constexpr uint8_t UartKey[] = { 86, 105, 114, 116, 117, 97, 108, 80, 97, 100 };
};
#endif