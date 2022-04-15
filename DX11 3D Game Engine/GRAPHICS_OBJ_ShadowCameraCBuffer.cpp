#include "GRAPHICS_OBJ_ShadowCameraCBuffer.h"
#include "GRAPHICS_OBJ_Camera.h"

namespace dx = DirectX;

namespace GPipeline
{
	ShadowCameraCBuf::ShadowCameraCBuf(Graphics& gfx, UINT slot)
		:
		pVcbuf{ std::make_unique<VertexConstantBuffer<Transform>>(gfx,slot) }
	{}
	void ShadowCameraCBuf::Bind(Graphics& gfx) noxnd
	{
		pVcbuf->Bind(gfx);
	}
	void ShadowCameraCBuf::Update(Graphics& gfx)
	{
		const Transform t{
			dx::XMMatrixTranspose(
				pCamera->GetMatrix() * pCamera->GetProjection()
			)
		};
		pVcbuf->Update(gfx, t);
	}
	void ShadowCameraCBuf::SetCamera(const Camera* p) noexcept
	{
		pCamera = p;
	}
}