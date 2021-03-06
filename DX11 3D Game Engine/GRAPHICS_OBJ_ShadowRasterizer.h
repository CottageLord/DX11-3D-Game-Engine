#pragma once
#include "GRAPHICS_OBJ_Bindable.h"
#include <array>

namespace GPipeline
{
	/**
	 * @brief Configures settings for slope scaled shadowing
	 */
	class ShadowRasterizer : public Bindable
	{
	public:
		ShadowRasterizer(Graphics& gfx, int depthBias, float slopeBias, float clamp);
		void ChangeDepthBiasParameters(Graphics& gfx, int depthBias, float slopeBias, float clamp);
		void Bind(Graphics& gfx) noxnd override;
		int GetDepthBias() const;
		float GetSlopeBias() const;
		float GetClamp() const;
	protected: 
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;
		int depthBias;
		float slopeBias;
		float clamp;
	};
}