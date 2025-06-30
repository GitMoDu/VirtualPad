#ifndef _VIRTUAL_PADS_INCLUDE_h
#define _VIRTUAL_PADS_INCLUDE_h

#if defined(ARDUINO)
#include "VirtualPads/JoystickShieldVirtualPad.h"

#include "VirtualPads/AtariJoystickVirtualPad.h"

#include "VirtualPads/MegaDriveVirtualPad.h"

#include "VirtualPads/Nintendo64VirtualPad.h"
#include "VirtualPads/GameCubeVirtualPad.h"
#endif

#if defined(_WINDOWS_)
#include "VirtualPads/WindowsVirtualPad.h"
#endif

#endif