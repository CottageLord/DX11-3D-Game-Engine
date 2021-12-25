#pragma once
#include "GRAPHICS_OBJ_DynamicConstant.h"
#include <string>
#include <memory>
#include <unordered_map>

namespace Dcb
{
	class LayoutPool
	{
	public:
		static Dcb::CookedLayout Resolve(Dcb::RawLayout&& layout) noxnd;
	private:
		static LayoutPool& Get_() noexcept;
		std::unordered_map<std::string, std::shared_ptr<Dcb::LayoutElement>> map;
	};
}