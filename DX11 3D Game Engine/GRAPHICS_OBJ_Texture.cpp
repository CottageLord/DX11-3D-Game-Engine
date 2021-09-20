#include "GRAPHICS_OBJ_Texture.h"
#include "GRAPHICS_OBJ_Surface.h"
#include "SYS_SET_GraphicsThrowMacros.h"

namespace GPipeline
{
	namespace wrl = Microsoft::WRL;

	Texture::Texture(Graphics& gfx, const Surface& s)
	{
		GET_INFO_MAN(gfx);

		// create texture2D resource
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = s.GetWidth();
		textureDesc.Height = s.GetHeight();
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;		// single texture
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // same as back buffer
		textureDesc.SampleDesc.Count = 1;				// anti aliasing
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // pipeline resource that will be used in shader
		textureDesc.CPUAccessFlags = 0; // we don't need to update it after it has been created 
		textureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = s.GetBufferPtr(); // set the ptr to the surface buffer
		sd.SysMemPitch = s.GetWidth() * sizeof(Surface::Color); // Pitch: distance in bytes between adjacent pixels
		wrl::ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(
			&textureDesc, &sd, &pTexture // to be filled
		));

		// create the resource view on the texture
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;	// mipmaping stuff
		srvDesc.Texture2D.MipLevels = 1;
		GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(
			pTexture.Get(), &srvDesc, &pTextureView
		));
	}

	void Texture::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf());
		// |__Bound texture to pipeline register(slot 0), can be accessed with 
		// declaring Texture in ps
		// Texture2D tex : register(t0) t for texture
	}
}