#pragma once
#include "GRAPHICS_BUF_TransformCbuffer.h"
#include "GRAPHICS_OBJ_DynamicConstant.h"

namespace GPipeline
{
	class TransformCbufScaling : public TransformCbuffer
	{
	public:
		TransformCbufScaling(Graphics& gfx, float scale);
		void Accept(TechniqueProbe& probe) override;
		void Bind(Graphics& gfx) noxnd override;
		std::unique_ptr<CloningBindable> Clone() const noexcept override;
	private:
		static Dcb::RawLayout MakeLayout();
	private:
		Dcb::Buffer buf;
	};
}