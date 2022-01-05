#pragma once
#include "SYS_SET_FilterWinH.h"
#include "SYS_SET_ConditionalNoexcept.h"
#include "SYS_CLASS_DXgiInfoManager.h"
#include "SYS_CLASS_MFException.h"


#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include <memory>
#include <random>
#include <d3dcompiler.h>
#include <DirectXMath.h>

class DepthStencil;

namespace GPipeline
{
	class Bindable;
}

class Graphics
{
	//friend class GPipeline::Bindable;
	friend class GraphicsResource;
public:
	class Exception : public MFException
	{
		using MFException::MFException;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;	// make sure we have hr reference so we can accurately locate error
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
public:
	Graphics(HWND hWnd, int width, int height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	// draw what we've got in this frame
	void EndFrame();
	void BeginFrame(float red, float green, float blue) noexcept;
	void BindSwapBuffer() noexcept;
	void BindSwapBuffer(const DepthStencil& ds) noexcept;
	void DrawIndexed(UINT count) noxnd;
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void SetCamera(DirectX::FXMMATRIX cam) noexcept;
	DirectX::XMMATRIX GetCamera() const noexcept;
	void EnableImgui() noexcept;
	void DisableImgui() noexcept;
	bool IsImguiEnabled() const noexcept;
	/**
	 * @brief Returns the width of the swap chain
	 */
	UINT GetWidth() const noexcept;
	UINT GetHeight() const noexcept;
private:
	UINT width;
	UINT height;
	DirectX::XMMATRIX camera;
	DirectX::XMMATRIX projection;
	bool imguiEnabled = true;
#ifndef NDEBUG
	DxgiInfoManager infoManager; // won't bein build/release mode
#endif
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
};