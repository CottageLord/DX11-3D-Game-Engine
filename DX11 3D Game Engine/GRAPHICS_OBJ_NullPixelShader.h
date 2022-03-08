#pragma once
#include "GRAPHICS_OBJ_Bindable.h"

namespace GPipeline
{
	class NullPixelShader : public Bindable
	{
	public:
		NullPixelShader(Graphics& gfx);
		void Bind(Graphics& gfx) noxnd override;
		static std::shared_ptr<NullPixelShader> Resolve(Graphics& gfx);
		static std::string GenerateUID();
		std::string GetUID() const noexcept override;
	};
}