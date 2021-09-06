#include "GRAPHICS_OBJ_Sampler.h"
#include "SYS_SET_GraphicsThrowMacros.h"

Sampler::Sampler(Graphics& gfx)
{
	GET_INFO_MAN(gfx);

	D3D11_SAMPLER_DESC samplerDesc = {};
	// set filter mode
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
									// |___rendered texture pixel is smaller than its native size
									//     |___larger
									//         |___the rendered is doing mipmaping
									//             |___in this case all 3 are doing linear (smooth blending between pixels)
									//                 use POINT for pixel-art like effect
	// set wrapping mode
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler));
}

void Sampler::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
								// |__Bound sampler to pipeline register(slot 0), 
								// can be accessed with declaring SamplerState in ps
}