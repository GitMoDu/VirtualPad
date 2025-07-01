/*
* Example VirtualPad UART Client.
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
#include "PadListenerLogger.h"


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

// Pad sink definition.
using PadType = VirtualPad::AnalogVirtualPad<JoystickShield::ConfigurationCode>;

// Process scheduler.
TS::Scheduler SchedulerBase{};

// Pad instance.
PadType Pad{};

// Pad updater task.
PadListenerLogger<VirtualPad::analog_pad_state_t> PadLogger{};

// UART Client for Virtual Pad.
VirtualPadUartClientTask<UartType> UartClient(SchedulerBase, UartModem, Pad);

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

	PadLogger.Setup();

	UartClient.Start();
}

void loop()
{
	SchedulerBase.execute();
}
