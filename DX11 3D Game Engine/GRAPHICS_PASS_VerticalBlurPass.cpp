
#include "GRAPHICS_PASS_VerticalBlurPass.h"
#include "GRAPHICS_RG_Sink.h"
#include "GRAPHICS_RG_Source.h"
#include "GRAPHICS_OBJ_PixelShader.h"
#include "GRAPHICS_OBJ_Blender.h"
#include "GRAPHICS_OBJ_Stencil.h"
#include "GRAPHICS_OBJ_Sampler.h"

using namespace GPipeline;

namespace Rgph
{
	VerticalBlurPass::VerticalBlurPass(std::string name, Graphics& gfx)
		:
		FullscreenPass(std::move(name), gfx)
	{
		AddBind(PixelShader::Resolve(gfx, "BlurOutline_PS.cso"));
		AddBind(Blender::Resolve(gfx, true));
		AddBind(Stencil::Resolve(gfx, Stencil::Mode::Mask));
		AddBind(Sampler::Resolve(gfx, Sampler::Type::Bilinear, true));

		AddBindSink<RenderTarget>("scratchIn");
		AddBindSink<CachingPixelConstantBufferEx>("kernel");
		RegisterSink(DirectBindableSink<CachingPixelConstantBufferEx>::Make("direction", direction));
		RegisterSink(DirectBufferSink<RenderTarget>::Make("renderTarget", renderTarget));
		RegisterSink(DirectBufferSink<DepthStencil>::Make("depthStencil", depthStencil));

		RegisterSource(DirectBufferSource<RenderTarget>::Make("renderTarget", renderTarget));
		RegisterSource(DirectBufferSource<DepthStencil>::Make("depthStencil", depthStencil));
	}

	// see the note on HorizontalBlurPass::Execute
	void VerticalBlurPass::Execute(Graphics& gfx) const noxnd
	{
		auto buf = direction->GetBuffer();
		buf["isHorizontal"] = false;
		direction->SetBuffer(buf);

		direction->Bind(gfx);
		FullscreenPass::Execute(gfx);
	}
}