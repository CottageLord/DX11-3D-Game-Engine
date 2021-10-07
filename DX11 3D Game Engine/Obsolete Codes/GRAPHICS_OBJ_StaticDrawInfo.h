/**
 * @file GRAPHICS_OBJ_StaticDrawInfo.h
 * @brief A class between the drawable and the actually draw target (like a box)
 * Use template class feature such that all StaticDrawInfo<drawable type> could share same attributes, like vertex buffer
 */
#pragma once
#include "GRAPHICS_OBJ_Drawable.h"
#include "GRAPHICS_BUF_IndexBuffer.h"
#include "SYS_SET_ConditionalNoexcept.h"

/*
Template class gets automatically shared within a group of children and exists independently across types
*/
template<class T>
class StaticDrawInfo : public Drawable
{
protected:
	/**
	 * @brief return true if we have bind buffer(s). We assume at least 1 bufferis there upon initialization
	 */
	/*static*/ bool IsStaticInitialized() noexcept
	{
		return !staticBinds.empty();
	}
	/**
	 * @brief bind a static object (i.e. Vertex Buffer of all cubes)
	 * @param bind unique_ptr to the object
	 */
	/*static*/ void AddStaticBind(std::unique_ptr<GPipeline::Bindable> bind) noxnd
	{
		assert("*Must* use AddStaticIndexBuffer to bind index buffer" && 
			typeid(*bind) != typeid(GPipeline::IndexBuffer));
		staticBinds.push_back(std::move(bind));
	}
	/**
	 * @brief bind the shared index buffer
	 * @param ibuf unique_ptr to the index buffer
	 */
	void AddStaticIndexBuffer(std::unique_ptr<GPipeline::IndexBuffer> ibuf) noxnd
	{
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		pIndexBuffer = ibuf.get(); // set the index buf for current obj
		pSharedIndBuf = ibuf.get(); // share this index buf with all <class T> siblings
		staticBinds.push_back(std::move(ibuf));
	}
	/**
	 * @brief bind the shared index buffer with the shared ibuf from siblings
	 */
	void SetIndexFromStatic() noxnd
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
	/**
	 * @brief returns pointers to all shared bindable data
	 */
	const std::vector<std::unique_ptr<GPipeline::Bindable>>& GetStaticBinds() const noexcept override
	{
		return staticBinds;
	}
private:
	/// all shared bindable data
	static std::vector<std::unique_ptr<GPipeline::Bindable>> staticBinds;
	/// the shared index buffer
	static class GPipeline::IndexBuffer* pSharedIndBuf;
};
// initialize the static variable
template<class T>
std::vector<std::unique_ptr<GPipeline::Bindable>> StaticDrawInfo<T>::staticBinds;
template<class T>
GPipeline::IndexBuffer* StaticDrawInfo<T>::pSharedIndBuf;