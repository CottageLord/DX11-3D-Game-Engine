#pragma once
#include "GRAPHICS_JOB_Pass.h"
#include "GRAPHICS_RG_Sink.h"

namespace GPipeline
{
	class Bindable;
}

namespace Rgph
{
	class BindingPass : public Pass
	{
	protected:
		BindingPass(std::string name, std::vector<std::shared_ptr<GPipeline::Bindable>> binds = {});
		void AddBind(std::shared_ptr<GPipeline::Bindable> bind) noexcept;
		void BindAll(Graphics& gfx) const noexcept;
		void Finalize() override;
	protected:
		template<class T>
		void AddBindSink(std::string name)
		{
			const auto index = binds.size();
			binds.emplace_back();
			RegisterSink(std::make_unique<ContainerBindableSink<T>>(std::move(name), binds, index));
		}
		std::shared_ptr<GPipeline::RenderTarget> renderTarget;
		std::shared_ptr<GPipeline::DepthStencil> depthStencil;
	private:
		void BindBufferResources(Graphics& gfx) const noxnd;
	private:
		std::vector<std::shared_ptr<GPipeline::Bindable>> binds;
	};
}