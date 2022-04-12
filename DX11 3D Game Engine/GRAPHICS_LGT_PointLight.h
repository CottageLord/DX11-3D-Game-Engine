#pragma once
#include "SYS_CLASS_Graphics.h"
#include "GRAPHICS_OBJ_SolidSphere.h"
#include "GRAPHICS_BUF_ConstantBuffers.h"
#include "SYS_SET_ConditionalNoexcept.h"

namespace Rgph
{
	class RenderGraph;
}

class Camera;

class PointLight
{
public:
	PointLight(Graphics& gfx, float radius = 0.5f);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Submit(size_t channels) const noxnd;
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept;
	void LinkTechniques(Rgph::RenderGraph&);
	std::shared_ptr<Camera> ShareCamera() const noexcept;
private:
	struct PointLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};
private:
	PointLightCBuf cbData;
	mutable SolidSphere mesh;
	/// <summary>
	///  Makes the light info visible to the shaders
	/// </summary>
	mutable GPipeline::PixelConstantBuffer<PointLightCBuf> cbuf;
	std::shared_ptr<Camera> pCamera;
};