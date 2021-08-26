#pragma once
#include "SYS_SET_Config.h"
#include "SYS_CLASS_MFException.h"
#include "SYS_CLASS_IO_Keyboard.h"
#include "SYS_CLASS_IO_Mouse.h"
#include "SYS_CLASS_Graphics.h"
#include "resource.h"
#include <optional>
#include <memory>

// error exception helper macro
#define MFWND_EXCEPT( hr ) Window::HrException( __LINE__,__FILE__,(hr) )
#define MFWND_LAST_EXCEPT() Window::HrException( __LINE__,__FILE__,GetLastError() )
#define MFWND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ ) 

class Window
{
public:
	class Exception : public MFException
	{
		using MFException::MFException;
	public:
		// explain the given code
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	private:
		HRESULT hr;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};
private:
	// As we only need one window
	// singleton manages registration/cleanup of window class
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "Direct3D Engine Window";
		static WindowClass wndClass; // construct when program starts
		HINSTANCE hInst;
	};
public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);
	// static function that processes msgs for all windows
	static std::optional<int> ProcessMessages() noexcept; // optinally returns
	Graphics& Gfx();
private:
	// we can register those func as windows procedure
	// setup the pointer access info to the win32 api side
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	// connect to the real runtime handler
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Keyboard kbd; // public embeded object
	Mouse mouse; 
private:
	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx; // deletes instandly when window destructs
};