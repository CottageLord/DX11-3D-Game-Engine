#include "SYS_CLASS_App.h"
#include "SYS_SET_Math.h"
#include "GRAPHICS_OBJ_Surface.h"
#include "GRAPHICS_BUF_VertexBuffer.h"
#include "GRAPHICS_HELP_TexturePreprocessor.h"
#include "SYS_HELP_Utility.h"
#include "Testing.h"

#include "imgui/imgui.h"
#include <DirectXTex.h>

#include <algorithm>
#include <memory>
#include <shellapi.h>

namespace dx = DirectX;

App::App(const std::string& commandLine)
	:
	wnd( 1280,720,"°¢Ã©µÄÒýÇæ" ),
	scriptCommander(TokenizeQuoted(commandLine)),
	light(wnd.Gfx())
{
	cube.SetPos({ 4.0f,0.0f,0.0f });
	cube2.SetPos({ 0.0f,4.0f,0.0f });
	//TestDynamicConstant();
	//wall.SetRootTransform(dx::XMMatrixTranslation(-12.0f, 0.0f, 0.0f));
	//tp.SetPos({ 12.0f,0.0f,0.0f });
	//gobber.SetRootTransform(dx::XMMatrixTranslation(0.0f, -10.0f, 0.0f));
	
	//nano.SetRootTransform(dx::XMMatrixTranslation(0.0f, -7.0f, 6.0f));
	//bluePlane.SetPos(cam.GetPos());
	//redPlane.SetPos(cam.GetPos());

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

	light.Submit(fc);
	//sponza.Draw( wnd.Gfx() );
	cube.Submit(fc);
	cube2.Submit(fc);
	//bluePlane.Draw(wnd.Gfx());
	//redPlane.Draw(wnd.Gfx());
	fc.Execute(wnd.Gfx());
	

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
	//bluePlane.SpawnControlWindow(wnd.Gfx(), "Blue Plane");
	//redPlane.SpawnControlWindow(wnd.Gfx(), "Red Plane");

	//ShowImguiDemoWindow();
	/*
	wall.ShowWindow(wnd.Gfx(), "Wall");
	tp.SpawnControlWindow(wnd.Gfx());
	nano.ShowWindow(wnd.Gfx(), "Nano");
	*/
	//gobber.ShowWindow(wnd.Gfx(), "Zhongli");
	cube.SpawnControlWindow(wnd.Gfx(), "Cube 1");
	cube2.SpawnControlWindow(wnd.Gfx(), "Cube 2");
	//sponza.ShowWindow(wnd.Gfx(), "Sponza");
	// present
	wnd.Gfx().EndFrame();
	fc.Reset();
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