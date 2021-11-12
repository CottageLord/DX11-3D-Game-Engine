/**
 * @file SYS_CLASS_App.h
 * @brief Specifies the main loop logic (engine update)
 */
#pragma once
#include "SYS_SET_FilterWinH.h"
#include "SYS_CLASS_Window.h"
#include "SYS_CLASS_Timer.h"
#include "SYS_CLASS_ImguiManager.h"
#include "GRAPHICS_OBJ_Camera.h"
#include "GRAPHICS_OBJ_Mesh.h"
#include "GRAPHICS_LGT_PointLight.h"
#include <set>

#include "TestPlane.h"

class App
{
public:
	App();
	// master frame / message loop
	int Go();
	~App();
private:
	void DoFrame();
	void ShowImguiDemoWindow();
private:
	bool show_demo_window = true;
	ImguiManager imgui;
	Window wnd;
	EngineTimer timer;
	Camera cam;
	PointLight light;
	float speed_factor = 1.0f;
	Model nano { wnd.Gfx(),"models\\nano_textured\\nanosuit.obj" };
	Model nano2{ wnd.Gfx(),"Models\\nano_textured\\nanosuit.obj" };
	//Model nano{ wnd.Gfx(),"Models\\zhongli_textured\\zhongli.pmx" };
	TestPlane plane;
};