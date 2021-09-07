#pragma once
#include "SYS_CLASS_Graphics.h"
#include "GRAPHICS_OBJ_SolidSphere.h"
#include "GRAPHICS_BUF_ConstantBuffers.h"

class PointLight
{
public:
	PointLight(Graphics& gfx, float radius = 0.5f);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Draw(Graphics& gfx) const noexcept(!IS_DEBUG);
	void Bind(Graphics& gfx) const noexcept;
private:
	struct PointLightCBuf
	{
		DirectX::XMFLOAT3 pos;
		float padding;
	};
private:
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };;
	mutable SolidSphere mesh;
	/// <summary>
	///  Makes the light info visible to the shaders
	/// </summary>
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};