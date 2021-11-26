/*
	Description : The window startup (main entry) to the engine program
	Author		: Yang Hu
	Date		: 08/22/2021  
*/

#include "SYS_SET_Config.h"
#include "SYS_CLASS_Window.h"
#include "SYS_CLASS_App.h"

int CALLBACK WinMain(		// CALLBACK modifies the function, specifies stdcall calling convention that Windows uses
	HINSTANCE hInstance,	// Where the window object resides
	HINSTANCE hPrevInstance,// Always Null, Obsolete
	LPSTR	  lpCmdLine,	// Receive command line - string tokens
	int		  nCmdShow		// Indicates how the window should be displayed, for example SW_SHOW_MINIMIZED
)
{
	try 
	{
		// initialize and start looping
		return App{ lpCmdLine }.Go();
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
