/*
* Example VirtualPad Dispatcher Task.
* Reads a source controller, translates and writes to VirtualPad.
* Logs the controller actions to Serial.
* Uncomment the desired source controller to use.
* Nintendo controllers depend on https://github.com/GitMoDu/JoybusOverUart
*/


// Uncomment controller to use.
//#define DIRECT_CONTROLLER
#define USE_N64_CONTROLLER
//#define USE_GAMECUBE_CONTROLLER

#define SERIAL_BAUD_RATE 115200

#define _TASK_OO_CALLBACKS
#define _TASK_SLEEP_ON_IDLE_RUN
#include <TaskScheduler.h>

#include <VirtualPadSources.h>
#include <VirtualPadTaskSources.h>
#include <VirtualPadDispatchTask.h>


#include "DispatchLogger.h"
#include "TemplateVirtualPadMapperTask.h"

static constexpr uint32_t ReadPeriodMicros = 3000;
static constexpr uint32_t UpdatePeriodMillis = 10;
static constexpr uint32_t LogPeriodMillis = 100;

#if defined(USE_N64_CONTROLLER) || defined(USE_GAMECUBE_CONTROLLER)
static constexpr HardwareSerial* JoyBusSerial = &Serial3;
#endif

// Process scheduler.
Scheduler SchedulerBase{};

#if defined(DIRECT_CONTROLLER)
// Controller data and reader.
DirectInputVirtualPadWriter Pad(PA0, PA1, 3, 4);
// Updater task.
DirectInputVirtualPadWriterTask ReadTask(&SchedulerBase, &Pad, ReadPeriodMicros / 1000);
#elif defined(USE_N64_CONTROLLER)
#include <Nintendo64ControllerReadTask.h>
// Raw Controller state.
Nintendo64Controller::state_data_t ControllerState{};

// Controller read task.
Nintendo64ControllerReadTask ReadTask(&SchedulerBase, ControllerState, JoyBusSerial, ReadPeriodMicros);

// Mapped controller data.
using ControllerType = Nintendo64VirtualPadWriter<>;
Nintendo64VirtualPadWriter<> Pad{};

// Nintendo64ControllerReadTask only updates ControllerState, requiring a separate task to map to the Pad.
TemplateVirtualPadMapperTask<Nintendo64Controller::state_data_t, ControllerType> MapperTask(SchedulerBase, ControllerState, Pad, ReadPeriodMicros / 1000);
#elif defined(USE_GAMECUBE_CONTROLLER)
#include <GameCubeControllerReadTask.h>
// Raw Controller state.
GameCubeController::state_data_t ControllerState{};

// Controller read task.
GameCubeControllerReadTask ReadTask(&SchedulerBase, ControllerState, JoyBusSerial, ReadPeriodMicros);

// Mapped controller data.
GameCubeVirtualPadWriter<> Pad{};
using ControllerType = GameCubeVirtualPadWriter<>;

// GameCubeControllerReadTask only updates ControllerState, requiring a separate task to map to the Pad.
TemplateVirtualPadMapperTask<GameCubeController::state_data_t, ControllerType> MapperTask(SchedulerBase, ControllerState, Pad, ReadPeriodMicros / 1000);
#endif


// Dispatcher Task.
VirtualPadDispatchTask Dispatcher(SchedulerBase, &Pad, UpdatePeriodMillis);

// Dispatch Listener.
DispatchLogger Logger(LogPeriodMillis);

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
	Serial.begin(SERIAL_BAUD_RATE);
	while (!Serial)
		;
	delay(1000);
	Serial.print(F("Dispatcher Demo..."));

#if defined(DIRECT_CONTROLLER)
	if (!Pad.Setup())
	{
		Halt();
	}
#endif

	// Start Dispatcher task.
	Dispatcher.SetDispatch(&Logger);
	Dispatcher.Start();

	// Start source and mapper task.
	ReadTask.Start();

#if defined(DIRECT_CONTROLLER)
#else
	MapperTask.Start();
#endif

	Serial.println(F("Starting!"));

}

void loop()
{
	SchedulerBase.execute();
}
