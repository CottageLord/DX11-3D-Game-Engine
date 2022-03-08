#pragma once
#include "GRAPHICS_PASS_RenderQueuePass.h"
#include "GRAPHICS_OBJ_PixelShader.h"
#include "GRAPHICS_OBJ_VertexShader.h"
#include "GRAPHICS_OBJ_Stencil.h"
#include "GRAPHICS_OBJ_Rasterizer.h"
#include "GRAPHICS_OBJ_RenderTarget.h"
#include "GRAPHICS_OBJ_DepthStencil.h"

#include "GRAPHICS_JOB_Job.h"
#include <vector>

class Graphics;

namespace Rgph
{
	class OutlineDrawingPass : public RenderQueuePass
	{
	public:
		OutlineDrawingPass(Graphics& gfx, std::string name)
			:
			RenderQueuePass(std::move(name))
		{
			using namespace GPipeline;
			RegisterSink(DirectBufferSink<GPipeline::RenderTarget>::Make("renderTarget", renderTarget));
			RegisterSink(DirectBufferSink<GPipeline::DepthStencil>::Make("depthStencil", depthStencil));
			RegisterSource(DirectBufferSource<GPipeline::RenderTarget>::Make("renderTarget", renderTarget));
			RegisterSource(DirectBufferSource<GPipeline::DepthStencil>::Make("depthStencil", depthStencil));
			AddBind(VertexShader::Resolve(gfx, "Solid_VS.cso"));
			AddBind(PixelShader::Resolve(gfx, "Solid_PS.cso"));
			AddBind(Stencil::Resolve(gfx, Stencil::Mode::Mask));
			AddBind(Rasterizer::Resolve(gfx, false));
		}
	};
}