/**
 * @file GRAPHICS_OBJ_Drawable.h
 * @brief The base class for all drawable geomrtric primitives. Should contain all bindable buffers and settings.
 */
#pragma once
#include "SYS_CLASS_Graphics.h"
#include "SYS_SET_ConditionalNoexcept.h"
#include "GRAPHICS_JOB_Technique.h"
#include <DirectXMath.h>
#include <memory>

class TechniqueProbe;
class Material;
struct aiMesh;

namespace GPipeline
{
	class IndexBuffer;
	class VertexBuffer;
	class Topology;
	class InputLayout;
}

/**
 * @brief The base class for all drawable geomrtric primitives (Cube, Sphere, Plane...).
 */
class Drawable
{
public:
	Drawable() = default;
	Drawable(Graphics& gfx, const Material& mat, const aiMesh& mesh, float scale = 1.0f) noexcept;
	Drawable(const Drawable&) = delete;
	void AddTechnique(Technique tech_in) noexcept;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Submit(class FrameCommander& frame) const noexcept;
	void Bind(Graphics& gfx) const noexcept;
	void Accept(TechniqueProbe& probe);
	UINT GetIndexCount() const noxnd;
	virtual ~Drawable();
protected:
	/**
	 * @brief Stores the *dynamic* bindable objects/settings, like transformation matrix. 
	 * @param bind The bindable object that contains things like vertex buffer.
	 */
	//void AddBind(std::shared_ptr<GPipeline::Bindable> bind) noxnd;
	/**
	 * @brief Stores the index buffer. This need to be specialized because the index are independent argument in DX's Draw()
	 * @param ibuf The bindable index buffer that will be used in drawing
	 
	void AddIndexBuffer(std::unique_ptr<GPipeline::IndexBuffer> ibuf) noxnd;
	*/
	std::shared_ptr<GPipeline::IndexBuffer> pIndices;
	std::shared_ptr<GPipeline::VertexBuffer> pVertices;
	std::shared_ptr<GPipeline::Topology> pTopology;
	std::vector<Technique> techniques;
private:
	/**
	 * @brief Get all of the *static* bindable objects/settings, like vertex buffer 
	
	virtual const std::vector<std::unique_ptr<GPipeline::Bindable>>& GetStaticBinds() const noexcept = 0;
	 */
	// pointer to the GRAPHICS_BUF_IndexBuffer object, this should be set of every drawable instance
	//const class GPipeline::IndexBuffer* pIndexBuffer = nullptr;
	// stores all *dynamic* bindable objects
	//std::vector<std::shared_ptr<GPipeline::Bindable>> binds;
};