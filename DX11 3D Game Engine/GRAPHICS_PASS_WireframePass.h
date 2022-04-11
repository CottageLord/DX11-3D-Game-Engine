#pragma once
#include "Graphics_PASS_RenderQueuePass.h"
#include "GRAPHICS_OBJ_Stencil.h"
#include "GRAPHICS_JOB_Job.h"
#include "GRAPHICS_RG_Sink.h"
#include "GRAPHICS_RG_Source.h"
#include <vector>

class Graphics;

namespace Rgph
{
	class WireframePass : public RenderQueuePass
	{
	public:
		WireframePass(Graphics& gfx, std::string name)
			:
			RenderQueuePass(std::move(name))
		{
			using namespace GPipeline;
			RegisterSink(DirectBufferSink<RenderTarget>::Make("renderTarget", renderTarget));
			RegisterSink(DirectBufferSink<DepthStencil>::Make("depthStencil", depthStencil));
			RegisterSource(DirectBufferSource<RenderTarget>::Make("renderTarget", renderTarget));
			RegisterSource(DirectBufferSource<DepthStencil>::Make("depthStencil", depthStencil));
			AddBind(Stencil::Resolve(gfx, Stencil::Mode::DepthReversed));
		}
	};
}