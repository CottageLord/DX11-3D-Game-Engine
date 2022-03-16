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
		/**
		  * @brief Spawn a UI control for Gauss constants
		  */
		void RenderWidgets(Graphics& gfx);
	private:
		// private functions
		void SetKernelGauss(int radius, float sigma) noxnd;
		void SetKernelBox(int radius) noxnd;
		// private data
		enum class KernelType
		{
			Gauss,
			Box,
		} kernelType = KernelType::Gauss;
		static constexpr int maxRadius = 7;
		int radius = 4;
		float sigma = 2.0f;
		std::shared_ptr<GPipeline::CachingPixelConstantBufferEx> blurKernel;
		std::shared_ptr<GPipeline::CachingPixelConstantBufferEx> blurDirection;
	};
}
