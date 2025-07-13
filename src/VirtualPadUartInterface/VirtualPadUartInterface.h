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

	template<uint32_t baudrate = 115200,
		uint8_t maxSerialStep = 32,
		uint32_t timeoutMillis = 20,
		uint32_t pollPeriodMillis = 1>
	using TemplateUartDefinitions = UartInterface::TemplateUartDefinitions<
		baudrate,
		128,
		maxSerialStep,
		maxSerialStep,
		timeoutMillis,
		timeoutMillis,
		pollPeriodMillis>;

	/// <summary>
	/// </summary>
	static constexpr uint8_t UartKey[] = { 'V', 'i', 'r', 't', 'u', 'a', 'l', 'P', 'a', 'd' };
};
#endif