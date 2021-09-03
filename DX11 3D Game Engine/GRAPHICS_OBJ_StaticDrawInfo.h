/**
 * @file GRAPHICS_OBJ_StaticDrawInfo.h
 * @brief A connection class built between drawable and the actually draw target (like a box)
 * Use template feature to let compiler copy and link this obj for every child obj
 */
#pragma once
#include "GRAPHICS_OBJ_Drawable.h"
#include "GRAPHICS_BUF_IndexBuffer.h"

/*
Template class gets automatically copied for specified children
*/
template<class T>
class StaticDrawInfo : public Drawable
{
protected:
	/**
	 * @brief We assume at least 1 static bindable is here (like vertex buffer) after initialization
	 */
	/*static*/ bool IsStaticInitialized() noexcept
	{
		return !staticBinds.empty();
	}
	/*static*/ void AddStaticBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG)
	{
		assert("*Must* use AddStaticIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer)); 
		staticBinds.push_back(std::move(bind));
	}
	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept(!IS_DEBUG)
	{
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		pIndexBuffer = ibuf.get(); // set the index buf for current obj
		pSharedIndBuf = ibuf.get(); // share this index buf with all <class T> siblings
		staticBinds.push_back(std::move(ibuf));
	}
	void SetIndexFromStatic() noexcept(!IS_DEBUG)
	{
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		/*for (const auto& b : staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				pIndexBuffer = p;
				return;
			}
		}*/
		pIndexBuffer = pSharedIndBuf;
		assert("Failed to find index buffer in static binds" && pIndexBuffer != nullptr);
	}
private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
	{
		return staticBinds;
	}
private:
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
	static class IndexBuffer* pSharedIndBuf;
};
// initialize the static variable
template<class T>
std::vector<std::unique_ptr<Bindable>> StaticDrawInfo<T>::staticBinds;
template<class T>
IndexBuffer* StaticDrawInfo<T>::pSharedIndBuf;