#pragma once
#include "GRAPHICS_JOB_Step.h"
#include <vector>


class Technique
{
public:
	void Submit(class FrameCommander& frame, const class Drawable& drawable) const noexcept;
	void AddStep(Step step) noexcept
	{
		steps.push_back(std::move(step));
	}
	void Activate() noexcept
	{
		active = true;
	}
	void Deactivate() noexcept
	{
		active = false;
	}
	/**
	  * @brief This function allows us to make the techniques shareable between draws
	  * everytime a technique is assigned to a new object, the drawable reference (containing
	  * corresponding vertex and index data) can be updated.
	  */
	void InitializeParentReferences(const class Drawable& parent) noexcept;
private:
	bool active = true;
	std::vector<Step> steps;
};