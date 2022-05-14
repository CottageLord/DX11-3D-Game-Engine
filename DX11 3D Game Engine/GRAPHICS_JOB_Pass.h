#pragma once
#include "SYS_SET_ConditionalNoexcept.h"
#include <string>
#include <vector>
#include <array>
#include <memory>

class Graphics;

namespace GPipeline
{
	class RenderTarget;
	class DepthStencil;
}

namespace Rgph
{
	class Sink;
	class Source;
	class Pass
	{
	public:
		Pass(std::string name) noexcept;
		virtual void Execute(Graphics& gfx) const noxnd = 0;
		virtual void Reset() noxnd;
		const std::string& GetName() const noexcept;
		const std::vector<std::unique_ptr<Sink>>& GetSinks() const;
		Source& GetSource(const std::string& registeredName) const;
		Sink& GetSink(const std::string& registeredName) const;
		/*
		 * @brief link one sink to a source, meaning that this sink expects the output from this source
		 */
		void SetSinkLinkage(const std::string& sinkName, const std::string& sourceName);
		virtual void Finalize();
		virtual ~Pass();
	protected:
		/*
		 * @brief add one sink to the sink pool, this is the prerequisite for SetSinkLinkage on this sink
		 */
		void RegisterSink(std::unique_ptr<Sink> sink);
		/*
		 * @brief add one sink to the source pool, this is the prerequisite for SetSinkLinkage on this source
		 */
		void RegisterSource(std::unique_ptr<Source> source);
	private:
		std::vector<std::unique_ptr<Sink>> sinks;
		std::vector<std::unique_ptr<Source>> sources;
		std::string name;
	};
}