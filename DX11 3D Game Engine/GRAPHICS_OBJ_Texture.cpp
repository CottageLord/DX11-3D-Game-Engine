#include "GRAPHICS_OBJ_Texture.h"
#include "GRAPHICS_OBJ_Surface.h"
#include "GRAPHICS_OBJ_BindablePool.h"
#include "SYS_SET_GraphicsThrowMacros.h"


namespace GPipeline
{
	namespace wrl = Microsoft::WRL;

	Texture::Texture(Graphics& gfx, const std::string& path, UINT slot)
		:
		path(path),
		tSlot(slot)
	{
		GET_INFO_MAN(gfx);

		// load surface
		const auto s = Surface::FromFile(path);
		hasAlpha = s.AlphaLoaded();

		// create texture2D resource
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = s.GetWidth();
		textureDesc.Height = s.GetHeight();
		textureDesc.MipLevels = 0;		// 1 for no mipmap, 0 for all mipmaps
		textureDesc.ArraySize = 1;		// single texture
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // same as back buffer
		textureDesc.SampleDesc.Count = 1;				// anti aliasing
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET; // pipeline resource that will be used in shader
		textureDesc.CPUAccessFlags = 0; // we don't need to update it after it has been created 
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		/*
		* This only works when no mipmaping
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = s.GetBufferPtr(); // set the ptr to the surface buffer
		sd.SysMemPitch = s.GetWidth() * sizeof(Surface::Color); // Pitch: distance in bytes between adjacent pixels
		*/
		wrl::ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(
						   // sd if no mipmaping
			 &textureDesc, nullptr, &pTexture // to be filled
		));

		// write image data into top mip level
		GetContext(gfx)->UpdateSubresource(
			pTexture.Get(),							// ptr to the texture resource to be filled
			0u,										// index identifies the destination subresource
			nullptr,								// A optional sub-region of resource to be updated. nullptr when updating whole thing
			s.GetBufferPtrConst(),					// A pointer to the source data in memory.
			s.GetWidth() * sizeof(Surface::Color),  // The size of one row of the source data.
			0u										// The size of one depth slice of source data.
		);

		// create the resource view on the texture
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;	// mipmaping stuff
		srvDesc.Texture2D.MipLevels = -1;		// all levels
		GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(
			pTexture.Get(), &srvDesc, &pTextureView
		));

		// generate the mip chain using the gpu rendering pipeline
		GetContext(gfx)->GenerateMips(pTextureView.Get());
	}

	void Texture::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetShaderResources(tSlot, 1u, pTextureView.GetAddressOf());
		// |__Bound texture to pipeline register(slot 0), can be accessed with 
		// declaring Texture in ps
		// Texture2D tex : register(t0) t for texture
	}

	std::shared_ptr<Texture> Texture::Resolve(Graphics& gfx, const std::string& path, UINT slot)
	{
		return BindablePool::Resolve<Texture>(gfx, path, slot);
	}
	std::string Texture::GenerateUID(const std::string& path, UINT slot)
	{
		using namespace std::string_literals;
		// distinguisah between textures in different slots
		return typeid(Texture).name() + "#"s + path + "#" + std::to_string(slot);
	}
	std::string Texture::GetUID() const noexcept
	{
		return GenerateUID(path, tSlot);
	}
	bool Texture::HasAlpha() const noexcept
	{
		return hasAlpha;
	}
	UINT Texture::CalculateNumberOfMipLevels(UINT width, UINT height) noexcept
	{
		const float xSteps = std::ceil(log2((float)width));
		const float ySteps = std::ceil(log2((float)height));
		return (UINT)std::max(xSteps, ySteps);
	}
}