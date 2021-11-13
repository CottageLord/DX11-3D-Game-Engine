#include "GRAPHICS_BUF_TransformCbuffer.h"
namespace GPipeline
{
	TransformCbuffer::TransformCbuffer(Graphics& gfx, const Drawable& target, UINT slot)
		:
		drawTarget(target)
	{
		// if the shared temp space haven't been assigned
		if (!pVcbFinalMatrix)
		{
			pVcbFinalMatrix = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
		}
	}

	void TransformCbuffer::Bind(Graphics& gfx) noexcept
	{
		UpdateBindImpl(gfx, GetTransforms(gfx));
	}

	void TransformCbuffer::UpdateBindImpl(Graphics & gfx, const Transforms & tf) noexcept
	{
		pVcbFinalMatrix->Update(gfx, tf);
		pVcbFinalMatrix->Bind(gfx);
	}

	TransformCbuffer::Transforms TransformCbuffer::GetTransforms(Graphics & gfx) noexcept
	{
		const auto modelView = drawTarget.GetTransformXM() * gfx.GetCamera();
		return {
			DirectX::XMMatrixTranspose(modelView),
			DirectX::XMMatrixTranspose(
				modelView *
				gfx.GetProjection()
			)
		};
	}

	std::unique_ptr<VertexConstantBuffer<TransformCbuffer::Transforms>> TransformCbuffer::pVcbFinalMatrix;
}