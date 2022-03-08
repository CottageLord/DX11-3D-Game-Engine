#pragma once
#include "GRAPHICS_PASS_FullscreenPass.h"
#include "GRAPHICS_BUF_ConstantBuffersEx.h"

class Graphics;

namespace GPipeline
{
	class PixelShader;
	class RenderTarget;
}

namespace Rgph
{
	class VerticalBlurPass : public FullscreenPass
	{
	public:
		VerticalBlurPass(std::string name, Graphics& gfx);
		void Execute(Graphics& gfx) const noxnd override;
	private:
		std::shared_ptr<GPipeline::CachingPixelConstantBufferEx> direction;
	};
}