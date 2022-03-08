#include "GRAPHICS_JOB_StepLinkingProbe.h"
#include "GRAPHICS_JOB_Step.h"
#include "GRAPHICS_RG_RenderGraph.h"

void StepLinkingProbe::OnSetStep()
{
	pStep->Link(rg);
}