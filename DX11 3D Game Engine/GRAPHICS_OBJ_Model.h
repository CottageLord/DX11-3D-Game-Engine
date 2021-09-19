#pragma once
#include "GRAPHICS_OBJ_StaticDrawInfo.h"
#include "GRAPHICS_SET_BindableBase.h"
#include "GRAPHICS_OBJ_DynamicVertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh : public StaticDrawInfo<Mesh>
{
public:
	/**
	* @brief Take in the drawable data for flexibility concern (Dependency Injection)
	* @param std::vector<std::unique_ptr<Bindable>> bindPtrs all bindable info
	*/
	Mesh(Graphics& gfx, std::vector<std::unique_ptr<Bindable>> bindPtrs)
	{
		if (!IsStaticInitialized())
		{
			AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}

		for (auto& pb : bindPtrs)
		{
			if (auto pi = dynamic_cast<IndexBuffer*>(pb.get()))
			{
				// if passed, we have two unique pointers pointing to the index buff
				AddIndexBuffer(std::unique_ptr<IndexBuffer>{ pi });
				// thus we need to release the original one
				pb.release();
			}
			else{ AddBind(std::move(pb)); }
		}

		AddBind(std::make_unique<TransformCbuffer>(gfx, *this));
	}
	/**
	* @brief Store the parents' transforms and excute draw()
	* @param DirectX::FXMMATRIX accumulatedTransform The combined transform from the parent nodes
	*/
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept(!IS_DEBUG)
	{
		DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
		Drawable::Draw(gfx);
	}
	DirectX::XMMATRIX GetTransformXM() const noexcept override
	{
		return DirectX::XMLoadFloat4x4(&transform);
	}
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
	Node(std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform) noexcept(!IS_DEBUG)
		:
		meshPtrs(std::move(meshPtrs))
	{
		DirectX::XMStoreFloat4x4(&this->transform, transform);
	}
	/**
	* @brief Draw a node with given accumulatedTransform
	* @param DirectX::FXMMATRIX accumulatedTransform we need to mutiply the node's trans with this one
	*/
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept(!IS_DEBUG)
	{
		const auto built = DirectX::XMLoadFloat4x4(&transform) * accumulatedTransform;
		// draw recursively
		for (const auto pm : meshPtrs) { pm->Draw(gfx, built); }
		for (const auto& pc : childPtrs) { pc->Draw(gfx, built); }
	}
private:
	/**
	* @brief An addChild() that could only be accessed by friend Model class
	*/
	void AddChild(std::unique_ptr<Node> pChild) noexcept(!IS_DEBUG)
	{
		assert(pChild);
		childPtrs.push_back(std::move(pChild));
	}
private:
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	DirectX::XMFLOAT4X4 transform;
};

class Model
{
public:
	/**
	* @brief Load a model with given model file
	*/
	Model(Graphics& gfx, const std::string fileName)
	{
		Assimp::Importer imp;
		const auto pScene = imp.ReadFile(fileName.c_str(),
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices
		);
		// store all meshes
		for (size_t i = 0; i < pScene->mNumMeshes; i++)
		{
			meshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i]));
		}
		// recursively calls all children
		pRoot = ParseNode(*pScene->mRootNode);
	}

	void Draw(Graphics& gfx, DirectX::FXMMATRIX transform) const
	{
		// draw on the root, trigger the recursion
		pRoot->Draw(gfx, transform);
	}
private:
	static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh)
	{
		namespace dx = DirectX;
		using DynamicVertex::VertexLayout;
		// specify layout
		DynamicVertex::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
		));
		// interpret vertecies
		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mVertices[i]), // positions
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i])
			);
		}

		std::vector<unsigned short> indices;
		// iterate through the polygons
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			// correspond to triangle vertecies
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		std::vector<std::unique_ptr<Bindable>> bindablePtrs;

		bindablePtrs.push_back(std::make_unique<VertexBuffer>(gfx, vbuf));

		bindablePtrs.push_back(std::make_unique<IndexBuffer>(gfx, indices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));

		bindablePtrs.push_back(std::make_unique<InputLayout>(gfx, vbuf.GetLayout().GetD3DLayout(), pvsbc));
		// temporary material
		struct PSMaterialConstant
		{
			DirectX::XMFLOAT3 color = { 0.6f,0.6f,0.8f };
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[3];
		} pmc;
		bindablePtrs.push_back(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, pmc, 1u));
		// create the mesh drawable
		return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));
	}

	std::unique_ptr<Node> ParseNode(const aiNode& node)
	{
		namespace dx = DirectX;
		// change between row/colomn major
		const auto transform = dx::XMMatrixTranspose(dx::XMLoadFloat4x4(
			reinterpret_cast<const dx::XMFLOAT4X4*>(&node.mTransformation)
		));
		// maintain the mesh_ptr in the curr child node
		std::vector<Mesh*> curMeshPtrs;
		curMeshPtrs.reserve(node.mNumMeshes);
		// retrieve the mesh ptr from the indicies
		for (size_t i = 0; i < node.mNumMeshes; i++)
		{
			const auto meshIdx = node.mMeshes[i];// array of indicies into pScene->meshes[]
			curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
		}
		// call ParseNode() in every child
		auto pNode = std::make_unique<Node>(std::move(curMeshPtrs), transform);
		for (size_t i = 0; i < node.mNumChildren; i++)
		{
			pNode->AddChild(ParseNode(*node.mChildren[i]));
		}

		return pNode;
	}
private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
};