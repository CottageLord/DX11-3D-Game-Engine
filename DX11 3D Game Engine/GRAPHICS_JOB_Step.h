#pragma once
#include <vector>
#include <memory>
#include "GRAPHICS_OBJ_Bindable.h"
#include "SYS_CLASS_Graphics.h"

class TechniqueProbe;
class Drawable;

namespace Rgph
{
	class RenderQueuePass;
	class RenderGraph;
}
/**
* @brief Represents (one of) the step[s] to finish a drawing technique. Contains all bindables relate to the sub-job.
*/
class Step
{
public:
	Step(std::string targetPassName);
	Step(Step&&) = default;
	Step(const Step& src) noexcept;
	Step& operator=(const Step&) = delete;
	Step& operator=(Step&&) = delete;
	void AddBindable(std::shared_ptr<GPipeline::Bindable> bind_in) noexcept;
	void Submit(const Drawable& drawable) const;
	void Bind(Graphics& gfx) const noxnd;
	void InitializeParentReferences(const Drawable& parent) noexcept;
	void Accept(TechniqueProbe& probe);
	void Link(Rgph::RenderGraph& rg);
private:
	std::vector<std::shared_ptr<GPipeline::Bindable>> bindables;
	Rgph::RenderQueuePass* pTargetPass = nullptr;
	std::string targetPassName;
};