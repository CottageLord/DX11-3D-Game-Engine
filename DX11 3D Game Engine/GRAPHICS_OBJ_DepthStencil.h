#pragma once
#include "SYS_CLASS_Graphics.h"
#include "GRAPHICS_OBJ_GraphicsResource.h"


class DepthStencil : public GraphicsResource
{
	friend class RenderTarget;
	friend class Graphics;
public:
	DepthStencil(Graphics& gfx, UINT width, UINT height);
	void BindAsDepthStencil(Graphics& gfx) const noexcept;
	void Clear(Graphics& gfx) const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
};