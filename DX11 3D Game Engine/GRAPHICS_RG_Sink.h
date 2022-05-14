#pragma once
#include "GRAPHICS_RG_Source.h"
#include "GRAPHICS_CLASS_RenderGraphCompileException.h"
#include "GRAPHICS_OBJ_Bindable.h"
#include "GRAPHICS_OBJ_BufferResource.h"
#include "GRAPHICS_OBJ_RenderTarget.h"
#include "GRAPHICS_OBJ_DepthStencil.h"

#include <string>
#include <memory>
#include <typeinfo>
#include <sstream>
#include <cctype>
#include <type_traits>

namespace GPipeline
{
	class Bindable;
}

namespace Rgph
{
	
	class Pass;
	/**
	* @brief Base class for all sinks in a render graph
	*/
	class Sink
	{
	public:
		const std::string& GetRegisteredName() const noexcept;
		const std::string& GetPassName() const noexcept;
		const std::string& GetOutputName() const noexcept;
		void SetTarget(std::string passName, std::string sourceName);
		/**
		 * @brief Called by render graph. Once the relationships between sources and sinks are clear, bind them
		 */
		virtual void Bind(Source& source) = 0;
		virtual void PostLinkValidate() const = 0;
		virtual ~Sink() = default;
	protected:
		Sink(std::string registeredName);
	private:
		std::string registeredName;
		std::string passName;
		std::string outputName;
	};
	/**
	* @brief A sink that requires a specific resource in the render graph, hence the data cannot be shared and the order can be switched
	*/
	template<class T>
	class DirectBufferSink : public Sink
	{
		static_assert(std::is_base_of_v<GPipeline::BufferResource, T>, "DirectBufferSink target type must be a BufferResource type");
	public:
		static std::unique_ptr<Sink> Make(std::string registeredName, std::shared_ptr<T>& target)
		{
			return std::make_unique<DirectBufferSink>(std::move(registeredName), target);
		}
		void PostLinkValidate() const override
		{
			if (!linked)
			{
				throw RGC_EXCEPTION("Unlinked input: " + GetRegisteredName());
			}
		}
		/**
		 * @brief link this sink to target resource, check compatibility by dynamic_cast
		 */
		void Bind(Source& source) override
		{
			auto p = std::dynamic_pointer_cast<T>(source.YieldBuffer());
			if (!p)
			{
				std::ostringstream oss;
				oss << "Binding input [" << GetRegisteredName() << "] to output [" << GetPassName() << "." << GetOutputName() << "] "
					<< " { " << typeid(T).name() << " } not compatible with { " << typeid(*source.YieldBuffer().get()).name() << " }";
				throw RGC_EXCEPTION(oss.str());
			}
			target = std::move(p);
			linked = true;
		}
		DirectBufferSink(std::string registeredName, std::shared_ptr<T>& bind)
			:
			Sink(std::move(registeredName)),
			target(bind)
		{}
	private:
		/**
		  *@brief Stores the reference from input (source), which contains some concrete data (like the renderTarget data and depthStencil data)
		  */
		std::shared_ptr<T>& target;
		bool linked = false;
	};
	/**
	  * @brief A sink that requires a general resource in the render graph, hence the data can be shared and the order can be switched
	  * Used only in the BindingPass to prevent invalid pointer (which is used in normal bindableSink) 
	  * when the container size changed
	  * NOTE it is still not safe, since we are using fixed index - reordered/swaped vector still break the system.
	  * Right now we live under the assumption of no reordering bindables
	  */
	template<class T>
	class ContainerBindableSink : public Sink
	{
		static_assert(std::is_base_of_v<GPipeline::Bindable, T>, "DirectBindableSink target type must be a Bindable type");
	public:
		void PostLinkValidate() const override
		{
			if (!linked)
			{
				throw RGC_EXCEPTION("Unlinked input: " + GetRegisteredName());
			}
		}
		void Bind(Source& source) override
		{
			auto p = std::dynamic_pointer_cast<T>(source.YieldBindable());
			if (!p)
			{
				std::ostringstream oss;
				oss << "Binding input [" << GetRegisteredName() << "] to output [" << GetPassName() << "." << GetOutputName() << "] "
					<< " { " << typeid(T).name() << " } does not match { " << typeid(*source.YieldBindable().get()).name() << " }";
				throw RGC_EXCEPTION(oss.str());
			}
			container[index] = std::move(p);
			linked = true;
		}
		ContainerBindableSink(std::string registeredName, std::vector<std::shared_ptr<GPipeline::Bindable>>& container, size_t index)
			:
			Sink(std::move(registeredName)),
			container(container),
			index(index)
		{}
	private:
		std::vector<std::shared_ptr<GPipeline::Bindable>>& container;
		/**
		  * @brief the index of the specific resource we are asking for
		  */
		size_t index;
		bool linked = false;
	};
	/**
	  * @brief A sink that requires a general resource in the render graph, hence the data can be shared and the order can be switched
	  */
	template<class T>
	class DirectBindableSink : public Sink
	{
		static_assert(std::is_base_of_v<GPipeline::Bindable, T>, "DirectBindableSink target type must be a Bindable type");
	public:
		static std::unique_ptr<Sink> Make(std::string registeredName, std::shared_ptr<T>& target)
		{
			return std::make_unique<DirectBindableSink>(std::move(registeredName), target);
		}
		void PostLinkValidate() const override
		{
			if (!linked)
			{
				throw RGC_EXCEPTION("Unlinked input: " + GetRegisteredName());
			}
		}
		void Bind(Source& source) override
		{
			auto p = std::dynamic_pointer_cast<T>(source.YieldBindable());
			if (!p)
			{
				std::ostringstream oss;
				oss << "Binding input [" << GetRegisteredName() << "] to output [" << GetPassName() << "." << GetOutputName() << "] "
					<< " { " << typeid(T).name() << " } does not match { " << typeid(*source.YieldBindable().get()).name() << " }";
				throw RGC_EXCEPTION(oss.str());
			}
			target = std::move(p);
			linked = true;
		}
		DirectBindableSink(std::string registeredName, std::shared_ptr<T>& target)
			:
			Sink(std::move(registeredName)),
			target(target)
		{}
	private:
		std::shared_ptr<T>& target;
		bool linked = false;
	};
}