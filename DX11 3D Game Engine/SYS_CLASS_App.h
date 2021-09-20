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

class App
{
public:
	App();
	// master frame / message loop
	int Go();
	~App();
private:
	void DoFrame();
	void ShowModelWindow();
private:
	ImguiManager imgui;
	Window wnd;
	EngineTimer timer;
	Camera cam;
	PointLight light;
	float speed_factor = 1.0f;
	Model nano{ wnd.Gfx(),"Models\\nanosuit.obj" };
	struct
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} pos;
};