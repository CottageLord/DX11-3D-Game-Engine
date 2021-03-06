#pragma once
#include "GRAPHICS_OBJ_Bindable.h"
#include "GRAPHICS_OBJ_BufferResource.h"
#include "GRAPHICS_OBJ_Surface.h"

class Graphics;

namespace GPipeline
{
	class RenderTarget;
	/**
	* @brief A depthstencil texture for recording various data. Inherit from Bindable (for better fitting in RenderGraph system) and
	* BufferResource. It can help passing shadow map data to shaders.
	*/
	class DepthStencil : public Bindable, public BufferResource
	{
		friend RenderTarget;
	public:
		/**
		* @brief Distinguish between depth stencil usage:
		* when using for shadow buffering, the 8 bits stencil is no longer useful.
		* Configuring a ShadowDepth will use a 24 bits data unit, instead of 32
		*/
		enum class Usage
		{
			DepthStencil,
			ShadowDepth,
		};
	public:
		void BindAsBuffer(Graphics& gfx) noxnd override;
		void BindAsBuffer(Graphics& gfx, BufferResource* renderTarget) noxnd override;
		void BindAsBuffer(Graphics& gfx, RenderTarget* rt) noxnd;
		void Clear(Graphics& gfx) noxnd override;
		/**
		* @brief A debug helper that converts render targets into surfaces so we can store them as image files
		*/
		Surface ToSurface(Graphics& gfx, bool linearlize = true) const;
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
	protected:
		/**
		 * @brief Create a bindable depth stencil associated with 1 of the 6 sides on a cube map texture. 
		 *			Creates a 1 side view to view 1 of the 6 sides.
		 */
		DepthStencil(Graphics& gfx, Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture, UINT face);
		DepthStencil(Graphics& gfx, UINT width, UINT height, bool canBindShaderInput, Usage usage);
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
		unsigned int width;
		unsigned int height;
	};
	/**
	* @brief 
	*/
	class ShaderInputDepthStencil : public DepthStencil
	{
	public:
		ShaderInputDepthStencil(Graphics& gfx, UINT slot, Usage usage = Usage::DepthStencil);
		ShaderInputDepthStencil(Graphics& gfx, UINT width, UINT height, UINT slot, Usage usage = Usage::DepthStencil);
		void Bind(Graphics& gfx) noxnd override;
		void Clear(Graphics& gfx) noxnd override;
	private:
		UINT slot;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
	};
	/**
	* @brief 
	*/
	class OutputOnlyDepthStencil : public DepthStencil
	{
	public:
		/**
		 * @brief Create a bindable depth stencil associated with 1 of the 6 sides on a cube map texture. Calls corresponding base method
		 */
		OutputOnlyDepthStencil(Graphics& gfx, Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture, UINT face);
		OutputOnlyDepthStencil(Graphics& gfx);
		OutputOnlyDepthStencil(Graphics& gfx, UINT width, UINT height);
		void Bind(Graphics& gfx) noxnd override;
	};
}