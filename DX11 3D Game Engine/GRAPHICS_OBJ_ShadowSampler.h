#pragma once
#include "GRAPHICS_OBJ_Bindable.h"

namespace GPipeline
{
	class ShadowSampler : public Bindable
	{
	public:
		ShadowSampler(Graphics& gfx);
		void Bind(Graphics& gfx) noxnd override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};
}