#pragma once
#include "SYS_SET_ConditionalNoexcept.h"
#include "GRAPHICS_SET_BindableCommon.h"
//#include "GRAPHICS_OBJ_StaticDrawInfo.h"
#include "GRAPHICS_OBJ_Drawable.h"
#include "GRAPHICS_OBJ_DynamicVertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <optional>

/**
* @brief An exception object that reports model loading error
*/
class ModelException : public MFException
{
public:
	ModelException(int line, const char* file, std::string note) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	const std::string& GetNote() const noexcept;
private:
	std::string note;
};

/**
* @brief
*/
class Mesh : public Drawable
{
public:
	/**
	* @brief Take in the drawable data for flexibility concern (Dependency Injection)
	* @param std::vector<std::unique_ptr<Bindable>> bindPtrs all bindable info
	*/
	Mesh( Graphics& gfx,std::vector<std::shared_ptr<GPipeline::Bindable>> bindPtrs );
	/**
	* @brief Store the parents' transforms and excute the parent's draw()
	* @param DirectX::FXMMATRIX accumulatedTransform The combined transform from the parent nodes
	*/
	void Draw( Graphics& gfx,DirectX::FXMMATRIX accumulatedTransform ) const noxnd;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	mutable DirectX::XMFLOAT4X4 transform;
};
/**
* @brief
*/
class Node
{
	friend class Model;
public:
	/**
	* @brief Initialize a node with given mesh and inutial transform
	*/
	Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform) noxnd;
	/**
	* @brief Draw a node with given accumulatedTransform
	* @param DirectX::FXMMATRIX accumulatedTransform we need to mutiply the node's trans with this one
	*/
	void Draw( Graphics& gfx,DirectX::FXMMATRIX accumulatedTransform ) const noxnd;
	/**
	* @brief stores the new position of this node
	*/
	void SetAppliedTransform(DirectX::FXMMATRIX transform) noexcept;
	/**
	* @brief Draw a node with given accumulatedTransform
	* @param nodeIndex An unique index id for each node, increment through recursion
	* @param selectedIndex An index num for tracking the selected node, member var, neccessary for imgui
	* @param selectedNode neccessary for graphics
	*/
	void ShowTree(Node*& pSelectedNode) const noexcept;
	/**
	* @brief return the permanant ID of the current node
	*/
	int GetId() const noexcept;
private:
	/**
	* @brief An addChild() that could only be accessed by friend Model class
	*/
	void AddChild( std::unique_ptr<Node> pChild ) noxnd;
private:
	int id;
	std::string name;
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	/// <summary>
	/// Transform that loaded from the file
	/// </summary>
	DirectX::XMFLOAT4X4 localTransform;
	/// <summary>
	/// the moved position in the engine
	/// </summary>
	DirectX::XMFLOAT4X4 appliedTransform;
};
/**
* @brief a loaded model with multiple nodes
*/
class Model
{
public:
	/**
	* @brief Load a model with given model file
	*/
	Model( Graphics& gfx,const std::string fileName );
	~Model() noexcept;
	void Draw(Graphics& gfx) const noxnd;
	void ShowWindow(const char* windowName = nullptr) noexcept;
private:
	/**
	* @brief parse and store (vertex/index/normal/tc...) data from the mesh file
	* @param mesh A reference to the model itself
	* @param pMaterials An array of ptrs to materials
	*/
	static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials);
	/**
	* @brief parse the node structure from the assimp node input
	*/
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node) noexcept;
private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	std::unique_ptr<class ModelWindow> pWindow;
}; 
