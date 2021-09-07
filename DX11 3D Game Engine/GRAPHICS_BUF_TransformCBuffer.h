#pragma once
#include "GRAPHICS_BUF_ConstantBuffers.h"
#include "GRAPHICS_OBJ_Drawable.h"
#include <DirectXMath.h>

class TransformCbuffer : public Bindable
{
private:
	struct Transforms
	{
		DirectX::XMMATRIX modelViewProj;
		DirectX::XMMATRIX model;
	};
public:
	TransformCbuffer(Graphics& gfx, const Drawable& target);
	void Bind(Graphics& gfx) noexcept override;
private:
	/// <summary>
	/// A static constant buffer that will be reused across all primitives to store 
	/// their moving result (matrix) during each's binding stage
	/// </summary>
	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbFinalMatrix;
	const Drawable& drawTarget;
};