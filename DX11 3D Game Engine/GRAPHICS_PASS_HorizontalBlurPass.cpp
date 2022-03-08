#include "GRAPHICS_PASS_HorizontalBlurPass.h"
#include "GRAPHICS_OBJ_PixelShader.h"
#include "GRAPHICS_OBJ_RenderTarget.h"
#include "GRAPHICS_OBJ_Blender.h"
#include "GRAPHICS_OBJ_Sampler.h"
#include "GRAPHICS_RG_Sink.h"
#include "GRAPHICS_RG_Source.h"
#include "GRAPHICS_BUF_ConstantBuffersEx.h"

using namespace GPipeline;

namespace Rgph
{
	HorizontalBlurPass::HorizontalBlurPass(std::string name, Graphics& gfx, unsigned int fullWidth, unsigned int fullHeight)
		:
		FullscreenPass(std::move(name), gfx)
	{
		AddBind(PixelShader::Resolve(gfx, "BlurOutline_PS.cso"));
		AddBind(Blender::Resolve(gfx, false));
		AddBind(Sampler::Resolve(gfx, Sampler::Type::Point, true));

		AddBindSink<GPipeline::RenderTarget>("scratchIn");
		AddBindSink<GPipeline::CachingPixelConstantBufferEx>("kernel");
		RegisterSink(DirectBindableSink<CachingPixelConstantBufferEx>::Make("direction", direction));

		// the renderTarget is internally sourced and then exported as a Bindable
		renderTarget = std::make_shared<GPipeline::ShaderInputRenderTarget>(gfx, fullWidth / 2, fullHeight / 2, 0u);
		RegisterSource(DirectBindableSource<RenderTarget>::Make("scratchOut", renderTarget));
	}

	// this override is necessary because we cannot (yet) link input bindables directly into
	// the container of bindables (mainly because vector growth buggers references)
	void HorizontalBlurPass::Execute(Graphics& gfx) const noxnd
	{
		auto buf = direction->GetBuffer();
		buf["isHorizontal"] = true;
		direction->SetBuffer(buf);

		direction->Bind(gfx);
		FullscreenPass::Execute(gfx);
	}
}