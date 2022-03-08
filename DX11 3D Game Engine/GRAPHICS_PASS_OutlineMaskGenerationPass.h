#pragma once
#include "GRAPHICS_PASS_RenderQueuePass.h"
#include "GRAPHICS_OBJ_NullPixelShader.h"
#include "GRAPHICS_OBJ_VertexShader.h"
#include "GRAPHICS_OBJ_Stencil.h"
#include "GRAPHICS_OBJ_Rasterizer.h"
#include "GRAPHICS_OBJ_DepthStencil.h"

#include "GRAPHICS_JOB_Job.h"
#include <vector>

class Graphics;

namespace Rgph
{
	class OutlineMaskGenerationPass : public RenderQueuePass
	{
	public:
		OutlineMaskGenerationPass(Graphics& gfx, std::string name)
			:
			RenderQueuePass(std::move(name))
		{
			using namespace GPipeline;
			RegisterSink(DirectBufferSink<GPipeline::DepthStencil>::Make("depthStencil", depthStencil));
			RegisterSource(DirectBufferSource<GPipeline::DepthStencil>::Make("depthStencil", depthStencil));
			AddBind(VertexShader::Resolve(gfx, "Solid_VS.cso"));
			AddBind(NullPixelShader::Resolve(gfx));
			AddBind(Stencil::Resolve(gfx, Stencil::Mode::Write));
			AddBind(Rasterizer::Resolve(gfx, false));
		}
	};
}