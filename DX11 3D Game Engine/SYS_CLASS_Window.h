#pragma once
#include "SYS_SET_Config.h"
#include "SYS_CLASS_MFException.h"
#include "SYS_CLASS_IO_Keyboard.h"
#include "SYS_CLASS_IO_Mouse.h"
#include "SYS_CLASS_Graphics.h"
#include <optional>
#include <memory>


class Window
{
// inner classes
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
// methods
public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);
	/**
	* @brief enable the cursor using winApi
	*/
	void EnableCursor() noexcept;
	/**
	* @brief disable the cursor using winApi
	*/
	void DisableCursor() noexcept;
	bool CursorEnabled() const noexcept;
	// static function that processes msgs for all windows
	static std::optional<int> ProcessMessages() noexcept; // optinally returns
	Graphics& Gfx();
private:
	/**
	* @brief hide the cursor using winApi
	*/
	void HideCursor() noexcept;
	/**
	* @brief show the cursor using winApi
	*/
	void ShowCursor() noexcept;
	/**
	* @brief enable the cursor interaction with UI elements
	*/
	void EnableImGuiMouse() noexcept;
	/**
	* @brief disable the cursor interaction completely
	*/
	void DisableImGuiMouse() noexcept;
	/**
	* @brief confine the cursor to a rectangle window area
	*/
	void ConfineCursor() noexcept;
	/**
	* @brief free the cursor from window rectangle area
	*/
	void FreeCursor() noexcept;

	// we can register those func as windows procedure
	// deliver the pointer access info to the win32 api side
	/**
	* @brief build connection between application and WinAPI message system
	*/
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	// connect to the real runtime handler
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Keyboard kbd; // public embeded object
	Mouse mouse; 
private:
	bool cursorEnabled = true;
	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx; // deletes instandly when window destructs
	std::vector<BYTE> rawBuffer;
	std::string commandLine;
};