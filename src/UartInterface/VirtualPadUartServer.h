// VirtualPadUartServer.h

#ifndef _VIRTUAL_PAD_UART_SERVER_h
#define _VIRTUAL_PAD_UART_SERVER_h

#if defined(_TASK_OO_CALLBACKS)
#include <TSchedulerDeclarations.hpp>

#include <UartInterfaceTask.h>

/// <summary>
/// Continuosly pushes out the VirtualPad state over UART.
/// Depends on https://github.com/GitMoDu/UartInterface
/// </summary>
/// <typeparam name="SerialType"></typeparam>
template<typename SerialType,
	typename UartDefinitions = UartInterface::ExampleUartDefinitions,
	uint32_t UpdatePeriod = 10,
	uint32_t UpdateLongPeriod = 50>
class VirtualPadUartServer : private TS::Task, public virtual UartInterfaceListener
{
private:
	using MessageEnum = VirtualPadUartInterface::MessageEnum;

private:
	UartInterfaceTask<SerialType, UartDefinitions> Interface;

private:
	VirtualPad& Pad;

private:
	virtual_pad_state_t RawState{};

	uint32_t LastCommandReceived = 0;

public:
	VirtualPadUartServer(TS::Scheduler& scheduler,
		SerialType& serialInstance,
		VirtualPad& virtualPad)
		: UartInterfaceListener()
		, TS::Task(TASK_IMMEDIATE, TASK_FOREVER, &scheduler, false)
		, Interface(scheduler, serialInstance, this,
			VirtualPadUartInterface::UartKey, sizeof(VirtualPadUartInterface::UartKey))
		, Pad(virtualPad)
	{}

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

	virtual bool Callback() final
	{
		if (Interface.IsSerialConnected())
		{
			Pad.CopyStateTo(RawState);

			if (RawState.Connected)
			{
				TS::Task::delay(UpdatePeriod);
			}
			else
			{
				TS::Task::delay(UpdateLongPeriod);
			}

			Interface.SendMessage((uint8_t)MessageEnum::UpdateState, (uint8_t*)&RawState, sizeof(virtual_pad_state_t));
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

	void OnMessageReceived(const uint8_t header) final
	{
	}

	void OnMessageReceived(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize)
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
#endif