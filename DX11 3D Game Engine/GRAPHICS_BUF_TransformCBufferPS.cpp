#include "GRAPHICS_BUF_TransformCbufferPS.h"

namespace GPipeline
{
	TransformCbufferPS::TransformCbufferPS(Graphics& gfx, const Drawable& parent, UINT slotV, UINT slotP)
		:
		TransformCbuffer(gfx, parent, slotV)
	{
		if (!pPcbFinalMatrix)
		{
			pPcbFinalMatrix = std::make_unique<PixelConstantBuffer<Transforms>>(gfx, slotP);
		}
	}

	void GPipeline::TransformCbufferPS::Bind(Graphics& gfx) noexcept
	{
		const auto tf = GetTransforms(gfx);
		TransformCbuffer::UpdateBindImpl(gfx, tf);
		UpdateBindImpl(gfx, tf);
	}

	void TransformCbufferPS::UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept
	{
		pPcbFinalMatrix->Update(gfx, tf);
		pPcbFinalMatrix->Bind(gfx);
	}

	std::unique_ptr<PixelConstantBuffer<TransformCbuffer::Transforms>> TransformCbufferPS::pPcbFinalMatrix;
}