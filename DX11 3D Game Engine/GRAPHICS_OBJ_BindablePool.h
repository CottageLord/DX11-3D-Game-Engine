#pragma once

#include "GRAPHICS_OBJ_Bindable.h"
#include <memory>
#include <unordered_map>
#include <type_traits>

namespace GPipeline
{
	/**
	* @brief A singleton Bindable Pool that maps bindable object path with the shared content
	*/
	class BindablePool
	{
	public:
		template<class T, typename...Params>
		static std::shared_ptr<T> Resolve(Graphics& gfx, Params&&...p) noxnd
		{
			static_assert(std::is_base_of<Bindable, T>::value, "Can only resolve classes derived from Bindable");
			return Get().Resolve_<T>(gfx, std::forward<Params>(p)...);
		}
	private:
		template<class T, typename...Params>
		std::shared_ptr<T> Resolve_(Graphics& gfx, Params&&...p) noxnd
		{
			// generate uid from the given path
			const auto key = T::GenerateUID(std::forward<Params>(p)...);
			const auto i = binds.find(key);
			if (i == binds.end())
			{
				auto bind = std::make_shared<T>(gfx, std::forward<Params>(p)...);
				binds[key] = bind;
				return bind;
			}
			else
			{
				// cast the sharedPtr from Bindable to T
				return std::static_pointer_cast<T>(i->second);
			}
		}
		static BindablePool& Get()
		{
			static BindablePool codex;
			return codex;
		}
	private:
		std::unordered_map<std::string, std::shared_ptr<Bindable>> binds;
	};
}