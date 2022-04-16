#pragma once
#include "GRAPHICS_PASS_RenderQueuePass.h"
#include "GRAPHICS_OBJ_PixelShader.h"
#include "GRAPHICS_OBJ_VertexShader.h"
#include "GRAPHICS_OBJ_Stencil.h"
#include "GRAPHICS_OBJ_Rasterizer.h"
#include "GRAPHICS_OBJ_RenderTarget.h"
#include "GRAPHICS_OBJ_Blender.h"
#include "GRAPHICS_OBJ_NullPixelShader.h"
#include "GRAPHICS_OBJ_ShadowRasterizer.h"
#include "GRAPHICS_JOB_Job.h"
#include "GRAPHICS_RG_Source.h"
#include <vector>

class Graphics;

namespace Rgph
{
	class ShadowMappingPass : public RenderQueuePass
	{
	public:
		void BindShadowCamera(const Camera& cam) noexcept
		{
			pShadowCamera = &cam;
		}
		ShadowMappingPass(Graphics& gfx, std::string name)
			:
			RenderQueuePass(std::move(name))
		{
			using namespace GPipeline;
			depthStencil = std::make_unique<ShaderInputDepthStencil>(gfx, 3, DepthStencil::Usage::ShadowDepth);
			AddBind(VertexShader::Resolve(gfx, "Solid_VS.cso"));
			AddBind(NullPixelShader::Resolve(gfx));
			AddBind(Stencil::Resolve(gfx, Stencil::Mode::Off));
			AddBind(Blender::Resolve(gfx, false));
			AddBind(std::make_shared<GPipeline::ShadowRasterizer>(gfx, 50, 2.0f, 0.1f));
			RegisterSource(DirectBindableSource<GPipeline::DepthStencil>::Make("map", depthStencil));
		}
		void Execute(Graphics& gfx) const noxnd override
		{
			depthStencil->Clear(gfx);
			pShadowCamera->BindToGraphics(gfx);
			RenderQueuePass::Execute(gfx);
		}
		void DumpShadowMap(Graphics& gfx, const std::string& path) const
		{
			depthStencil->ToSurface(gfx).Save(path);
		}

		void Reset() noxnd override
		{
			RenderQueuePass::Reset();
		}
	private:
		const Camera* pShadowCamera = nullptr;
	};
}