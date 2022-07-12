#pragma once
#include "GRAPHICS_OBJ_Bindable.h"

class Surface;

namespace GPipeline
{
	class OutputOnlyDepthStencil;
	class OutputOnlyRenderTarget;
	/**
	 * @brief A box texture containing 6 textures. Used as skybox.
	 */
	class CubeTexture : public Bindable
	{
	public:
		/**
		 * @brief configures a 2d texture array as box texture
		 */
		CubeTexture(Graphics& gfx, const std::string& path, UINT slot = 0);
		/**
		 * @brief same as binding normal texture
		 */
		void Bind(Graphics& gfx) noxnd override;
	private:
		unsigned int slot;
	protected:
		std::string path;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
	/**
	 * @brief A box texture containing 6 textures. Output depth data through pixel shader and render target.
	 */
	class CubeTargetTexture : public Bindable
	{
	public:
		CubeTargetTexture(Graphics& gfx, UINT width, UINT height, UINT slot = 0, DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);
		void Bind(Graphics& gfx) noxnd override;
		std::shared_ptr<OutputOnlyRenderTarget> GetRenderTarget(size_t index) const;
	private:
		unsigned int slot;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
		std::vector<std::shared_ptr<OutputOnlyRenderTarget>> renderTargets;
	};
	/**
	 * @brief A box texture containing 6 textures. Used in shadow mapping. Output data through depth stencil and vertex shader
	 */
	class DepthCubeTexture : public Bindable
	{
	public:
		/**
		 * @brief configures a 2d texture array as box texture, using r32 data type for depth
		 */
		DepthCubeTexture(Graphics& gfx, UINT size, UINT slot = 0);
		void Bind(Graphics& gfx) noxnd override;
		/**
		 * @brief Get 1 of the 6 depth buffers by index
		 */
		std::shared_ptr<OutputOnlyDepthStencil> GetDepthBuffer(size_t index) const;
	private:
		unsigned int slot;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
		/**
		 * @brief bindable depth buffers for all 6 sides
		 */
		std::vector<std::shared_ptr<OutputOnlyDepthStencil>> depthBuffers;
	};
}