#pragma once
#include "GRAPHICS_JOB_TechniqueProbe.h"

namespace Rgph
{
	class RenderGraph;
}

class StepLinkingProbe : public TechniqueProbe
{
protected:
	void OnSetStep() override;
private:
	Rgph::RenderGraph& rg;
};