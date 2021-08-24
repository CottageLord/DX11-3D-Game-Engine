#pragma once
#include "config.h"

// a costimized window procedure that processes msgs
LRESULT CALLBACK MyWndProc(HWND hWindow, UINT msgID, WPARAM wParam, LPARAM lParam) {

	static std::string title;
	switch (msgID)
	{
	case WM_CLOSE:
		PostQuitMessage(MY_QUIT_STATUS);
		break;
	
	// no capital letter accepted (Shift + D)
	case WM_KEYDOWN:
		switch ( wParam )
		{
		case 'F':
			SetWindowText( hWindow, "Respect" );
			break;
		}
		break;
		// text input only
	
	case WM_KEYUP:
		switch ( wParam )
		{
		case 'F':
			SetWindowText(hWindow, "No Respect");
			break;
		}
		break;
	// overrides WM_KEYDOWN
	case WM_CHAR:
		/*
		title.push_back((char)wParam);
		SetWindowText(hWindow, title.c_str());
		*/
		break;
	case WM_LBUTTONDOWN:
		POINTS mousePt = MAKEPOINTS( lParam );
		/*
		std::ostringstream oss;
		oss << "[" << mousePt.x << "," << mousePt.y << "]";
		SetWindowText(hWindow, oss.str().c_str());
		*/
		break;
	}

	return DefWindowProc(hWindow, msgID, wParam, lParam);
}


// converts string to wstring
// This is also one of the reasons why CPP is annoying :)
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

// fill info into class register struct
WNDCLASSEX configureClassRegister(WNDCLASSEX &infoStruct) {
	/*
		typedef struct tagWNDCLASSEXW {
		UINT      cbSize;		// Set this member to sizeof(WNDCLASSEX). before calling the GetClassInfoEx function.
		UINT      style;		// The class style(s)
		WNDPROC   lpfnWndProc;	// A pointer to the window procedure. You must use the CallWindowProc function to call the window procedure.
		int       cbClsExtra;	// allocate extra bytes to store costom data
		int       cbWndExtra;	// store costomized data on windows api side
		HINSTANCE hInstance;	// A handle to the instance that contains the window procedure for the class.
		HICON     hIcon;		// Default for this proj
		HCURSOR   hCursor;		// Default for this proj
		HBRUSH    hbrBackground;// We will draw ourselves
		LPCWSTR   lpszMenuName;	// no menu in this proj
		LPCWSTR   lpszClassName;// the name to register the window object
		HICON     hIconSm;		// icon of the project
		} WNDCLASSEXW, *PWNDCLASSEXW, *NPWNDCLASSEXW, *LPWNDCLASSEXW;
	*/
	infoStruct.cbSize = sizeof(infoStruct);
	infoStruct.style = CS_OWNDC;		// means we can costomize all windows devices
	infoStruct.lpfnWndProc = MyWndProc;
	infoStruct.cbClsExtra = 0;
	infoStruct.cbWndExtra = 0;
	infoStruct.hIcon = nullptr;
	infoStruct.hIconSm = nullptr;
	infoStruct.hCursor = nullptr;
	infoStruct.lpszMenuName = nullptr;
	infoStruct.hbrBackground = nullptr;
	// Convert and assign the name
	/* Visual Studio 2017: For anyone having trouble passing pClassName into CreateWindowEx or initializing it with a c string,
	you've got to go to Properties->General (Advanced, if using VS2019) and set Character Set to "Use Multi-Byte Character Set".
	I think the default is "Use Unicode Character Set" and that mucks everything up for some reason*/
	infoStruct.lpszClassName = pClassName.c_str(); // general solution
	return infoStruct;
}

HWND createWinInstance( HINSTANCE &hInstance) {

	// ======================== create window instance ======================= //
	/*
		typedef struct tagCREATESTRUCTA {
		LPVOID    lpCreateParams;	// used to pass para to window func
		HINSTANCE hInstance;
		HMENU     hMenu;
		HWND      hwndParent;		// handle to parent
		int       cy;
		int       cx;
		int       y;
		int       x;
		LONG      style;			// ex: allow drag-and-drop
		LPCSTR    lpszName;
		LPCSTR    lpszClass;
		DWORD     dwExStyle;
		} CREATESTRUCTA, *LPCREATESTRUCTA;
	*/
	std::wstring pstrWname = s2ws(pWindowName); // general solution
	std::wstring pstrCname = s2ws(pClassName);
	return CreateWindowExW(
		0,					// style
		pstrCname.c_str(),	// class name
		pstrWname.c_str(),	// window name
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // window styles DWORD
		initWindowX,
		initWindowY,
		initWindowW,
		initWindowH,
		nullptr,			// handle to parent
		nullptr,			// handle to menu
		hInstance,			// handle to the instance
		nullptr				// costomized params
	);
}


// about message sys
	/*
		BOOL GetMessage(
		LPMSG lpMsg,		// ptr to msg struct that the GetMsg could fill

			typedef struct tagMSG {
			HWND   hwnd;		// A handle to the window whose window procedure receives the message.
								// This member is NULL when the message is a thread message
			UINT   message;		// type of the message
			WPARAM wParam;		// 32 bits msg type dependent info
			LPARAM lParam;		// 32 bits
			DWORD  time;		// msg's post time
			POINT  pt;			// pos of the cursor
			DWORD  lPrivate;	// don't touch it
			} MSG, *PMSG, *NPMSG, *LPMSG;

		HWND  hWnd,			// null -> retrieves any msgs for windows belongs to current thread
		UINT  wMsgFilterMin,// filter msg we want to get
		UINT  wMsgFilterMax // all 0s then all msgs are retieved
		);
		If the function retrieves a message other than WM_QUIT, the return value is nonzero.

		If the function retrieves the WM_QUIT message, the return value is zero.

		If there is an error, the return value is -1.
		For example, the function fails if hWnd is an invalid window handle or lpMsg is an invalid pointer.
		To get extended error information, call GetLastError.
	*/