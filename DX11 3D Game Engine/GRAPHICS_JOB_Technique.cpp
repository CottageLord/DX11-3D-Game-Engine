
#include "GRAPHICS_JOB_Technique.h"
#include "GRAPHICS_OBJ_Drawable.h"
#include "GRAPHICS_JOB_FrameCommander.h"

void Technique::Submit(FrameCommander& frame, const Drawable& drawable) const noexcept
{
	if (active)
	{
		for (const auto& step : steps)
		{
			step.Submit(frame, drawable);
		}
	}
}

void Technique::InitializeParentReferences(const Drawable& parent) noexcept
{
	for (auto& s : steps)
	{
		s.InitializeParentReferences(parent);
	}
}