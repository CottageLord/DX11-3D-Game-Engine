#include "GRAPHICS_RG_Sink.h"
#include "GRAPHICS_RG_Source.h"
#include "GRAPHICS_CLASS_RenderGraphCompileException.h"
#include "SYS_HELP_Utility.h"
#include "GRAPHICS_OBJ_Bindable.h"
#include "GRAPHICS_OBJ_BufferResource.h"
#include <sstream>
#include <cctype>

namespace Rgph
{
	Sink::Sink(std::string registeredNameIn)
		:
		registeredName(std::move(registeredNameIn))
	{
		if (registeredName.empty())
		{
			throw RGC_EXCEPTION("Empty output name");
		}
		const bool nameCharsValid = std::all_of(registeredName.begin(), registeredName.end(), [](char c) {
			return std::isalnum(c) || c == '_';
			});
		if (!nameCharsValid || std::isdigit(registeredName.front()))
		{
			throw RGC_EXCEPTION("Invalid output name: " + registeredName);
		}
	}

	const std::string& Sink::GetRegisteredName() const noexcept
	{
		return registeredName;
	}

	const std::string& Sink::GetPassName() const noexcept
	{
		return passName;
	}

	const std::string& Sink::GetOutputName() const noexcept
	{
		return outputName;
	}

	void Sink::SetTarget(std::string passName, std::string sourceName)
	{
		{
			if (passName.empty())
			{
				throw RGC_EXCEPTION("Empty output name");
			}
			const bool nameCharsValid = std::all_of(passName.begin(), passName.end(), [](char c) {
				return std::isalnum(c) || c == '_';
				});
			if (passName != "$" && (!nameCharsValid || std::isdigit(passName.front())))
			{
				throw RGC_EXCEPTION("Invalid output name: " + registeredName);
			}
			this->passName = passName;
		}
		{
			if (sourceName.empty())
			{
				throw RGC_EXCEPTION("Empty output name");
			}
			const bool nameCharsValid = std::all_of(sourceName.begin(), sourceName.end(), [](char c) {
				return std::isalnum(c) || c == '_';
				});
			if (!nameCharsValid || std::isdigit(sourceName.front()))
			{
				throw RGC_EXCEPTION("Invalid output name: " + registeredName);
			}
			this->outputName = sourceName;
		}
	}
}