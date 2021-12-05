/**
 * @file GRAPHICS_OBJ_Texture.h
 * @brief Generate graphical texture from a surface, load the texture to the GPU pipeline
 */
#pragma once
#include "GRAPHICS_OBJ_Bindable.h"
namespace GPipeline
{
	class Texture : public Bindable
	{
	public:
		Texture(Graphics& gfx, const std::string& path, UINT slot = 0);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Texture> Resolve(Graphics& gfx, const std::string& path, UINT slot = 0);
		/**
		* @brief Generate UID while uniquely distinguisah between textures in different slots
		*/
		static std::string GenerateUID(const std::string& path, UINT slot = 0);
		std::string GetUID() const noexcept override;
		bool HasAlpha() const noexcept;
	private:
		static UINT CalculateNumberOfMipLevels(UINT width, UINT height) noexcept;
	protected:
		bool hasAlpha = false;
		std::string path;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	private:
		unsigned int tSlot;
	};

}