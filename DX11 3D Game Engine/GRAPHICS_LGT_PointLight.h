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
	/**
	 * @brief 
	 */
	PointLight(Graphics& gfx, DirectX::XMFLOAT3 pos = { 10.0f,9.0f,2.5f }, float radius = 0.5f);
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
	PointLightCBuf home;
	PointLightCBuf cbData;
	mutable SolidSphere mesh;
	/// Makes the light info visible to the shaders
	mutable GPipeline::PixelConstantBuffer<PointLightCBuf> cbuf;
	/// A camera allowing to generate shadow map for this point light
	std::shared_ptr<Camera> pCamera;
};