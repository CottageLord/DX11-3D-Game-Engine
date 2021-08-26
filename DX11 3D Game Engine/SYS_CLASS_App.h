#pragma once
#include "SYS_SET_FilterWinH.h"
#include "SYS_CLASS_Window.h"
#include "SYS_CLASS_Timer.h"
#include <iomanip>

class App
{
public:
	App();
	// master frame / message loop
	int Go();
private:
	void DoFrame();
private:
	Window wnd;
	EngineTimer timer;
};