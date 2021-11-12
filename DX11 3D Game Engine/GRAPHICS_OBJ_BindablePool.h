#pragma once

#include "GRAPHICS_OBJ_Bindable.h"
#include <memory>
#include <unordered_map>
#include <type_traits>

namespace GPipeline
{
	/**
	* @brief A singleton Bindable Pool that allows different meshes share bindables instead of generating copies
	*/
	class BindablePool
	{
	public:
		/**
		 * @brief Store or access a bindable, forwarding all required parameters for instanciating such bindable
		*/
		template<class T, typename...Params>
		static std::shared_ptr<T> Resolve(Graphics& gfx, Params&&...p) noxnd
		{
			static_assert(std::is_base_of<Bindable, T>::value, "Can only resolve classes derived from Bindable");
			// call the privat resolve
			return Get().Resolve_<T>(gfx, std::forward<Params>(p)...);
		}
	private:
		template<class T, typename...Params>
		std::shared_ptr<T> Resolve_(Graphics& gfx, Params&&...p) noxnd
		{
			// generate uid from the given file path
			const auto key = T::GenerateUID(std::forward<Params>(p)...); // all bindables implement this
			const auto i = binds.find(key);
			if (i == binds.end())
			{
				// if not stored yet, store it into the map
				auto bind = std::make_shared<T>(gfx, std::forward<Params>(p)...);
				binds[key] = bind;
				return bind;
			}
			else
			{
				// if the bindable already exists, return it
				// cast the sharedPtr from Bindable to T
				return std::static_pointer_cast<T>(i->second);
			}
		}
		static BindablePool& Get()
		{
			static BindablePool pool;
			return pool;
		}
	private:
		std::unordered_map<std::string, std::shared_ptr<Bindable>> binds;
	};
}