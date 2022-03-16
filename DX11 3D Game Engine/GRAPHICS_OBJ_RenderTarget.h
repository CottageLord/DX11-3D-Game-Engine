#pragma once
#include "GRAPHICS_OBJ_Bindable.h"
#include "GRAPHICS_OBJ_BufferResource.h"

#include <d3d11.h>

class Graphics;

namespace GPipeline
{
	class DepthStencil;
	class Surface;
	/**
	 * @brief RenderTarget represents a texture-like bindable resource. This resource could have various usings,
	 * including fullscreen filter blur effect
	 */
	class RenderTarget : public Bindable, public BufferResource
	{
	public:
		void BindAsBuffer(Graphics& gfx) noxnd override;
		void BindAsBuffer(Graphics& gfx, BufferResource* bufferResource) noxnd override;
		void BindAsBuffer(Graphics& gfx, DepthStencil* depthStencil) noxnd;
		void Clear(Graphics& gfx) noxnd override;
		void Clear(Graphics& gfx, const std::array<float, 4>& color) noxnd;
		UINT GetWidth() const noexcept;
		UINT GetHeight() const noexcept;
	private:
		void BindAsBuffer(Graphics& gfx, ID3D11DepthStencilView* pDepthStencilView) noxnd;
	protected:
		RenderTarget(Graphics& gfx, ID3D11Texture2D* pTexture);
		RenderTarget(Graphics& gfx, UINT width, UINT height);
		UINT width;
		UINT height;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;
	};

	class ShaderInputRenderTarget : public RenderTarget
	{
	public:
		ShaderInputRenderTarget(Graphics& gfx, UINT width, UINT height, UINT slot);
		void Bind(Graphics& gfx) noxnd override;
		/**
		 * @brief Helps storing GPU side textures as image files. As GPU side data is not readable from CPU side,
		 * a staging texture is needed as the medium.
		 */
		Surface ToSurface(Graphics& gfx) const;
	private:
		UINT slot;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
	};

	// RT for Graphics to create RenderTarget for the back buffer
	class OutputOnlyRenderTarget : public RenderTarget
	{
		friend Graphics;
	public:
		void Bind(Graphics& gfx) noxnd override;
	private:
		OutputOnlyRenderTarget(Graphics& gfx, ID3D11Texture2D* pTexture);
	};
}
