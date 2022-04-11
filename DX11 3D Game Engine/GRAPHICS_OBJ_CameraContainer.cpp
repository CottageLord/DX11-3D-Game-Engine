#include "GRAPHICS_OBJ_CameraContainer.h"
#include "GRAPHICS_OBJ_Camera.h"
#include "GRAPHICS_RG_RenderGraph.h"
#include "SYS_CLASS_Graphics.h"
#include "imgui/imgui.h"

void CameraContainer::SpawnWindow(Graphics& gfx)
{
	if (ImGui::Begin("Cameras"))
	{
		if (ImGui::BeginCombo("Active Camera", (*this)->GetName().c_str()))
		{
			for (int i = 0; i < std::size(cameras); i++)
			{
				const bool isSelected = i == active;
				if (ImGui::Selectable(cameras[i]->GetName().c_str(), isSelected))
				{
					active = i;
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("Controlled Camera", GetControlledCamera().GetName().c_str()))
		{
			for (int i = 0; i < std::size(cameras); i++)
			{
				const bool isSelected = i == controlled;
				if (ImGui::Selectable(cameras[i]->GetName().c_str(), isSelected))
				{
					controlled = i;
				}
			}
			ImGui::EndCombo();
		}

		GetControlledCamera().SpawnControlWidgets(gfx);
	}
	ImGui::End();
}

void CameraContainer::Bind(Graphics& gfx)
{
	gfx.SetCamera((*this)->GetMatrix());
}

void CameraContainer::AddCamera(std::unique_ptr<Camera> pCam)
{
	cameras.push_back(std::move(pCam));
}

Camera* CameraContainer::operator->()
{
	return cameras[active].get();
}

CameraContainer::~CameraContainer()
{}

void CameraContainer::LinkTechniques(Rgph::RenderGraph& rg)
{
	for (auto& pcam : cameras)
	{
		pcam->LinkTechniques(rg);
	}
}

void CameraContainer::Submit() const
{
	for (size_t i = 0; i < cameras.size(); i++)
	{
		if (i != active)
		{
			cameras[i]->Submit();
		}
	}
}

Camera& CameraContainer::GetControlledCamera()
{
	return *cameras[controlled];
}