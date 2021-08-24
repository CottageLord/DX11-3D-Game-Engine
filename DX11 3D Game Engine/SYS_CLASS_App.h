#pragma once
#include "SYS_SET_FilterWinH.h"
#include "SYS_CLASS_Window.h"

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
};