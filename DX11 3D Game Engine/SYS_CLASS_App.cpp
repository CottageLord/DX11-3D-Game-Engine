#include "SYS_CLASS_App.h"
#include "SYS_HELP_Utility.h"
#include "SYS_HELP_PerfLog.h"
#include "GRAPHICS_HELP_TexturePreprocessor.h"
#include "GRAPHICS_HELP_MatrixTranslator.h"
#include "GRAPHICS_JOB_TechniqueProbe.h"
#include "GRAPHICS_OBJ_Camera.h"
#include "GRAPHICS_OBJ_Channels.h"

#include "Testing.h"
#include "TestModelProbe.h"

#include "imgui/imgui.h"

#include <DirectXTex.h>
#include <algorithm>
#include <shellapi.h>

namespace dx = DirectX;

App::App(const std::string& commandLine)
	:
	wnd( 1280,720,"°¢Ã©µÄÒýÇæ" ),
	scriptCommander(TokenizeQuoted(commandLine)),
	light(wnd.Gfx(), { 10.0f,5.0f,0.0f })
{
	cameras.AddCamera(std::make_unique<Camera>(wnd.Gfx(), "A", dx::XMFLOAT3{ -13.5f,6.0f,3.5f }, 0.0f, PI / 2.0f));
	cameras.AddCamera(std::make_unique<Camera>(wnd.Gfx(), "B", dx::XMFLOAT3{ -13.5f,28.8f,-6.4f }, PI / 180.0f * 13.0f, PI / 180.0f * 61.0f));
	
	cameras.AddCamera(light.ShareCamera());
	//TestDynamicConstant();
	cube.SetPos({ 10.0f,5.0f,6.0f });
	cube2.SetPos({ 10.0f,5.0f,14.0f });

	nano.SetRootTransform(
		dx::XMMatrixRotationY(PI / 2.f) *
		dx::XMMatrixTranslation(27.f, -0.56f, 1.7f)
	);
	/*
	gobber.SetRootTransform(
		dx::XMMatrixRotationY(-PI / 2.f) *
		dx::XMMatrixTranslation(-8.f, 10.f, 0.f)
	);*/

	cube.LinkTechniques(rg);
	cube2.LinkTechniques(rg);
	light.LinkTechniques(rg);
	sponza.LinkTechniques(rg);
	//gobber.LinkTechniques(rg);
	nano.LinkTechniques(rg);

	//TestDynamicConstant();
	//wall.SetRootTransform(dx::XMMatrixTranslation(-12.0f, 0.0f, 0.0f));
	//tp.SetPos({ 12.0f,0.0f,0.0f });
	//gobber.SetRootTransform(dx::XMMatrixTranslation(0.0f, -10.0f, 0.0f));
	
	//nano.SetRootTransform(dx::XMMatrixTranslation(0.0f, -7.0f, 6.0f));
	//bluePlane.SetPos(cam.GetPos());
	//redPlane.SetPos(cam.GetPos());

	cameras.LinkTechniques(rg);
	rg.BindShadowCamera(*light.ShareCamera());
}

void App::HandleInput( float dt )
{
	while( const auto e = wnd.kbd.ReadKey() )
	{
		if( !e->IsPress() )
		{
			continue;
		}

		switch( e->GetCode() )
		{
		case VK_ESCAPE:
			if( wnd.CursorEnabled() )
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
			showDemoWindow = true;
			break;
		case VK_RETURN:
			savingDepth = true;
			break;
		}
	}

	if( !wnd.CursorEnabled() )
	{
		if( wnd.kbd.KeyIsPressed( 'W' ) )
		{
			cameras->Translate( { 0.0f,0.0f,dt } );
		}
		if( wnd.kbd.KeyIsPressed( 'A' ) )
		{
			cameras->Translate( { -dt,0.0f,0.0f } );
		}
		if( wnd.kbd.KeyIsPressed( 'S' ) )
		{
			cameras->Translate( { 0.0f,0.0f,-dt } );
		}
		if( wnd.kbd.KeyIsPressed( 'D' ) )
		{
			cameras->Translate( { dt,0.0f,0.0f } );
		}
		if( wnd.kbd.KeyIsPressed( 'R' ) )
		{
			cameras->Translate( { 0.0f,dt,0.0f } );
		}
		if( wnd.kbd.KeyIsPressed( 'F' ) )
		{
			cameras->Translate( { 0.0f,-dt,0.0f } );
		}
	}

	while( const auto delta = wnd.mouse.ReadRawDelta() )
	{
		if( !wnd.CursorEnabled() )
		{
			cameras->Rotate( (float)delta->x,(float)delta->y );
		}
	}
}

void App::DoFrame( float dt )
{
	wnd.Gfx().BeginFrame( 0.07f,0.0f,0.12f );
	light.Bind( wnd.Gfx(), cameras->GetMatrix() );
	rg.BindMainCamera(cameras.GetActiveCamera());

	light.Submit(Channel::main);
	cube.Submit(Channel::main);
	sponza.Submit(Channel::main);
	cube2.Submit(Channel::main);
	nano.Submit(Channel::main);
	cameras.Submit(Channel::main);

	sponza.Submit(Channel::shadow);
	cube.Submit(Channel::shadow);
	sponza.Submit(Channel::shadow);
	cube2.Submit(Channel::shadow);
	nano.Submit(Channel::shadow);

	rg.Execute(wnd.Gfx());

	if (savingDepth)
	{
		rg.DumpShadowMap(wnd.Gfx(), "shadow.png");
		savingDepth = false;
	}
	/*
	// imgui windows


	*/

	static MP sponzeProbe{ "Sponza" };
	//static MP gobberProbe{ "Gobber" };
	static MP nanoProbe{ "Nano" };
	sponzeProbe.SpawnWindow(sponza);
	//gobberProbe.SpawnWindow(gobber);
	nanoProbe.SpawnWindow(nano);

	cameras.SpawnWindow(wnd.Gfx());

	light.SpawnControlWindow();
	ShowImguiDemoWindow();
	cube.SpawnControlWindow( wnd.Gfx(),"Cube 1" );
	cube2.SpawnControlWindow( wnd.Gfx(),"Cube 2" );

	rg.RenderWidgets(wnd.Gfx());
	// present
	wnd.Gfx().EndFrame();
	rg.Reset();

	if (savingDepth)
	{
		rg.StoreDepth(wnd.Gfx(), "depth.png");
		savingDepth = false;
	}
}

void App::ShowImguiDemoWindow()
{
	if( showDemoWindow )
	{
		ImGui::ShowDemoWindow( &showDemoWindow );
	}
}

App::~App()
{}

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
		// execute the game logic
		const auto dt = timer.Mark() * speed_factor;
		HandleInput( dt );
		DoFrame( dt );
	}
}