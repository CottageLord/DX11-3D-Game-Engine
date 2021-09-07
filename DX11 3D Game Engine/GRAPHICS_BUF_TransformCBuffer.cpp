#include "GRAPHICS_BUF_TransformCbuffer.h"

TransformCbuffer::TransformCbuffer(Graphics& gfx, const Drawable& target)
	:
	drawTarget(target)
{
	// if the shared temp space haven't been assigned
	if (!pVcbFinalMatrix)
	{
		pVcbFinalMatrix = std::make_unique<VertexConstantBuffer<Transforms>>(gfx);
	}
}

void TransformCbuffer::Bind(Graphics& gfx) noexcept
{
	const auto model = drawTarget.GetTransformXM();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(model),
		DirectX::XMMatrixTranspose(
			model *
			gfx.GetCamera() *
			gfx.GetProjection()
		)
	};
	pVcbFinalMatrix->Update(gfx, tf);
	pVcbFinalMatrix->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformCbuffer::Transforms>> TransformCbuffer::pVcbFinalMatrix;