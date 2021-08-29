/**
 * @file .h
 * @brief 
 */
#pragma once
#include "SYS_CLASS_Graphics.h"
#include "SYS_SET_GraphicsThrowMacros.h"
#include "GRAPHICS_BUF_IndexBuffer.h"
#include <cassert>
#include <typeinfo>
#include <DirectXMath.h>

class Bindable;

class Drawable
{
private:
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
	 * @brief AddBind.
	 * @param bind The bindable objects that will be draw
	 */
	void AddBind(std::unique_ptr<Bindable> bind) noexcept (!IS_DEBUG);
	/**
	 * @brief AddIndexBuffer.
	 * @param ibuf The bindable index buffer that will be used in drawing
	 */
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept (!IS_DEBUG);
private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;
	const class IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};