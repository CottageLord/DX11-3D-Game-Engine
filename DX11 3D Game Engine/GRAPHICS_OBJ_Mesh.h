#pragma once
#include "SYS_SET_ConditionalNoexcept.h"
#include "GRAPHICS_OBJ_StaticDrawInfo.h"
#include "GRAPHICS_SET_BindableCommon.h"
#include "GRAPHICS_OBJ_DynamicVertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
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
public:
	/**
	* @brief Initialize a node with given mesh and inutial transform
	*/
	Node( std::vector<Mesh*> meshPtrs,const DirectX::XMMATRIX& transform ) noxnd;
	/**
	* @brief Draw a node with given accumulatedTransform
	* @param DirectX::FXMMATRIX accumulatedTransform we need to mutiply the node's trans with this one
	*/
	void Draw( Graphics& gfx,DirectX::FXMMATRIX accumulatedTransform ) const noxnd;
private:
	/**
	* @brief An addChild() that could only be accessed by friend Model class
	*/
	void AddChild( std::unique_ptr<Node> pChild ) noxnd;
private:
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	DirectX::XMFLOAT4X4 transform;
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
	void Draw( Graphics& gfx,DirectX::FXMMATRIX transform ) const;
private:
	static std::unique_ptr<Mesh> ParseMesh( Graphics& gfx,const aiMesh& mesh );
	std::unique_ptr<Node> ParseNode( const aiNode& node );
private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
}; 
