#pragma once
#include "GRAPHICS_PASS_RenderQueuePass.h"
#include "GRAPHICS_OBJ_Stencil.h"
#include "GRAPHICS_OBJ_DepthStencil.h"
#include "GRAPHICS_OBJ_Camera.h"
#include "GRAPHICS_OBJ_DepthStencil.h"
#include "GRAPHICS_OBJ_ShadowCameraCBuffer.h"
#include "GRAPHICS_OBJ_ShadowSampler.h"
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
			RenderQueuePass(std::move(name)),
			pShadowCBuf{ std::make_shared<GPipeline::ShadowCameraCBuf>(gfx) }
		{
			using namespace GPipeline;

			AddBind(pShadowCBuf);

			RegisterSink(DirectBufferSink<GPipeline::RenderTarget>::Make("renderTarget", renderTarget));
			RegisterSink(DirectBufferSink<GPipeline::DepthStencil>::Make("depthStencil", depthStencil));

			AddBindSink<GPipeline::Bindable>("shadowMap");

			AddBind(std::make_shared<GPipeline::ShadowSampler>(gfx));

			RegisterSource(DirectBufferSource<GPipeline::RenderTarget>::Make("renderTarget", renderTarget));
			RegisterSource(DirectBufferSource<GPipeline::DepthStencil>::Make("depthStencil", depthStencil));
			AddBind(Stencil::Resolve(gfx, Stencil::Mode::Off));
		}
		void BindMainCamera(const Camera& cam) noexcept
		{
			pMainCamera = &cam;
		}
		void BindShadowCamera(const Camera& cam) noexcept
		{
			pShadowCBuf->SetCamera(&cam);
		}
		void Execute(Graphics& gfx) const noxnd override
		{
			assert(pMainCamera);
			pShadowCBuf->Update(gfx);
			pMainCamera->BindToGraphics(gfx);
			RenderQueuePass::Execute(gfx);
		}
	private:
		std::shared_ptr<GPipeline::ShadowCameraCBuf> pShadowCBuf;
		//std::shared_ptr<GPipeline::Bindable> pShadowMap;
		const Camera* pMainCamera = nullptr;
	};
}