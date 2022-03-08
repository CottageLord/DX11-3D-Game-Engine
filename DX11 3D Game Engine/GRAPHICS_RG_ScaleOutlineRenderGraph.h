#pragma once
#include "GRAPHICS_RG_RenderGraph.h"

class Graphics;

namespace Rgph
{
	class ScaleOutlineRenderGraph : public RenderGraph
	{
	public:
		ScaleOutlineRenderGraph(Graphics& gfx);
	};
}