/*
* Example VirtualPad UART Server.
* Depends on:
*	UART Interface https://github.com/GitMoDu/UartInterface
*	Task Scheduler https://github.com/arkhipenko/TaskScheduler
*/

//#define DEBUG
#define SERIAL_BAUD_RATE 115200

#define _TASK_OO_CALLBACKS
#include <TScheduler.hpp>

#include <VirtualPadUartInterface.h>
#include <VirtualPads.h>
#include "PadStepperTask.h"

// UART modem definition.
#if defined(ARDUINO_ARCH_NRF52)
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#ifdef DEBUG
using UartType = Adafruit_USBD_CDC;
UartType& UartModem(Serial);
#else
using UartType = Uart;
UartType& UartModem(Serial1);
#endif
#else
using UartType = HardwareSerial;
#if defined(ARDUINO_ARCH_AVR)
UartType& UartModem(Serial);
#else
UartType& UartModem(Serial1);
#endif
#endif

// Pad source definition.
#if defined(ARDUINO_ARCH_STM32F1)
static constexpr uint8_t JoyXPin = PA0;
static constexpr uint8_t JoyYPin = PA1;
#else
static constexpr uint8_t JoyXPin = A0;
static constexpr uint8_t JoyYPin = A1;
#endif
using PadType = JoystickShield::JoystickShieldVirtualPad<JoyXPin, JoyYPin, 4, 3, 5, 2, 8, 6, 7>;

// Process scheduler.
TS::Scheduler SchedulerBase{};

// Pad instance.
PadType Pad{};

// Pad updater task.
PadStepperTask<PadType> PadStepper(SchedulerBase, Pad);

// UART Server for Virtual Pad.
VirtualPadUartServerTask<UartType> UartServer(SchedulerBase, UartModem, Pad);

void Halt()
{
#ifdef DEBUG
	Serial.println();
	Serial.println("Critical Error");
	delay(1000);
#endif	
	while (1);;
}

void setup()
{
#if defined(DEBUG)
	Serial.begin(SERIAL_BAUD_RATE);
	while (!Serial)
		;
	delay(1000);
	Serial.print(F("VirtualPad UART Server"));
#endif

	if (!Pad.Setup())
	{
		Halt();
	}

	// Wait for stepper task to update controller state.
	Pad.SetConnected(false);

	// Start server.
	UartServer.Start();
}

void loop()
{
	SchedulerBase.execute();
}
