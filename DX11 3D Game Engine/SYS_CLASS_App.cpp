#include "SYS_CLASS_App.h"
#include "SYS_SET_Math.h"
#include "GRAPHICS_SET_GDIPlusManager.h"
#include "GRAPHICS_OBJ_Surface.h"
#include "GRAPHICS_BUF_VertexBuffer.h"
//#include "GRAPHICS_HELP_NormalMapTwerker.h"
#include "GRAPHICS_HELP_TexturePreprocessor.h"
//#include "GRAPHICS_OBJ_Box.h"
//#include "GRAPHICS_OBJ_Melon.h"
//#include "GRAPHICS_OBJ_Pyramid.h"
//#include "GRAPHICS_OBJ_Sheet.h"
//#include "GRAPHICS_OBJ_SkinnedBox.h"
//#include "GRAPHICS_OBJ_Cylinder.h"
//#include "GRAPHICS_OBJ_DynamicVertex.h"
//#include "AssTest.h"

#include "imgui/imgui.h"

#pragma comment(lib,"assimp-vc142-mtd.lib")

#include <algorithm>
#include <memory>
#include <shellapi.h>

namespace dx = DirectX;
GDIPlusManager gdipm;

App::App(const std::string& commandLine)
	:
	wnd( 1280,720,"°¢Ã©µÄÒýÇæ" ),
	light(wnd.Gfx())
{
	// makeshift cli for doing some preprocessing bullshit (so many hacks here)
	if (this->commandLine != "")
	{
		int nArgs;
		const auto pLineW = GetCommandLineW();
		const auto pArgs = CommandLineToArgvW(pLineW, &nArgs);
		if (nArgs >= 3 && std::wstring(pArgs[1]) == L"--twerk-objnorm")
		{
			const std::wstring pathInWide = pArgs[2];
			TexturePreprocessor::FlipYAllNormalMapsInObj(
				std::string(pathInWide.begin(), pathInWide.end())
			);
			throw std::runtime_error("Normal maps all processed successfully. Just kidding about that whole runtime error thing.");
		}
		else if (nArgs >= 4 && std::wstring(pArgs[1]) == L"--twerk-validate")
		{
			const std::wstring minWide = pArgs[2];
			const std::wstring maxWide = pArgs[3];
			const std::wstring pathWide = pArgs[4];
			TexturePreprocessor::ValidateNormalMap(
				std::string(pathWide.begin(), pathWide.end()), std::stof(minWide), std::stof(maxWide)
			);
			throw std::runtime_error("Normal map validated successfully. Just kidding about that whole runtime error thing.");
		} 
		else if (nArgs >= 3 && std::wstring(pArgs[1]) == L"--twerk-flipy")
		{
			const std::wstring pathInWide = pArgs[2];
			const std::wstring pathOutWide = pArgs[3];
			TexturePreprocessor::FlipYNormalMap(
				std::string(pathInWide.begin(), pathInWide.end()),
				std::string(pathOutWide.begin(), pathOutWide.end())
			);
			throw std::runtime_error("Normal map processed successfully. Just kidding about that whole runtime error thing.");
		}
	}

	//wall.SetRootTransform(dx::XMMatrixTranslation(-12.0f, 0.0f, 0.0f));
	//tp.SetPos({ 12.0f,0.0f,0.0f });
	//gobber.SetRootTransform(dx::XMMatrixTranslation(0.0f, 0.0f, -4.0f));
	//nano.SetRootTransform(dx::XMMatrixTranslation(0.0f, -7.0f, 6.0f));
	bluePlane.SetPos(cam.GetPos());

	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 400.0f));
}

int App::Go()
{
	while( true )
	{
		// process all messages pending, but to not block for new messages
		if( const auto ecode = Window::ProcessMessages() )
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	const auto dt = timer.Mark() * speed_factor;

	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);

	wnd.Gfx().SetCamera(cam.GetMatrix());

	// bind the light info so it could be accessed by all drawables
	light.Bind(wnd.Gfx(), cam.GetMatrix());

	//wall.Draw(wnd.Gfx());
	//tp.Draw(wnd.Gfx());
	//nano.Draw(wnd.Gfx());
	//gobber.Draw(wnd.Gfx());

	light.Draw(wnd.Gfx());
	sponza.Draw(wnd.Gfx());
	bluePlane.Draw(wnd.Gfx());

	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress()) { continue; }

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd.CursorEnabled())
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;
		case VK_F1:
			show_demo_window = true;
			break;
		}
	}
	if (!wnd.CursorEnabled())
	{
		if (wnd.kbd.KeyIsPressed('W'))	{cam.Translate({ 0.0f,0.0f,dt });}
		if (wnd.kbd.KeyIsPressed('A'))	{cam.Translate({ -dt,0.0f,0.0f });}
		if (wnd.kbd.KeyIsPressed('S'))	{cam.Translate({ 0.0f,0.0f,-dt });}
		if (wnd.kbd.KeyIsPressed('D'))	{cam.Translate({ dt,0.0f,0.0f });}
		if (wnd.kbd.KeyIsPressed('R'))	{cam.Translate({ 0.0f,dt,0.0f });}
		if (wnd.kbd.KeyIsPressed('F'))	{cam.Translate({ 0.0f,-dt,0.0f });}
	}


	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			cam.Rotate((float)delta->x, (float)delta->y);
		}
	}
	// imgui windows
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	bluePlane.SpawnControlWindow(wnd.Gfx());

	//ShowImguiDemoWindow();
	/*
	wall.ShowWindow(wnd.Gfx(), "Wall");
	tp.SpawnControlWindow(wnd.Gfx());
	nano.ShowWindow(wnd.Gfx(), "Nano");
	gobber.ShowWindow(wnd.Gfx(), "gobber");
	*/

	sponza.ShowWindow(wnd.Gfx(), "Sponza");
	// present
	wnd.Gfx().EndFrame();
	// imgui windows to control camera and light
}

void App::ShowImguiDemoWindow()
{
	if (show_demo_window)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}
}

App::~App()
{}