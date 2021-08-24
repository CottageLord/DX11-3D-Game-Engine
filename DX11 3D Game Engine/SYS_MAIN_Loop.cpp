/*
	Description : The window startup (main entry) to the engine program
	Author		: Yang Hu
	Date		: 08/22/2021  
*/

#include "SYS_SET_Config.h"
#include "SYS_CLASS_Window.h"

// helper func that ocnverts str to wstr
std::wstring s2ws(const std::string& s);
// a costimized window procedure that processes msgs
LRESULT CALLBACK MyWndProc( HWND hWindow, UINT msgID, WPARAM wParams, LPARAM lParams);

int CALLBACK WinMain(		// CALLBACK modifies the function, specifies stdcall calling convention that Windows uses
	HINSTANCE hInstance,	// Where the window object resides
	HINSTANCE hPrevInstance,// Always Null, Obsolete
	LPSTR	  lpCmdLine,	// Receive command line - string tokens
	int		  nCmdShow		// Indicates how the window should be displayed, for example SW_SHOW_MINIMIZED
)
{
	// ====================== register window class ======================= //
	Window myWnd(640, 480, "Game Engine Main");
	MSG winMessage;
	BOOL gResult;
	try 
	{
		while ((gResult = GetMessage(&winMessage, nullptr, 0, 0)) > 0) {
			TranslateMessage(&winMessage); // will not modify msg, generating WM_CHAR along with WM_KEY_DOWN
			DispatchMessage(&winMessage);
		}
		// returns the window message status
		if (gResult == -1) return -1;
		else return winMessage.wParam;
	}
	catch (const MFException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	// check if GetMessage call itself borked
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}
