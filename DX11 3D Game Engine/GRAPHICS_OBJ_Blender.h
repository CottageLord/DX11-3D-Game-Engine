#pragma once
#include "GRAPHICS_OBJ_Bindable.h"
#include <array>
#include <optional>

namespace GPipeline
{
	class Blender : public Bindable
	{
	public:
		/**
		 * @brief Set up the blender state
		 * @param blending do we need (alpha) blending?
		 * @param factor universal rgba blender factors that controls blending effect (like all fading out)
		 */
		Blender(Graphics& gfx, bool blending, std::optional<float> factor = {});
		/**
		 * @brief Bind to Output Merger
		 */
		void Bind(Graphics& gfx) noexcept override;
		/**
		 * @brief Set the universal effect factor
		 */
		void SetFactor(float factor) noxnd;
		/**
		 * @brief Get the universal effect factor
		 */
		float GetFactor() const noxnd;
		static std::shared_ptr<Blender> Resolve(Graphics& gfx, bool blending, std::optional<float> factor = {});
		static std::string GenerateUID(bool blending, std::optional<float> factor);
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11BlendState> pBlender;
		/// @brief enable blending or not
		bool blending;
		/// @brief universal RGBA blender factors
		std::optional<std::array<float, 4>> factors;
	};
}