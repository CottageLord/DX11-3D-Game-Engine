#pragma once
#include "GRAPHICS_OBJ_Drawable.h"
#include "GRAPHICS_OBJ_Bindable.h"
#include "GRAPHICS_BUF_IndexBuffer.h"

class TestCube : public Drawable
{
public:
	TestCube(Graphics& gfx, float size);
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	void SetRotation(float roll, float pitch, float yaw) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SpawnControlWindow(Graphics& gfx, const char* name) noexcept;
	void DrawOutline(Graphics& gfx) noxnd
	{
		outlining = true;
		for (auto& b : outlineEffect)
		{
			b->Bind(gfx);
		}
		gfx.DrawIndexed(QueryBindable<GPipeline::IndexBuffer>()->GetCount());
		outlining = false;
	}
private:
	std::vector<std::shared_ptr<GPipeline::Bindable>> outlineEffect;
	struct PSMaterialConstant
	{
		float specularIntensity = 0.1f;
		float specularPower = 20.0f;
		BOOL normalMappingEnabled = TRUE;
		float padding[1];
	} pmc;
	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	bool outlining = false;
};