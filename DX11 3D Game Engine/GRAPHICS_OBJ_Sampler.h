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
		/**
		* @brief A helper to access BindablePool. This could also be done through BindablePool::Resolve, but
		* is not friendly to IDE IntelliSence (not definite set of params)
		*/
		static std::shared_ptr<Sampler> Resolve(Graphics& gfx);
		static std::string GenerateUID();
		/**
		* @brief Get the UID from a existing Bindable. WIll be useful when searching for Bindable within Drawable
		*/
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};
}