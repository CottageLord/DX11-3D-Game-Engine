#pragma once
#include "GRAPHICS_OBJ_Bindable.h"
#include "GRAPHICS_OBJ_BufferResource.h"
#include <optional>
#include <d3d11.h>

class Graphics;

namespace GPipeline
{
	class DepthStencil;
	class Surface;
	/**
	 * @brief RenderTarget represents a texture-like bindable resource that can be bound to the Output Merger stage. 
	 * It is used to ship data from GPU back to CPU for future uses. Examples include fullscreen filtering and shadowmapping textures.
	 */
	class RenderTarget : public Bindable, public BufferResource
	{
	public:
		/**
		 * @brief: Bind one or more render targets atomically and the depth-stencil buffer to the output-merger stage.
		 */
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
		/**
		 * @brief Creates a render target with 1.texture - setting up the description of the texture and then creating that texture
		 * 2.texture view - uses that texture to setup a render target view so that the texture can be drawn to as a render target
		 */
		RenderTarget(Graphics& gfx, ID3D11Texture2D* pTexture, std::optional<UINT> face);
		RenderTarget(Graphics& gfx, UINT width, UINT height);
		UINT width;
		UINT height;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;
	};
	/**
	 * @brief ShaderInputRenderTarget represents a render target capable of being fed into the render pipeline as shader input texture
	 */
	class ShaderInputRenderTarget : public RenderTarget
	{
	public:
		/**
		 * @brief Creates a render target with 1.texture - setting up the description of the texture and then creating that texture
		 * 2.texture view - uses that texture to setup a render target view so that the texture can be drawn to as a render target
		 * 3. shader resource view - create a shader resource view of that texture so that we can supply the rendered data to calling objects.		 
		 */
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

	/**
	 * @brief OutputOnlyRenderTarget is used to store the final redered result from the pipeline.
	 * Examples include cube shadow maps and back buffer
	 */
	class OutputOnlyRenderTarget : public RenderTarget
	{
	public:
		void Bind(Graphics& gfx) noxnd override;
		/**
		 * @brief Creates a render target with View only, meaning that this is used for referencing the output only
		 */
		OutputOnlyRenderTarget(Graphics& gfx, ID3D11Texture2D* pTexture, std::optional<UINT> face = {});
	};
}
