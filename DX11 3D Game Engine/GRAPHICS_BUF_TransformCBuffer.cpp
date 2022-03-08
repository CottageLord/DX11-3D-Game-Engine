#include "GRAPHICS_BUF_TransformCbuffer.h"
namespace GPipeline
{
	TransformCbuffer::TransformCbuffer(Graphics& gfx, UINT slot)
	{
		// if the shared temp space haven't been assigned
		if (!pVcbFinalMatrix)
		{
			pVcbFinalMatrix = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
		}
	}

	void TransformCbuffer::Bind(Graphics& gfx) noxnd
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(UpdateBindImpl(gfx, GetTransforms(gfx)));
	}

	void TransformCbuffer::InitializeParentReference(const Drawable& drawTarget) noexcept
	{
		pDrawTarget = &drawTarget;
	}

	std::unique_ptr<CloningBindable> TransformCbuffer::Clone() const noexcept
	{
		return std::make_unique<TransformCbuffer>(*this);
	}


	void TransformCbuffer::UpdateBindImpl(Graphics & gfx, const Transforms & tf) noxnd
	{
		assert(pDrawTarget != nullptr);
		pVcbFinalMatrix->Update(gfx, tf);
		pVcbFinalMatrix->Bind(gfx);
	}

	TransformCbuffer::Transforms TransformCbuffer::GetTransforms(Graphics & gfx) noxnd
	{
		assert(pDrawTarget != nullptr);
		const auto modelView = pDrawTarget->GetTransformXM() * gfx.GetCamera();
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