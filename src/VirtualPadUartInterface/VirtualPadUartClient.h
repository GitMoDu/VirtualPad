#ifndef _VIRTUAL_PAD_UART_CLIENT_h
#define _VIRTUAL_PAD_UART_CLIENT_h

#define _TASK_OO_CALLBACKS
#include <TSchedulerDeclarations.hpp>

#include <UartInterfaceTask.h>
#include <VirtualPad.h>

/// <summary>
/// Listens to UART updates for VirtualPad.
/// Depends on https://github.com/GitMoDu/UartInterface
/// </summary>
/// <typeparam name="SerialType"></typeparam>
/// <typeparam name="UartDefinitions"></typeparam>
/// <typeparam name="pad_state_t"></typeparam>
template<typename SerialType,
	typename UartDefinitions = UartInterface::TemplateUartDefinitions<>,
	typename pad_state_t = VirtualPad::analog_pad_state_t>
class VirtualPadUartClientTask : public virtual UartInterfaceListener, private TS::Task
{
private:
	using MessageEnum = VirtualPadUartInterface::MessageEnum;

private:
	UartInterfaceTask<SerialType, UartDefinitions> Interface;

private:
	pad_state_t& Pad;

private:
	VirtualPad::StateListener<pad_state_t>* Listener;
	Print* SerialLogger;

private:
	uint32_t LastUpdate = 0;
	bool FeedbackPending = 0;

	static const uint32_t OperatorCheckPeriod = 50;
	static const uint32_t OperatorTimeoutPeriod = 500;

public:
	VirtualPadUartClientTask(TS::Scheduler& scheduler,
		SerialType& serialInstance,
		pad_state_t& pad,
		VirtualPad::StateListener<pad_state_t>* listener = nullptr,
		Print* serialLogger = nullptr)
		: UartInterfaceListener()
		, TS::Task(TASK_IMMEDIATE, TASK_FOREVER, &scheduler, false)
		, Interface(scheduler, serialInstance, this,
			VirtualPadUartInterface::UartKey, sizeof(VirtualPadUartInterface::UartKey))
		, Pad(pad)
		, Listener(listener)
		, SerialLogger(serialLogger)
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

	virtual bool Callback() final
	{
		if (Pad.Connected()
			&& ((millis() - LastUpdate) > OperatorTimeoutPeriod))
		{
			memset(&Pad, 0, sizeof(pad_state_t));
			if (Listener != nullptr)
			{
				Listener->OnUpdate(Pad);
			}
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
			memset(&Pad, 0, sizeof(pad_state_t));
			Pad.SetConnected(true);
			if (Listener != nullptr)
			{
				Listener->OnUpdate(Pad);
			}
		}
	}

	virtual void OnUartTxError(const TxErrorEnum error) final
	{
		if (SerialLogger != nullptr)
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
		if (SerialLogger != nullptr)
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
	{
	}

	void OnUartRx(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize) final
	{
		switch ((MessageEnum)header)
		{
		case MessageEnum::UpdateState:
			if (payloadSize == sizeof(VirtualPad::button_pad_state_t))
			{
				memcpy((uint8_t*)&Pad, payload, sizeof(VirtualPad::button_pad_state_t));
			}
			else if (payloadSize == sizeof(VirtualPad::analog_pad_state_t))
			{
				memcpy((uint8_t*)&Pad, payload, sizeof(VirtualPad::analog_pad_state_t));
			}
			else if (payloadSize == sizeof(VirtualPad::motion_pad_state_t))
			{
				memcpy((uint8_t*)&Pad, payload, sizeof(VirtualPad::motion_pad_state_t));
			}
			else
			{
				// Unrecognized message.
				return;
			}

			// Unused fields should be cleared.
			if (payloadSize >= sizeof(pad_state_t))
			{
				memset(&Pad, 0, payloadSize - sizeof(pad_state_t));
			}

			LastUpdate = millis();
			if (Listener != nullptr)
			{
				Listener->OnUpdate(Pad);
			}
			break;
		default:
			break;
		}
	}
};
#endif