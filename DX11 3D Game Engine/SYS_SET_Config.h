/*
	Description : The Win32 Application configuration file that contains all #include and constants
	Author		: Yang Hu
	Date		: 08/22/2021
*/

#pragma once
// Always include this on the top, to avoid Windows.h spread codes on its own
#include "SYS_SET_FilterWinH.h"
#include <string>
#include <sstream>

// the window class name for window registration
const std::string pClassName { "engine_window_YH" };
const std::string pWindowName { "engine main window" };
// window position for window init
const int initWindowX { 200 };
const int initWindowY { 200 };
const int initWindowW { 640 };
const int initWindowH { 480 };
// application return status codes
const int MY_QUIT_STATUS { 69 };