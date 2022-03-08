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
	class HorizontalBlurPass : public FullscreenPass
	{
	public:
		HorizontalBlurPass(std::string name, Graphics& gfx, unsigned int fullWidth, unsigned int fullHeight);
		void Execute(Graphics& gfx) const noxnd override;
	private:
		std::shared_ptr<GPipeline::CachingPixelConstantBufferEx> direction;
	};
}