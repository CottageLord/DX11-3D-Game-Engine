#pragma once
#include "GRAPHICS_BUF_ConstantBuffers.h"
#include "GRAPHICS_OBJ_Drawable.h"
#include <DirectXMath.h>

namespace GPipeline
{
	class TransformCbuffer : public CloningBindable
	{
	protected:
		struct Transforms
		{
			DirectX::XMMATRIX model;
			DirectX::XMMATRIX modelView;
			DirectX::XMMATRIX modelViewProj;
		};
	public:
		TransformCbuffer(Graphics& gfx, UINT slot = 0u);
		/**
		* @brief update vs shader with the new transform matrix
		*/
		void Bind(Graphics& gfx) noxnd override;
		void InitializeParentReference(const Drawable& drawTarget) noexcept override;
		std::unique_ptr<CloningBindable> Clone() const noexcept override;
	protected:
		void UpdateBindImpl(Graphics& gfx, const Transforms& tf) noxnd;
		Transforms GetTransforms(Graphics& gfx) noxnd;
	private:
		/// <summary>
		/// A static constant buffer that will be reused across all primitives to store 
		/// their moving result (matrix) during each's binding stage
		/// </summary>
		static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbFinalMatrix;
		//const Drawable& drawTarget;
		const Drawable* pDrawTarget = nullptr;
	};
}