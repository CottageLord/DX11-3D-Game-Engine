#pragma once
#include "GRAPHICS_PASS_RenderQueuePass.h"
#include "GRAPHICS_OBJ_Stencil.h"
#include "GRAPHICS_OBJ_DepthStencil.h"
#include "GRAPHICS_JOB_Job.h"
#include "GRAPHICS_RG_Sink.h"
#include "GRAPHICS_RG_Source.h"

#include <vector>
class Graphics;

namespace Rgph
{
	class LambertianPass : public RenderQueuePass
	{
	public:
		LambertianPass(Graphics& gfx, std::string name)
			:
			RenderQueuePass(std::move(name))
		{
			using namespace GPipeline;
			RegisterSink(DirectBufferSink<GPipeline::RenderTarget>::Make("renderTarget", renderTarget));
			RegisterSink(DirectBufferSink<GPipeline::DepthStencil>::Make("depthStencil", depthStencil));
			RegisterSource(DirectBufferSource<GPipeline::RenderTarget>::Make("renderTarget", renderTarget));
			RegisterSource(DirectBufferSource<GPipeline::DepthStencil>::Make("depthStencil", depthStencil));
			AddBind(Stencil::Resolve(gfx, Stencil::Mode::Off));
		}
	};
}