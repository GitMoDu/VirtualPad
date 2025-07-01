#ifndef _VIRTUAL_PAD_UART_SERVER_h
#define _VIRTUAL_PAD_UART_SERVER_h

#define _TASK_OO_CALLBACKS
#include <TSchedulerDeclarations.hpp>

#include <UartInterfaceTask.h>
#include <VirtualPad.h>

/// <summary>
/// Continuously pushes out the VirtualPad state over UART.
/// Depends on https://github.com/GitMoDu/UartInterface
/// </summary>
/// <typeparam name="SerialType"></typeparam>
/// <typeparam name="UartDefinitions"></typeparam>
/// <typeparam name="pad_state_t"></typeparam>
/// <typeparam name="UpdateLongPeriod"></typeparam>
/// <typeparam name="UpdatePeriod"></typeparam>
template<typename SerialType,
	typename UartDefinitions = UartInterface::TemplateUartDefinitions<>,
	typename pad_state_t = VirtualPad::analog_pad_state_t,
	uint32_t UpdatePeriod = 10,
	uint32_t UpdateLongPeriod = 50>
class VirtualPadUartServerTask : public virtual UartInterfaceListener, private TS::Task
{
private:
	using MessageEnum = VirtualPadUartInterface::MessageEnum;

private:
	UartInterfaceTask<SerialType, UartDefinitions> Interface;

private:
	pad_state_t Pad;

private:
	pad_state_t RawState{};

	uint32_t LastCommandReceived = 0;

public:
	VirtualPadUartServerTask(TS::Scheduler& scheduler,
		SerialType& serialInstance,
		pad_state_t& virtualPad)
		: UartInterfaceListener()
		, TS::Task(TASK_IMMEDIATE, TASK_FOREVER, &scheduler, false)
		, Interface(scheduler, serialInstance, this,
			VirtualPadUartInterface::UartKey, sizeof(VirtualPadUartInterface::UartKey))
		, Pad(virtualPad)
	{
	}

	/// <summary>
	/// Passthrough event to wake up uart reader, if needed.
	/// </summary>
	void OnSerialEvent()
	{
		Interface.OnSerialEvent();
	}

	void Start()
	{
		Interface.Start();
		TS::Task::enableDelayed(0);
	}

	void Stop()
	{
		TS::Task::disable();
		Interface.Stop();
	}

	bool Callback() final
	{
		if (Interface.IsSerialConnected())
		{
			memcpy(&RawState, &Pad, sizeof(pad_state_t));

			if (RawState.Connected())
			{
				TS::Task::delay(UpdatePeriod);
			}
			else
			{
				TS::Task::delay(UpdateLongPeriod);
			}

			Interface.SendMessage((uint8_t)MessageEnum::UpdateState, (uint8_t*)&RawState, sizeof(pad_state_t));
		}
		else
		{
			TS::Task::delay(UpdateLongPeriod);
		}

		return true;
	}

	void OnUartStateChange(const bool connected) final
	{
		if (connected)
		{
			TS::Task::enableDelayed(0);
		}
	}

	void OnUartRx(const uint8_t header) final
	{
	}

	void OnUartRx(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize)
	{
		switch ((MessageEnum)header)
		{
		case MessageEnum::UpdateFeedback:
			LastCommandReceived = millis();

			//TODO: notify feedback listener.
			//if (payloadSize == ?)
			//{
				// TODO:
			//}
			break;
		default:
			break;
		}
	}
};
#endif