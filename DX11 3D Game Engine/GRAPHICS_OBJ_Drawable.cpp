#include "GRAPHICS_OBJ_Drawable.h"
#include "SYS_SET_GraphicsThrowMacros.h"
#include "GRAPHICS_SET_BindableCommon.h"
#include "GRAPHICS_OBJ_BindablePool.h"

using namespace GPipeline;

/**
* @brief Binds all info to the gpu pipeline and excute drawIndexed()
*/
void Drawable::Submit(FrameCommander& frame) const noexcept
{
	for (const auto& tech : techniques)
	{
		tech.Submit(frame, *this);
	}
}

void Drawable::AddTechnique(Technique tech_in) noexcept
{
	tech_in.InitializeParentReferences(*this);
	techniques.push_back(std::move(tech_in));
}

void Drawable::Bind(Graphics & gfx) const noexcept
{
	pTopology->Bind(gfx);
	pIndices->Bind(gfx);
	pVertices->Bind(gfx);
}

UINT Drawable::GetIndexCount() const noxnd
{
	return pIndices->GetCount();
}

Drawable::~Drawable()
{}