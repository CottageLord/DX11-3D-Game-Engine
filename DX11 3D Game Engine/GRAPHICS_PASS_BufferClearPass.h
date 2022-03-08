#pragma once
#include "GRAPHICS_JOB_Pass.h"

namespace GPipeline
{
	class BufferResource;
}

namespace Rgph
{
	class BufferClearPass : public Pass
	{
	public:
		BufferClearPass(std::string name);
		void Execute(Graphics& gfx) const noxnd override;
	private:
		std::shared_ptr<GPipeline::BufferResource> buffer;
	};
}