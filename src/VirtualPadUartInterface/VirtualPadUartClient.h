#ifndef _VIRTUAL_PAD_UART_CLIENT_h
#define _VIRTUAL_PAD_UART_CLIENT_h

#define _TASK_OO_CALLBACKS
#include <TSchedulerDeclarations.hpp>

#include <UartInterfaceTask.h>
#include <VirtualPad.h>

#if __has_include(<type_traits>)
#include <type_traits>
#endif

namespace VirtualPadUartInterface
{
	/// <summary>
	/// Listens to UART updates for VirtualPad.
	/// Depends on https://github.com/GitMoDu/UartInterface
	/// </summary>
	/// <typeparam name="SerialType"></typeparam>
	/// <typeparam name="UartDefinitions"></typeparam>
	/// <typeparam name="pad_state_t"></typeparam>
	template<typename SerialType,
		typename UartDefinitions = VirtualPadUartInterface::TemplateUartDefinitions<>,
		typename pad_state_t = VirtualPad::analog_pad_state_t>
	class ClientTask : public UartInterface::UartListener, public TS::Task
	{
#if __has_include(<type_traits>)
	private:
		static_assert(std::is_trivially_copyable<VirtualPad::analog_pad_state_t>::value, "Not trivially copyable!");
#endif

	private:
		using MessageEnum = VirtualPadUartInterface::MessageEnum;

	private:
		UartInterface::UartInterfaceTask<SerialType, UartDefinitions> Interface;

	private:
		pad_state_t Pad{};

	private:
		VirtualPad::StateListener<pad_state_t>* Listener;
		Print* SerialLogger;

	private:
		uint32_t LastUpdate = 0;
		bool FeedbackPending = 0;

		static const uint32_t OperatorCheckPeriod = 50;
		static const uint32_t OperatorTimeoutPeriod = 500;

	public:
		ClientTask(TS::Scheduler& scheduler,
			SerialType& serialInstance,
			VirtualPad::StateListener<pad_state_t>* listener = nullptr,
			Print* serialLogger = nullptr)
			: UartInterface::UartListener()
			, TS::Task(TASK_IMMEDIATE, TASK_FOREVER, &scheduler, false)
			, Interface(scheduler, serialInstance, this,
				VirtualPadUartInterface::UartKey, sizeof(VirtualPadUartInterface::UartKey))
			, Listener(listener)
			, SerialLogger(serialLogger)
		{
		}

		bool Setup()
		{
			return Interface.Setup();
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
			if (Pad.Connected()
				&& ((millis() - LastUpdate) > OperatorTimeoutPeriod))
			{
				Pad = pad_state_t{};
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
			// Reset pad state on any connection state change.
			Pad = pad_state_t{};
			Pad.SetConnected(false);
			if (Listener != nullptr)
			{
				Listener->OnUpdate(Pad);
			}
		}

		void OnUartTxError(const UartInterface::TxErrorEnum error) final
		{
			if (SerialLogger != nullptr)
			{
				switch (error)
				{
				case UartInterface::TxErrorEnum::StartTimeout:
					SerialLogger->println(F("Tx Start Timeout"));
					break;
				case UartInterface::TxErrorEnum::DataTimeout:
					SerialLogger->println(F("Tx Data Timeout"));
					break;
				case UartInterface::TxErrorEnum::EndTimeout:
					SerialLogger->println(F("Tx End Timeout"));
					break;
				default:
					break;
				}
			}
		}

		void OnUartRxError(const UartInterface::RxErrorEnum error) final
		{
			if (SerialLogger != nullptr)
			{
				switch (error)
				{
				case UartInterface::RxErrorEnum::StartTimeout:
					SerialLogger->println(F("Rx Start Timeout"));
					break;
				case UartInterface::RxErrorEnum::Crc:
					SerialLogger->println(F("Rx Crc Mismatch"));
					break;
				case UartInterface::RxErrorEnum::TooShort:
					SerialLogger->println(F("Rx Too Short"));
					break;
				case UartInterface::RxErrorEnum::TooLong:
					SerialLogger->println(F("Rx TooLong"));
					break;
				case UartInterface::RxErrorEnum::EndTimeout:
					SerialLogger->println(F("Rx End Timeout"));
					break;
				default:
					break;
				}
			}
		}

		void OnUartTx() final
		{
		}

		void OnUartRx(const uint8_t header) final
		{
		}

		void OnUartRx(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize) final
		{
			switch ((MessageEnum)header)
			{
			case MessageEnum::UpdateState:
				if (payloadSize == sizeof(VirtualPad::info_pad_state_t))
				{
					const size_t usableSize = (sizeof(pad_state_t) >= sizeof(VirtualPad::info_pad_state_t)) ?
						sizeof(VirtualPad::info_pad_state_t) : sizeof(pad_state_t);
					memcpy((uint8_t*)&Pad, payload, usableSize);
				}
				else if (payloadSize == sizeof(VirtualPad::button_pad_state_t))
				{
					const size_t usableSize = (sizeof(pad_state_t) >= sizeof(VirtualPad::button_pad_state_t)) ?
						sizeof(VirtualPad::button_pad_state_t) : sizeof(pad_state_t);
					memcpy((uint8_t*)&Pad, payload, usableSize);
				}
				else if (payloadSize == sizeof(VirtualPad::analog_pad_state_t))
				{
					const size_t usableSize = (sizeof(pad_state_t) >= sizeof(VirtualPad::analog_pad_state_t)) ?
						sizeof(VirtualPad::analog_pad_state_t) : sizeof(pad_state_t);
					memcpy((uint8_t*)&Pad, payload, usableSize);
				}
				else if (payloadSize == sizeof(VirtualPad::motion_pad_state_t))
				{
					const size_t usableSize = (sizeof(pad_state_t) >= sizeof(VirtualPad::motion_pad_state_t)) ?
						sizeof(VirtualPad::motion_pad_state_t) : sizeof(pad_state_t);
					memcpy((uint8_t*)&Pad, payload, usableSize);
				}
				else
				{
					// Unrecognized message.
					if (SerialLogger != nullptr)
						SerialLogger->print(F("VirtualPad Unrecognized message"));
					return;
				}

				// Unused fields should be cleared.
				if (payloadSize < sizeof(pad_state_t))
				{
					uint8_t* firstByteAddress = &((uint8_t*)(&Pad))[payloadSize];
					memset(firstByteAddress, 0, sizeof(pad_state_t) - payloadSize);
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
}
#endif