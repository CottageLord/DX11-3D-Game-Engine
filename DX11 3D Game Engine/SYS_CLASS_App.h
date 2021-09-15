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
	void SpawnSimulationWindow() noexcept;
	void SpawnBoxWindowManagerWindow() noexcept;
	void SpawnBoxWindows() noexcept;
private:
	ImguiManager imgui;
	Window wnd;
	EngineTimer timer;
	Camera cam;
	PointLight light;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	std::vector<class Box*> boxes;
	static constexpr size_t nDrawables = 180;
	std::optional<int> comboBoxIndex;
	std::set<int> boxControlIds;
	float speed_factor = 1.0f;
};