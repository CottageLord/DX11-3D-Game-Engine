#include "GRAPHICS_JOB_Job.h"
#include "GRAPHICS_JOB_Step.h"
#include "GRAPHICS_OBJ_Drawable.h"


Job::Job(const Step* pStep, const Drawable* pDrawable)
	:
	pDrawable{ pDrawable },
	pStep{ pStep }
{}

void Job::Execute(Graphics& gfx) const noxnd
{
	pDrawable->Bind(gfx);
	pStep->Bind(gfx);
	gfx.DrawIndexed(pDrawable->GetIndexCount());
}
