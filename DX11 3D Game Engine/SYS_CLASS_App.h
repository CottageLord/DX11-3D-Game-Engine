/**
 * @file SYS_CLASS_App.h
 * @brief Specifies the main loop logic (engine update)
 */
#pragma once
#include "SYS_SET_FilterWinH.h"
#include "SYS_CLASS_Window.h"
#include "SYS_CLASS_Timer.h"
#include "GRAPHICS_OBJ_Box.h"
#include "GRAPHICS_OBJ_Melon.h"
#include "GRAPHICS_OBJ_Pyramid.h"
#include <algorithm>
#include "SYS_SET_Math.h"
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
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawables = 180;
};