#pragma once
#include "SYS_SET_ConditionalNoexcept.h"
#include "GRAPHICS_SET_BindableCommon.h"
#include "GRAPHICS_OBJ_StaticDrawInfo.h"
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
class Mesh : public StaticDrawInfo<Mesh>
{
public:
	/**
	* @brief Take in the drawable data for flexibility concern (Dependency Injection)
	* @param std::vector<std::unique_ptr<Bindable>> bindPtrs all bindable info
	*/
	Mesh( Graphics& gfx,std::vector<std::unique_ptr<GPipeline::Bindable>> bindPtrs );
	/**
	* @brief Store the parents' transforms and excute draw()
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
	friend class ModelWindow;
public:
	/**
	* @brief Initialize a node with given mesh and inutial transform
	*/
	Node(const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform_in) noxnd;
	/**
	* @brief Draw a node with given accumulatedTransform
	* @param DirectX::FXMMATRIX accumulatedTransform we need to mutiply the node's trans with this one
	*/
	void Draw( Graphics& gfx,DirectX::FXMMATRIX accumulatedTransform ) const noxnd;
	/**
	* @brief
	*/
	void SetAppliedTransform(DirectX::FXMMATRIX transform) noexcept;
private:
	/**
	* @brief An addChild() that could only be accessed by friend Model class
	*/
	void AddChild( std::unique_ptr<Node> pChild ) noxnd;
	/**
	* @brief Draw a node with given accumulatedTransform
	* @param nodeIndex An unique index id for each node, increment through recursion
	* @param selectedIndex An index num for tracking the selected node, member var, neccessary for imgui
	* @param selectedNode neccessary for graphics
	*/
	void ShowTree(int& nodeIndex, std::optional<int>& selectedIndex, Node*& pSelectedNode) const noexcept;
private:
	std::string name;
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	/// <summary>
	/// Transform that loaded from the file
	/// </summary>
	DirectX::XMFLOAT4X4 localTransform;
	/// <summary>
	/// Controlled by the gui sliders
	/// </summary>
	DirectX::XMFLOAT4X4 appliedTransform;
};
/**
* @brief
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
	static std::unique_ptr<Mesh> ParseMesh( Graphics& gfx,const aiMesh& mesh );
	std::unique_ptr<Node> ParseNode( const aiNode& node ) noexcept;
private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	std::unique_ptr<class ModelWindow> pWindow;
}; 
