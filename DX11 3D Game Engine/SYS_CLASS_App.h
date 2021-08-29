#pragma once
#include "SYS_SET_FilterWinH.h"
#include "SYS_CLASS_Window.h"
#include "SYS_CLASS_Timer.h"
#include "GRAPHICS_OBJ_Box.h"
#include <memory>

class App
{
public:
	App();
	// master frame / message loop
	int Go();
	~App();
private:
	void DoFrame();
private:
	Window wnd;
	EngineTimer timer;
	std::vector<std::unique_ptr<class Box>> boxes;
};