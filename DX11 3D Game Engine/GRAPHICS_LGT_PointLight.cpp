#include "GRAPHICS_LGT_PointLight.h"
#include "GRAPHICS_OBJ_Camera.h"
#include "SYS_SET_Math.h"
#include "imgui/imgui.h"

PointLight::PointLight(Graphics& gfx, DirectX::XMFLOAT3 pos, float radius)
	:
	mesh(gfx, radius),
	cbuf(gfx, 2)
{
	home = {
		pos,
		{ 0.05f,0.05f,0.05f },	// float3 ambient;
		{ 1.0f,1.0f,1.0f },		// float3 diffuseColor;
		1.0f,					// float diffuseIntensity;
		1.0f,					// float attConst;
		0.045f,					// float attLin;
		0.0075f,				// float attQuad;
	};
	Reset();
	pCamera = std::make_shared<Camera>(gfx, "Light", cbData.pos, 0.0f, PI / 2.0f, true);
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		bool dirtyPos = false;
		const auto d = [&dirtyPos](bool dirty) {dirtyPos = dirtyPos || dirty; };

		ImGui::Text("Position");
		d(ImGui::SliderFloat("X", &cbData.pos.x, -60.0f, 60.0f, "%.1f"));
		d(ImGui::SliderFloat("Y", &cbData.pos.y, -60.0f, 60.0f, "%.1f"));
		d(ImGui::SliderFloat("Z", &cbData.pos.z, -60.0f, 60.0f, "%.1f"));

		if (dirtyPos)
		{
			pCamera->SetPos(cbData.pos);
		}
		// more configurable settings for the light source
		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Intensity", &cbData.diffuseIntensity, 0.01f, 2.0f, "%.2f", 2);
		ImGui::ColorEdit3("Diffuse Color", &cbData.diffuseColor.x);
		ImGui::ColorEdit3("Ambient", &cbData.ambient.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &cbData.attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &cbData.attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &cbData.attQuad, 0.0000001f, 10.0f, "%.7f", 10);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	cbData = home;
}

void PointLight::Submit(size_t channels) const noxnd
{
	mesh.SetPos(cbData.pos);
	mesh.Submit(channels);
}

void PointLight::Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept
{
	auto dataCopy = cbData;
	const auto pos = DirectX::XMLoadFloat3(&cbData.pos);
	// transform the pos by cam matrix, and store back to dataCopy
	DirectX::XMStoreFloat3(&dataCopy.pos, DirectX::XMVector3Transform(pos, view));
	cbuf.Update(gfx, dataCopy);
	cbuf.Bind(gfx);
}

void PointLight::LinkTechniques(Rgph::RenderGraph& rg)
{
	mesh.LinkTechniques(rg);
}

std::shared_ptr<Camera> PointLight::ShareCamera() const noexcept
{
	return pCamera;
}