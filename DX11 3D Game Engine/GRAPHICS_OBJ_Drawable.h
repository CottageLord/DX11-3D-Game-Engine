/**
 * @file GRAPHICS_OBJ_Drawable.h
 * @brief The base class for all drawable geomrtric primitives. Should contain all bindable buffers and settings.
 */
#pragma once
#include "SYS_CLASS_Graphics.h"
#include <DirectXMath.h>

class Bindable;

/**
 * @brief The base class for all drawable geomrtric primitives (Cube, Sphere, Plane...).
 */
class Drawable
{
private:
	// makes sure the *static* connector could update the index buffer pointer in each instance
	template<class T>
	friend class StaticDrawInfo;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Draw(Graphics& gfx) const noexcept(!IS_DEBUG);
	virtual void Update(float dt) noexcept = 0;
	virtual ~Drawable() = default;
protected:
	/**
	 * @brief Stores the *dynamic* bindable objects/settings, like transformation matrix. 
	 * @param bind The bindable object that contains things like vertex buffer.
	 */
	void AddBind(std::unique_ptr<Bindable> bind) noexcept (!IS_DEBUG);
	/**
	 * @brief Stores the index buffer. This need to be specialized because the index are independent argument in DX's Draw()
	 * @param ibuf The bindable index buffer that will be used in drawing
	 */
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept (!IS_DEBUG);
private:
	/**
	 * @brief Get all of the *static* bindable objects/settings, like vertex buffer 
	 */
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;
	/// pointer to the GRAPHICS_BUF_IndexBuffer object, this should be set of every drawable instance
	const class IndexBuffer* pIndexBuffer = nullptr;
	/// stores all *dynamic* bindable objects
	std::vector<std::unique_ptr<Bindable>> binds;
};