#pragma once
#include "GRAPHICS_OBJ_Drawable.h"

class SolidSphere : public Drawable
{
public:
	SolidSphere(Graphics& gfx, float radius);
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
};