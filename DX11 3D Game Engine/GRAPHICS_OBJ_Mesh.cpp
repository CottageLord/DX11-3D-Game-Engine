
#include "GRAPHICS_HELP_MatrixTranslator.h"
#include "GRAPHICS_BUF_ConstantBuffersEx.h"
#include "GRAPHICS_OBJ_Blender.h"
#include "GRAPHICS_OBJ_DynamicConstant.h"
#include "GRAPHICS_OBJ_Mesh.h"
#include "GRAPHICS_OBJ_LayoutPool.h"
#include "GRAPHICS_OBJ_Surface.h"
#include "GRAPHICS_OBJ_Stencil.h"

#include "imgui/imgui.h"
#include <unordered_map>
#include <sstream>
#include <filesystem>


namespace dx = DirectX;


// Mesh
Mesh::Mesh(Graphics& gfx, const Material& mat, const aiMesh& mesh, float scale) noxnd
	:
Drawable(gfx, mat, mesh, scale)
{}

void Mesh::Submit(FrameCommander& frame, dx::FXMMATRIX accumulatedTranform) const noxnd
{
	dx::XMStoreFloat4x4(&transform, accumulatedTranform);
	Drawable::Submit(frame);
}

DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat4x4(&transform);
}