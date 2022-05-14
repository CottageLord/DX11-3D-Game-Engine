/**
 * @filename GRAPHICS_OBJ_Rasterizer.h
 * @brief Configures pipeline behavior at the rasterization stage. For example, disabling backface culling for sheet object
 */
#pragma once
#include "GRAPHICS_OBJ_Bindable.h"
#include <array>

namespace GPipeline
{
	class Rasterizer : public Bindable
	{
	public:
		/**
		 * @brief Configures the Rasterizer with different settings
		 * @param twoSided set to true to disable backface culling when rendering sheet geometry.
		 */
		Rasterizer(Graphics& gfx, bool twoSided);
		void Bind(Graphics& gfx) noxnd override;
		static std::shared_ptr<Rasterizer> Resolve(Graphics& gfx, bool twoSided);
		static std::string GenerateUID(bool twoSided);
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;
		bool twoSided;
	};
}