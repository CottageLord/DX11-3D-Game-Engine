#pragma once
#include "SYS_SET_ConditionalNoexcept.h"

class Drawable;
class Graphics;
class Step;

namespace Rgph
{
	/**
	* @brief represents a job in the job pool, contains pointers to a step (the job) and a drawable object (the job's target)
	*/
	class Job
	{
	public:
		Job(const Step* pStep, const Drawable* pDrawable);
		void Execute(Graphics& gfx) const noxnd;
	private:
		const class Drawable* pDrawable;
		const class Step* pStep;
	};
}