#include "GRAPHICS_BUF_TransformCbuffer.h"

TransformCbuffer::TransformCbuffer(Graphics& gfx, const Drawable& target)
	:
	drawTarget(target)
{
	// if the shared temp space haven't been assigned
	if (!pVcbFinalMatrix)
	{
		pVcbFinalMatrix = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void TransformCbuffer::Bind(Graphics& gfx) noexcept
{
	pVcbFinalMatrix->Update(gfx,
		DirectX::XMMatrixTranspose(
			drawTarget.GetTransformXM() * gfx.GetProjection()
		)
	);
	pVcbFinalMatrix->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCbuffer::pVcbFinalMatrix;