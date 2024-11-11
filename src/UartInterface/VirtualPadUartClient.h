// VirtualPadOverUart.h

#ifndef _VIRTUAL_PAD_UART_CLIENT_h
#define _VIRTUAL_PAD_UART_CLIENT_h

#if defined(_TASK_OO_CALLBACKS)
#include <TSchedulerDeclarations.hpp>

#include <UartInterfaceTask.h>
#include <VirtualPad.h>

/// <summary>
/// Listens to UART updates for VirtualPad.
/// Depends on https://github.com/GitMoDu/UartInterface
/// </summary>
/// <typeparam name="SerialType"></typeparam>
template<typename SerialType,
	typename UartDefinitions = UartInterface::TemplateUartDefinitions<>>
	class VirtualPadClientTask : private TS::Task, public virtual UartInterfaceListener
{
private:
	using MessageEnum = VirtualPadUartInterface::MessageEnum;

private:
	UartInterfaceTask<SerialType, UartDefinitions> Interface;

private:
	VirtualPad& Pad;

	Print* SerialLogger;

private:
	virtual_pad_state_t RawState{};

private:
	uint32_t LastUpdate = 0;
	bool FeedbackPending = 0;

	static const uint32_t OperatorCheckPeriod = 50;
	static const uint32_t OperatorTimeoutPeriod = 500;

public:
	VirtualPadClientTask(TS::Scheduler& scheduler,
		SerialType& serialInstance,
		VirtualPad& virtualPad,
		Print* serialLogger = nullptr)
		: UartInterfaceListener()
		, TS::Task(TASK_IMMEDIATE, TASK_FOREVER, &scheduler, false)
		, Interface(scheduler, serialInstance, this,
			VirtualPadUartInterface::UartKey, sizeof(VirtualPadUartInterface::UartKey))
		, Pad(virtualPad)
		, SerialLogger(serialLogger)
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
		if (Pad.Connected()
			&& ((millis() - LastUpdate) > OperatorTimeoutPeriod))
		{
			Pad.Clear();
			TS::Task::delay(OperatorCheckPeriod);
		}
		else if (FeedbackPending)
		{
			FeedbackPending = false;

			//TODO: Handle feedback.
			//Interface.SendMessage((uint8_t)MessageEnum::UpdateFeedback, (uint8_t*)?, ?);

			TS::Task::delay(0);
		}
		else
		{
			TS::Task::delay(OperatorCheckPeriod);
		}

		return true;
	}

	void OnUartStateChange(const bool connected) final
	{
		if (!connected
			&& Pad.Connected())
		{
			Pad.Clear();
		}
	}

	virtual void OnUartTxError(const TxErrorEnum error) final
	{
		if (SerialLogger)
		{
			switch (error)
			{
			case UartInterfaceListener::TxErrorEnum::StartTimeout:
				SerialLogger->println(F("Tx Start Timeout"));
				break;
			case UartInterfaceListener::TxErrorEnum::DataTimeout:
				SerialLogger->println(F("Tx Data Timeout"));
				break;
			case UartInterfaceListener::TxErrorEnum::EndTimeout:
				SerialLogger->println(F("Tx End Timeout"));
				break;
			default:
				break;
			}
		}
	}

	virtual void OnUartRxError(const RxErrorEnum error) final
	{
		if (SerialLogger)
		{
			switch (error)
			{
			case RxErrorEnum::StartTimeout:
				SerialLogger->println(F("Rx Start Timeout"));
				break;
			case RxErrorEnum::Crc:
				SerialLogger->println(F("Rx Crc Mismatch"));
				break;
			case RxErrorEnum::TooShort:
				SerialLogger->println(F("Rx Too Short"));
				break;
			case RxErrorEnum::TooLong:
				SerialLogger->println(F("Rx TooLong"));
				break;
			case RxErrorEnum::EndTimeout:
				SerialLogger->println(F("Rx End Timeout"));
				break;
			default:
				break;
			}
		}
	}

	void OnUartRx(const uint8_t header) final
	{}

	void OnUartRx(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize) final
	{
		switch ((MessageEnum)header)
		{
		case MessageEnum::UpdateState:
			if (payloadSize == sizeof(virtual_pad_state_t))
			{
				LastUpdate = millis();

				memcpy((uint8_t*)&RawState, payload, sizeof(virtual_pad_state_t));
				Pad.CopyStateFrom(RawState);
			}
			break;
		default:
			break;
		}
	}
};
#endif
#endif