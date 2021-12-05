#pragma once
#include "GRAPHICS_OBJ_Bindable.h"
#include <array>
#include <optional>

namespace GPipeline
{
	class Blender : public Bindable
	{
	public:
		Blender(Graphics& gfx, bool blending, std::optional<float> factor = {});
		void Bind(Graphics& gfx) noexcept override;
		void SetFactor(float factor) noxnd;
		float GetFactor() const noxnd;
		static std::shared_ptr<Blender> Resolve(Graphics& gfx, bool blending, std::optional<float> factor = {});
		static std::string GenerateUID(bool blending, std::optional<float> factor);
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11BlendState> pBlender;
		/// @brief enable blending or not
		bool blending;
		std::optional<std::array<float, 4>> factors;
	};
}