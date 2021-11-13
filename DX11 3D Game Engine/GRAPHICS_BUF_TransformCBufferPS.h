#pragma once
#include "GRAPHICS_BUF_TransformCbuffer.h"

namespace GPipeline
{
	class TransformCbufferPS : public TransformCbuffer
	{
	public:
		TransformCbufferPS(Graphics& gfx, const Drawable& parent, UINT slotV = 0u, UINT slotP = 0u);
		/**
		* @brief update vs AND ps shader with the new transform matrix
		*/
		void Bind(Graphics& gfx) noexcept override;
	protected:
		void UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept;
	private:
		static std::unique_ptr<PixelConstantBuffer<Transforms>> pPcbFinalMatrix;
	};
}