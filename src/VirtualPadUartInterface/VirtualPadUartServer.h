#ifndef _VIRTUAL_PAD_UART_SERVER_h
#define _VIRTUAL_PAD_UART_SERVER_h

#define _TASK_OO_CALLBACKS
#include <TSchedulerDeclarations.hpp>

#include <UartInterfaceTask.h>
#include <VirtualPad.h>

namespace VirtualPadUartInterface
{
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
		typename UartDefinitions = VirtualPadUartInterface::TemplateUartDefinitions<>,
		typename pad_state_t = VirtualPad::analog_pad_state_t,
		uint32_t UpdatePeriod = 5,
		uint32_t UpdateLongPeriod = 50>
	class ServerTask : public virtual UartInterface::UartListener, public TS::Task
	{
	private:
		UartInterface::UartInterfaceTask<SerialType, UartDefinitions> Interface;

	private:
		pad_state_t& Pad;

	private:
		pad_state_t RawState{};

		uint32_t LastCommandReceived = 0;

	public:
		ServerTask(TS::Scheduler& scheduler,
			SerialType& serialInstance,
			pad_state_t& virtualPad)
			: UartInterface::UartListener()
			, TS::Task(TASK_IMMEDIATE, TASK_FOREVER, &scheduler, false)
			, Interface(scheduler, serialInstance, this, VirtualPadUartInterface::UartKey, sizeof(VirtualPadUartInterface::UartKey))
			, Pad(virtualPad)
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
			TS::Task::enable();
			Interface.Stop();
		}

		bool Callback() final
		{
			if (Interface.IsSerialConnected())
			{
				if (Interface.CanSendMessage())
				{
					RawState = Pad;

					if (RawState.Connected())
					{
						TS::Task::delay(UpdatePeriod);
					}
					else
					{
						TS::Task::delay(UpdateLongPeriod);
					}

					Interface.SendMessage((uint8_t)VirtualPadUartInterface::MessageEnum::UpdateState, (uint8_t*)&RawState, sizeof(pad_state_t));
				}
				else
				{
					TS::Task::delay(0);
				}
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


		void OnUartTx() final
		{
		}

		void OnUartRxError(const UartInterface::RxErrorEnum error) final
		{
		}

		void OnUartTxError(const UartInterface::TxErrorEnum error) final
		{
		}

		void OnUartRx(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize) final
		{
			switch ((VirtualPadUartInterface::MessageEnum)header)
			{
			case VirtualPadUartInterface::MessageEnum::UpdateFeedback:
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
}
#endif