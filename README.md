# IInputController

Abstracted controller, inspired by RetroArch's RetroPad.

IInputController

	- Read API for controller state.

	- Optional shallow, for double buffering state.

	- Bit field button state, optimized for low RAM and fast copy.

WriteInputController
	- Inherits from IInputController.
	- Write API for controller state.



# Examples provided

- Arduino ADC+IO simple IInputController

- Nintendo controllers to IInputController
	- Depends on https://github.com/GitMoDu/JoybusOverUart

- Passthrough to XBox360
	- STM32Duino only https://github.com/rogerclarkmelbourne/Arduino_STM32
	- Depends on https://github.com/arpruss/USBComposite_stm32f1


# Task Controller Dependencies

- Task Scheduler https://github.com/arkhipenko/TaskScheduler

