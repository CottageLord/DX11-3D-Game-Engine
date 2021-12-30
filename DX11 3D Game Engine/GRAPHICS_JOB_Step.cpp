
#include "GRAPHICS_JOB_Step.h"
#include "GRAPHICS_OBJ_Drawable.h"
#include "GRAPHICS_JOB_FrameCommander.h"

void Step::Submit(FrameCommander& frame, const Drawable& drawable) const
{
	frame.Accept(Job{ this,&drawable }, targetPass);
}

void Step::InitializeParentReferences(const Drawable& parent) noexcept
{
	for (auto& b : bindables)
	{
		b->InitializeParentReference(parent);
	}
}