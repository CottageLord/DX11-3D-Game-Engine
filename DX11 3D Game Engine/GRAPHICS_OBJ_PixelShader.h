#pragma once
#include "GRAPHICS_OBJ_Bindable.h"
#include "SYS_SET_GraphicsThrowMacros.h"

class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& gfx, const std::wstring& path);
	void Bind(Graphics& gfx) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};