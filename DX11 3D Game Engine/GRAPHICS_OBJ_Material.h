#pragma once
#include "SYS_CLASS_Graphics.h"
#include "GRAPHICS_JOB_Technique.h"
#include "GRAPHICS_OBJ_DynamicVertex.h"
#include <filesystem>
#include <vector>

struct aiMaterial;
struct aiMesh;

namespace GPipeline
{
	class VertexBuffer;
	class IndexBuffer;
}

class Material
{
public:
	/**
	  * @brief Constructs a material given the model file path
	  */
	Material(Graphics& gfx, const aiMaterial& material, const std::filesystem::path& path) noxnd;
	/**
	  * @brief Extract the vertecies from the aimesh, according to dynamically configured vtxlayout
	  */
	DynamicVertex::VertexBuffer ExtractVertices(const aiMesh& mesh) const noexcept;
	/**
	  * @brief Extract the indecies ffrom the aimesh.
	  */
	std::vector<unsigned short> ExtractIndices(const aiMesh& mesh) const noexcept;
	/**
	  * @brief Resolve the vertex buffer.
	  */
	std::shared_ptr<GPipeline::VertexBuffer> MakeVertexBindable(Graphics& gfx, const aiMesh& mesh, 
		float scale = 1.0f) const noxnd;
	std::shared_ptr<GPipeline::IndexBuffer> MakeIndexBindable(Graphics& gfx, const aiMesh& mesh) const noxnd;
	std::vector<Technique> GetTechniques() const noexcept;
private:
	std::string MakeMeshTag(const aiMesh& mesh) const noexcept;
private:
	DynamicVertex::VertexLayout vtxLayout;
	std::vector<Technique> techniques;
	std::string modelPath;
	std::string name;
};