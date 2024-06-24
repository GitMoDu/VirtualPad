/*
* Example IInputController exposed ash native composite USB controller (XBox 360).
* Designed for STM32 using ArduinoSTM32 (https://github.com/rogerclarkmelbourne/Arduino_STM32)
* USB interface depends on https://github.com/arpruss/USBComposite_stm32f1
* Scheduler depends on  https://github.com/arkhipenko/TaskScheduler
* Nintendo controllers depend on https://github.com/GitMoDu/JoybusOverUart
*
* Uncomment the desired controller to use.
*/


// Uncomment controller to use.
//#define USE_N64_CONTROLLER
#define USE_GAMECUBE_CONTROLLER

#define _TASK_OO_CALLBACKS
#define _TASK_SLEEP_ON_IDLE_RUN
#include <TaskScheduler.h>

#include <InputControllers.h>


#include <Nintendo64ControllerReadTask.h>
#include <GameCubeControllerReadTask.h>
#include "Nintendo64ToX360Task.h"
#include "GameCubeToX360Task.h"


static constexpr uint32_t ReadPeriodMicros = 3000;
static constexpr uint32_t UpdatePeriodMicros = 5000;
static constexpr HardwareSerial* JoyBusSerial = &Serial3;


// Process scheduler.
Scheduler SchedulerBase{};

// XBox controller API.
USBXBox360 XBox360{};

#if defined(USE_N64_CONTROLLER)
// Raw Controller data with extra state flag.
Nintendo64Controller::state_data_t ControllerState{};

// Controller read task.
Nintendo64ControllerReadTask ReadTask(&SchedulerBase, ControllerState, JoyBusSerial, ReadPeriodMicros);

// Map and update task.
Nintendo64ToX360Task<> UpdateTask(&SchedulerBase, XBox360, ControllerState, UpdatePeriodMicros);
#elif defined(USE_GAMECUBE_CONTROLLER)
// Raw Controller data with extra state flag.
GameCubeController::state_data_t ControllerState{};

// Controller read task.
GameCubeControllerReadTask ReadTask(&SchedulerBase, ControllerState, JoyBusSerial, ReadPeriodMicros);

// Map and update task.
GameCubeToX360Task<> UpdateTask(&SchedulerBase, XBox360, ControllerState, UpdatePeriodMicros);
#endif


void setup()
{
	// Start USB the device.
	XBox360.begin();
	while (!USBComposite);

	// Start the raw controller reader.
	ReadTask.Start();

	// Start the updater.
	UpdateTask.Start();
}

void loop()
{
	SchedulerBase.execute();
}