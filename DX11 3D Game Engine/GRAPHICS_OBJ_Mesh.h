#pragma once
#include "SYS_SET_ConditionalNoexcept.h"
#include "SYS_CLASS_Graphics.h"
#include "GRAPHICS_OBJ_Drawable.h"
/*
#include "GRAPHICS_SET_BindableCommon.h"
#include "GRAPHICS_OBJ_DynamicVertex.h"
#include "GRAPHICS_BUF_ConstantBuffers.h"
#include "GRAPHICS_OBJ_DynamicConstant.h"
*/

/*
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <type_traits>
#include "imgui/imgui.h"

#include <optional>
#include <filesystem>
*/

class Material;
class FrameCommander;
struct aiMesh;

/**
 * @brief contains (partial) mesh data of an object
 */
class Mesh : public Drawable
{
public:
	Mesh(Graphics& gfx, const Material& mat, const aiMesh& mesh, float scale = 1.0f) noxnd;
	/**
	 * @brief Take in the drawable data for flexibility concern (Dependency Injection)
	 * @param std::vector<std::unique_ptr<Bindable>> bindPtrs all bindable info
	 
	Mesh( Graphics& gfx,std::vector<std::shared_ptr<GPipeline::Bindable>> bindPtrs );*/
	/**
	 * @brief Store the parents' transforms and excute the parent's draw()
	 * @param DirectX::FXMMATRIX accumulatedTransform The combined transform from the parent nodes
	 
	void Draw( Graphics& gfx,DirectX::FXMMATRIX accumulatedTransform ) const noxnd;*/
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void Submit(DirectX::FXMMATRIX accumulatedTranform) const noxnd;
private:
	mutable DirectX::XMFLOAT4X4 transform;
};
