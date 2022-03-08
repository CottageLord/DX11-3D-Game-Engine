#pragma once
#include "GRAPHICS_RG_RenderGraph.h"
#include "GRAPHICS_BUF_ConstantBuffersEx.h"
#include <memory>

class Graphics;
namespace GPipeline
{
	class Bindable;
	class RenderTarget;
}

namespace Rgph
{
	class BlurOutlineRenderGraph : public RenderGraph
	{
	public:
		BlurOutlineRenderGraph(Graphics& gfx);
	private:
		// private functions
		void SetKernelGauss(int radius, float sigma) noxnd;
		// private data
		static constexpr int maxRadius = 7;
		static constexpr int radius = 4;
		static constexpr float sigma = 2.0f;
		std::shared_ptr<GPipeline::CachingPixelConstantBufferEx> blurKernel;
		std::shared_ptr<GPipeline::CachingPixelConstantBufferEx> blurDirection;
	};
}
