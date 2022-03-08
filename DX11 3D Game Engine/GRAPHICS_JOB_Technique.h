#pragma once
#include "GRAPHICS_JOB_Step.h"
#include <vector>

class TechniqueProbe;
class Drawable;

namespace Rgph
{
	class RenderGraph;
}

class Technique
{
public:
	Technique() = default;
	Technique(std::string name, bool startActive = true) noexcept;
	void Submit(const Drawable& drawable) const noexcept;
	void AddStep(Step step) noexcept;
	bool IsActive() const noexcept;
	void SetActiveState(bool active_in) noexcept;
	/**
	  * @brief This function allows us to make the techniques shareable between draws
	  * everytime a technique is assigned to a new object, the drawable reference (containing
	  * corresponding vertex and index data) can be updated.
	  */
	void InitializeParentReferences(const Drawable& parent) noexcept;
	void Accept(TechniqueProbe& probe);
	const std::string& GetName() const noexcept;
	void Link(Rgph::RenderGraph&);
private:
	bool active = true;
	std::vector<Step> steps;
	std::string name = "Nameless Tech";
};