#pragma once
#include "GRAPHICS_PASS_RenderQueuePass.h"
#include "GRAPHICS_OBJ_PixelShader.h"
#include "GRAPHICS_OBJ_VertexShader.h"
#include "GRAPHICS_OBJ_Stencil.h"
#include "GRAPHICS_OBJ_Rasterizer.h"
#include "GRAPHICS_OBJ_RenderTarget.h"
#include "GRAPHICS_OBJ_Blender.h"

#include "GRAPHICS_RG_Source.h"
#include "GRAPHICS_JOB_Job.h"
#include <vector>

class Graphics;

namespace Rgph
{
	class BlurOutlineDrawingPass : public RenderQueuePass
	{
	public:
		BlurOutlineDrawingPass(Graphics& gfx, std::string name, unsigned int fullWidth, unsigned int fullHeight)
			:
			RenderQueuePass(std::move(name))
		{
			using namespace GPipeline;
			renderTarget = std::make_unique<ShaderInputRenderTarget>(gfx, fullWidth / 2, fullHeight / 2, 0);
			AddBind(VertexShader::Resolve(gfx, "Solid_VS.cso"));
			AddBind(PixelShader::Resolve(gfx, "Solid_PS.cso"));
			AddBind(Stencil::Resolve(gfx, Stencil::Mode::Mask));
			AddBind(Blender::Resolve(gfx, false));
			RegisterSource(DirectBindableSource<GPipeline::RenderTarget>::Make("scratchOut", renderTarget));
		}
		void Execute(Graphics& gfx) const noxnd override
		{
			renderTarget->Clear(gfx);
			RenderQueuePass::Execute(gfx);
		}
	};
}