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


	struct ExampleUartDefinitions
	{
		static const uint32_t Baudrate = 115200;
		static constexpr uint32_t MessageSizeMax = 32;
		static constexpr uint8_t MaxSerialStepOut = 16;
		static constexpr uint8_t MaxSerialStepIn = 16;

		static constexpr uint32_t ReadPollPeriodMillis = 50;
		static constexpr uint32_t WriteTimeoutMillis = 10;
	};

	/// <summary>
	/// "VirtualPad" in ASCII.
	/// </summary>
	static constexpr uint8_t UartKey[] = { 86, 105, 114, 116, 117, 97, 108, 80, 97, 100 };
};
#endif