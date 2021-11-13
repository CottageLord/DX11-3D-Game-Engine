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

	//Model zhongli{ wnd.Gfx(),"Models\\zhongli_textured\\zhongli.pmx" };
	Model gobber{ wnd.Gfx(),"Models\\gobber\\GoblinX.obj",6.0f };
	Model wall{ wnd.Gfx(),"Models\\brick_wall\\brick_wall.obj",6.0f };
	TestPlane tp{ wnd.Gfx(),6.0 };
	Model nano{ wnd.Gfx(),"Models\\nano_textured\\nanosuit.obj",2.0f };
	//TestPlane plane;
};