#include "SYS_CLASS_App.h"

App::App()
	:
	wnd(800, 600, "°¢Ã©µÄÒýÇæ")
{}

int App::Go()
{
	// GetMessage blocks when no message comes in
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
		Sleep(1); // <---------------------- A temporary fix, remove later
	}

}

void App::DoFrame()
{
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.0f);

	wnd.Gfx().DrawTestTriangle(
		timer.Peek(),
		wnd.mouse.GetPosX() / 400.0f - 1.0f,
		-wnd.mouse.GetPosY() / 300.0f + 1.0f
	);

	while (const auto e = wnd.mouse.Read())
	{
		switch (e->GetType())
		{
		case Mouse::Event::Type::Move:
		{
			std::ostringstream oss;
			oss << "Mouse moved to: (" << e->GetPosX() << "," << e->GetPosY() << ")\n";
			wnd.SetTitle(oss.str());
			break;
		}
		}
	}
	wnd.Gfx().EndFrame();
}