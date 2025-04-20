/*
* Example VirtualPad exposed as native composite USB controller (XBox 360).
* Designed for STM32 using ArduinoSTM32 (https://github.com/rogerclarkmelbourne/Arduino_STM32)
* USB interface depends on https://github.com/arpruss/USBComposite_stm32f1
* Scheduler depends on  https://github.com/arkhipenko/TaskScheduler
* Nintendo controllers depend on https://github.com/GitMoDu/JoybusOverUart
*
* Uncomment the desired controller to use.
*/

#if defined(ARDUINO_ARCH_STM32F1)
#else
#error This example only supports STM32F1 with Rogers core https://github.com/rogerclarkmelbourne/Arduino_STM32
#endif

// Uncomment controller to use.
#define USE_JOYSTICK_SHIELD_CONTROLLER
//#define USE_N64_CONTROLLER
//#define USE_GAMECUBE_CONTROLLER

#define _TASK_OO_CALLBACKS
#define _TASK_SLEEP_ON_IDLE_RUN
#include <TaskScheduler.h>

#include <USBXBox360.h>
#include <VirtualPad.h>

static constexpr uint32_t ReadPeriodMicros = 3000;
static constexpr uint32_t UpdatePeriodMicros = 5000;
static constexpr HardwareSerial* JoyBusSerial = &Serial3;


// Process scheduler.
Scheduler SchedulerBase{};

// XBox controller API.
USBXBox360 XBox360{};

#if defined(USE_JOYSTICK_SHIELD_CONTROLLER)
#include "JoystickShieldToXBoxTask.h"
#if defined(ARDUINO_ARCH_STM32F1)
static constexpr uint8_t JoyXPin = PA0;
static constexpr uint8_t JoyYPin = PA1;
#else
static constexpr uint8_t JoyXPin = A0;
static constexpr uint8_t JoyYPin = A1;
#endif
// Controller read, map and update task.
JoystickShieldToXBoxTask<JoyXPin, JoyYPin, 4, 3, 5, 2, 8, 6, 7> UpdateTask(&SchedulerBase, XBox360, UpdatePeriodMicros);

#elif defined(USE_N64_CONTROLLER)
#include <Nintendo64ControllerReadTask.h>
#include "Nintendo64ToXBoxTask.h"

// Raw Controller data with extra state flag.
Nintendo64Controller::state_data_t ControllerState{};

// Controller read task.
Nintendo64ControllerReadTask ReadTask(&SchedulerBase, ControllerState, JoyBusSerial, ReadPeriodMicros);

// Map and update task.
Nintendo64ToXBoxTask<> UpdateTask(&SchedulerBase, XBox360, ControllerState, UpdatePeriodMicros);
#elif defined(USE_GAMECUBE_CONTROLLER)
#include <GameCubeControllerReadTask.h>
#include "GameCubeToXBoxTask.h"

// Raw Controller data with extra state flag.
GameCubeController::state_data_t ControllerState{};

// Controller read task.
GameCubeControllerReadTask ReadTask(&SchedulerBase, ControllerState, JoyBusSerial, ReadPeriodMicros);

// Map and update task.
GameCubeToXBoxTask<> UpdateTask(&SchedulerBase, XBox360, ControllerState, UpdatePeriodMicros);
#endif


void setup()
{
	// Start USB the device.
	XBox360.begin();
	while (!USBComposite);

#if defined(USE_N64_CONTROLLER) || defined(USE_GAMECUBE_CONTROLLER)
	// Start the raw controller reader.
	ReadTask.Start();
#endif

	// Start the updater.
	UpdateTask.Start();
}

void loop()
{
	SchedulerBase.execute();
}