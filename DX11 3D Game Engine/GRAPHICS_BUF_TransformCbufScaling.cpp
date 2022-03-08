#include "GRAPHICS_BUF_TransformCbufScaling.h"
#include "GRAPHICS_JOB_TechniqueProbe.h"

namespace dx = DirectX;

GPipeline::TransformCbufScaling::TransformCbufScaling(Graphics& gfx, float scale)
	:
	TransformCbuffer(gfx),
	buf(MakeLayout())
{
	buf["scale"] = scale;
}

void GPipeline::TransformCbufScaling::Accept(TechniqueProbe& probe)
{
	probe.VisitBuffer(buf);
}

void GPipeline::TransformCbufScaling::Bind(Graphics& gfx) noxnd
{
	const float scale = buf["scale"];
	const auto scaleMatrix = dx::XMMatrixScaling(scale, scale, scale);
	auto xf = GetTransforms(gfx);
	xf.modelView = xf.modelView * scaleMatrix;
	xf.modelViewProj = xf.modelViewProj * scaleMatrix;
	UpdateBindImpl(gfx, xf);
}

std::unique_ptr<GPipeline::CloningBindable> GPipeline::TransformCbufScaling::Clone() const noexcept
{
	return std::make_unique<TransformCbufScaling>(*this);
}

Dcb::RawLayout GPipeline::TransformCbufScaling::MakeLayout()
{
	Dcb::RawLayout layout;
	layout.Add<Dcb::Float>("scale");
	return layout;
}