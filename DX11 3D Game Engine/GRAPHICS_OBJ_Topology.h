#pragma once
#include "GRAPHICS_OBJ_Bindable.h"

namespace GPipeline
{
	class Topology : public Bindable
	{
	public:
		Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Topology> Resolve(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
		static std::string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type);
		std::string GetUID() const noexcept override;
	protected:
		D3D11_PRIMITIVE_TOPOLOGY type;
	};
}