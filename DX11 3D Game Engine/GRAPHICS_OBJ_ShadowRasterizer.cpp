#include "GRAPHICS_OBJ_ShadowRasterizer.h"
#include "SYS_SET_GraphicsThrowMacros.h"
#include "GRAPHICS_OBJ_BindablePool.h"

namespace GPipeline
{
	ShadowRasterizer::ShadowRasterizer(Graphics& gfx, int depthBias, float slopeBias, float clamp)
	{
		ChangeDepthBiasParameters(gfx, depthBias, slopeBias, clamp);
	}

	void ShadowRasterizer::ChangeDepthBiasParameters(Graphics& gfx, int depthBias, float slopeBias, float clamp)
	{
		this->depthBias = depthBias;
		this->slopeBias = slopeBias;
		this->clamp = clamp;

		GET_INFO_MAN(gfx);

		D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		rasterDesc.DepthBias = depthBias;			// A constant depth bias applied to every pixel
		rasterDesc.SlopeScaledDepthBias = slopeBias;// Amount of bias to multiply with the slope and add back to the DepthBias
													// The larger the splore the larger the value
		rasterDesc.DepthBiasClamp = clamp;			// When the slope goes crazy, set this as the boundary

		GFX_THROW_INFO(GetDevice(gfx)->CreateRasterizerState(&rasterDesc, &pRasterizer));
	}

	int ShadowRasterizer::GetDepthBias() const
	{
		return depthBias;
	}

	float ShadowRasterizer::GetSlopeBias() const
	{
		return slopeBias;
	}

	float ShadowRasterizer::GetClamp() const
	{
		return clamp;
	}

	void ShadowRasterizer::Bind(Graphics& gfx) noxnd
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->RSSetState(pRasterizer.Get()));
	}
}