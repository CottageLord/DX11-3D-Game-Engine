/**
 * @file GRAPHICS_OBJ_Drawable.h
 * @brief The base class for all drawable geomrtric primitives. Should contain all bindable buffers and settings.
 */
#pragma once
#include "SYS_CLASS_Graphics.h"
#include "SYS_SET_ConditionalNoexcept.h"
#include <DirectXMath.h>
#include <memory>

namespace GPipeline
{
	class Bindable;
	class IndexBuffer;
}

/**
 * @brief The base class for all drawable geomrtric primitives (Cube, Sphere, Plane...).
 */
class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Draw(Graphics& gfx) const noxnd;
	virtual ~Drawable() = default;
protected:
	/**
	 * @brief Find the particular bindable object by its type.
	 */
	template<class T>
	T* QueryBindable() noexcept
	{
		for (auto& pb : binds)
		{
			if (auto pt = dynamic_cast<T*>(pb.get()))
			{
				return pt;
			}
		}
		return nullptr;
	}
	/**
	 * @brief Stores the *dynamic* bindable objects/settings, like transformation matrix. 
	 * @param bind The bindable object that contains things like vertex buffer.
	 */
	void AddBind(std::shared_ptr<GPipeline::Bindable> bind) noxnd;
	/**
	 * @brief Stores the index buffer. This need to be specialized because the index are independent argument in DX's Draw()
	 * @param ibuf The bindable index buffer that will be used in drawing
	 
	void AddIndexBuffer(std::unique_ptr<GPipeline::IndexBuffer> ibuf) noxnd;
	*/
private:
	/**
	 * @brief Get all of the *static* bindable objects/settings, like vertex buffer 
	
	virtual const std::vector<std::unique_ptr<GPipeline::Bindable>>& GetStaticBinds() const noexcept = 0;
	 */
	/// pointer to the GRAPHICS_BUF_IndexBuffer object, this should be set of every drawable instance
	const class GPipeline::IndexBuffer* pIndexBuffer = nullptr;
	/// stores all *dynamic* bindable objects
	std::vector<std::shared_ptr<GPipeline::Bindable>> binds;
};