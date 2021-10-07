#include "GRAPHICS_OBJ_Drawable.h"
#include "SYS_SET_GraphicsThrowMacros.h"
#include "GRAPHICS_BUF_IndexBuffer.h"
#include <cassert>

using namespace GPipeline;

/**
* @brief Binds all info to the gpu pipeline and excute drawIndexed()
*/
void Drawable::Draw(Graphics& gfx) const noxnd
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::shared_ptr<Bindable> bind) noxnd
{
	// special case for index buffer
	if (typeid(*bind) == typeid(IndexBuffer))
	{
		assert("Binding multiple index buffers not allowed" && pIndexBuffer == nullptr);
		pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
	}
	binds.push_back(std::move(bind));
}
