/**
 * @file GRAPHICS_OBJ_Sampler.h
 * @brief Instructs the pipeline how it should perform its lookups into the texture
 */
#pragma once
#include "GRAPHICS_OBJ_Bindable.h"

namespace GPipeline
{
	class Sampler : public Bindable
	{
	public:
		Sampler(Graphics& gfx);
		void Bind(Graphics& gfx) noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};
}