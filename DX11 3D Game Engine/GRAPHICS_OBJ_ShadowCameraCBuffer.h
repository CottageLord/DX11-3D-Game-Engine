#pragma once
#include "GRAPHICS_OBJ_Bindable.h"
#include "GRAPHICS_BUF_ConstantBuffers.h"

class Camera;

namespace GPipeline
{
	class ShadowCameraCBuf : public Bindable
	{
	protected:
		struct Transform
		{
			DirectX::XMMATRIX ViewProj;
		};
	public:
		ShadowCameraCBuf(Graphics& gfx, UINT slot = 1u);
		void Bind(Graphics& gfx) noxnd override;
		void Update(Graphics& gfx);
		void SetCamera(const Camera* pCamera) noexcept;
	private:
		std::unique_ptr<VertexConstantBuffer<Transform>> pVcbuf;
		const Camera* pCamera = nullptr;
	};
}