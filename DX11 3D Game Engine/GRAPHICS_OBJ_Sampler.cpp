#include "GRAPHICS_OBJ_Sampler.h"
#include "SYS_SET_GraphicsThrowMacros.h"
#include "GRAPHICS_OBJ_BindablePool.h"

namespace GPipeline
{
	Sampler::Sampler(Graphics& gfx)
	{
		GET_INFO_MAN(gfx);

		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };
		// set filter mode
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		// |___rendered texture pixel is smaller than its native size
		//     |___larger
		//         |___the rendered is doing mipmaping
		//             |___in this case all 3 are doing linear (smooth blending between pixels)
		//                 use POINT for pixel-art like effect

		// set wrapping mode
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		//samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

		// set up mipmap
		/*
		samplerDesc.MipLODBias = 0.0f; // offset, generally not useful
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX; // use all mipmaps
		*/
		GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler));
	}

	void Sampler::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
		// |__Bound sampler to pipeline register(slot 0), 
		// can be accessed with declaring SamplerState in ps
	}
	std::shared_ptr<Sampler> Sampler::Resolve(Graphics& gfx)
	{
		return BindablePool::Resolve<Sampler>(gfx);
	}
	std::string Sampler::GenerateUID()
	{
		return typeid(Sampler).name();
	}
	std::string Sampler::GetUID() const noexcept
	{
		return GenerateUID();
	}
}