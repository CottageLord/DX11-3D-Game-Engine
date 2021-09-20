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
		Texture(Graphics& gfx, const class Surface& s);
		void Bind(Graphics& gfx) noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}