#include "GRAPHICS_PASS_BufferClearPass.h"
#include "GRAPHICS_OBJ_RenderTarget.h"
#include "GRAPHICS_OBJ_DepthStencil.h"
#include "GRAPHICS_RG_Sink.h"
#include "GRAPHICS_RG_Source.h"


namespace Rgph
{
	BufferClearPass::BufferClearPass(std::string name)
		:
		Pass(std::move(name))
	{
		RegisterSink(DirectBufferSink<GPipeline::BufferResource>::Make("buffer", buffer));
		RegisterSource(DirectBufferSource<GPipeline::BufferResource>::Make("buffer", buffer));
	}

	void BufferClearPass::Execute(Graphics& gfx) const noxnd
	{
		buffer->Clear(gfx);
	}
}