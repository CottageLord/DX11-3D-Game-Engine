#pragma once
#include "GRAPHICS_PASS_BindingPass.h"

namespace GPipeline
{
	class IndexBuffer;
	class VertexBuffer;
	class VertexShader;
	class InputLayout;
}

namespace Rgph
{
	class FullscreenPass : public BindingPass
	{
	public:
		FullscreenPass(const std::string name, Graphics& gfx) noxnd;
		void Execute(Graphics& gfx) const noxnd override;
	};
}