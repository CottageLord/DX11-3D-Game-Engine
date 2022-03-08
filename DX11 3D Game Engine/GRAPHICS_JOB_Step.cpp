
#include "GRAPHICS_JOB_Step.h"
#include "GRAPHICS_OBJ_Drawable.h"
#include "GRAPHICS_JOB_TechniqueProbe.h"
#include "GRAPHICS_PASS_RenderQueuePass.h"
#include "GRAPHICS_RG_RenderGraph.h"

void Step::Submit(const Drawable& drawable) const
{
	pTargetPass->Accept(Rgph::Job{ this,&drawable });
}

void Step::InitializeParentReferences(const Drawable& parent) noexcept
{
	for (auto& b : bindables)
	{
		b->InitializeParentReference(parent);
	}
}

Step::Step(std::string targetPassName)
	:
	targetPassName{ std::move(targetPassName) }
{}

Step::Step(const Step& src) noexcept
	:
	targetPassName(src.targetPassName)
{
	bindables.reserve(src.bindables.size());
	for (auto& pb : src.bindables)
	{
		if (auto* pCloning = dynamic_cast<const GPipeline::CloningBindable*>(pb.get()))
		{
			bindables.push_back(pCloning->Clone());
		}
		else
		{
			bindables.push_back(pb);
		}
	}
}

void Step::AddBindable(std::shared_ptr<GPipeline::Bindable> bind_in) noexcept
{
	bindables.push_back(std::move(bind_in));
}

void Step::Bind(Graphics& gfx) const noxnd
{
	for (const auto& b : bindables)
	{
		b->Bind(gfx);
	}
}

void Step::Accept(TechniqueProbe& probe)
{
	probe.SetStep(this);
	for (auto& pb : bindables)
	{
		pb->Accept(probe);
	}
}

void Step::Link(Rgph::RenderGraph& rg)
{
	assert(pTargetPass == nullptr);
	pTargetPass = &rg.GetRenderQueue(targetPassName);
}