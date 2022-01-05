#pragma once
#include "SYS_CLASS_Graphics.h"
#include "SYS_SET_ConditionalNoexcept.h"
#include "GRAPHICS_OBJ_GraphicsResource.h"
#include <memory>
#include <string>

class Drawable;
class TechniqueProbe;

namespace GPipeline
{
	class Bindable : public GraphicsResource
	{
	public:
		virtual void Bind(Graphics& gfx) noexcept = 0;
		virtual void InitializeParentReference(const Drawable&) noexcept
		{}
		virtual void Accept(TechniqueProbe&)
		{}
		virtual std::string GetUID() const noexcept
		{
			assert(false);
			return "";
		}
		virtual ~Bindable() = default;
	};
	class CloningBindable : public Bindable
	{
	public:
		virtual std::unique_ptr<CloningBindable> Clone() const noexcept = 0;
	};
}