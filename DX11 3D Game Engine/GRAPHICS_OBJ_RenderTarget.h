#pragma once
#include "SYS_CLASS_Graphics.h"
#include "GRAPHICS_OBJ_GraphicsResource.h"

class DepthStencil;

class RenderTarget : public GraphicsResource
{
public:
	RenderTarget(Graphics& gfx, UINT width, UINT height);
	void BindAsTexture(Graphics& gfx, UINT slot) const noexcept;
	/**
	 * @brief Bind the output render target without configuring the depth stencil
	 */
	void BindAsTarget(Graphics& gfx) const noexcept;
	/**
	 * @brief Bind the output render target with the depth stencil
	 */
	void BindAsTarget(Graphics& gfx, const DepthStencil& depthStencil) const noexcept;
private:
	// input view
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	// output view
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;
};