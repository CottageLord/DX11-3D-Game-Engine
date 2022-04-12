#pragma once
#include <string>
#include <vector>
#include <memory>
#include "SYS_SET_ConditionalNoexcept.h"

class Graphics;

namespace GPipeline
{
	class RenderTarget;
	class DepthStencil;
}

namespace Rgph
{
	class Pass;
	class RenderQueuePass;
	class Source;
	class Sink;

	class RenderGraph
	{
	public:
		RenderGraph(Graphics& gfx);
		~RenderGraph();
		void Execute(Graphics& gfx) noxnd;
		/**
		 * @brief clear all jobs in every pass
		 */
		void Reset() noexcept;
		RenderQueuePass& GetRenderQueue(const std::string& passName);
		void StoreDepth(Graphics& gfx, const std::string& path);
	protected:
		void SetSinkTarget(const std::string& sinkName, const std::string& target);
		void AddGlobalSource(std::unique_ptr<Source>);
		void AddGlobalSink(std::unique_ptr<Sink>);
		/**
		 * @brief validating all links to make sure all sinks and sources have matches, no dangling sinks
		 */
		void Finalize();
		/**
		 * @brief append pass and start linking the sinks and sources of this pass
		 */
		void AppendPass(std::unique_ptr<Pass> pass);
		Pass& FindPassByName(const std::string& name);
	private:
		/**
		 * @brief called by AppendPass(), link all sinks of the incoming pass to the sources in the pool
		 */
		void LinkSinks(Pass& pass);
		void LinkGlobalSinks();
	private:
		std::vector<std::unique_ptr<Pass>> passes;
		std::vector<std::unique_ptr<Source>> globalSources;
		std::vector<std::unique_ptr<Sink>> globalSinks;
		std::shared_ptr<GPipeline::RenderTarget> backBufferTarget;
		std::shared_ptr<GPipeline::DepthStencil> masterDepth;
		bool finalized = false;
	};
}