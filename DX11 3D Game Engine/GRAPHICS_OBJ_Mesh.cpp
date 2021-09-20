
#include "GRAPHICS_OBJ_Mesh.h"

// Mesh
Mesh::Mesh(Graphics & gfx, std::vector<std::unique_ptr<GPipeline::Bindable>> bindPtrs)
{
	if (!IsStaticInitialized())
	{
		AddStaticBind(std::make_unique<GPipeline::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	for (auto& pb : bindPtrs)
	{
		// if passed, we have two unique pointers pointing to the index buff
		if (auto pi = dynamic_cast<GPipeline::IndexBuffer*>(pb.get()))
		{
			AddIndexBuffer(std::unique_ptr<GPipeline::IndexBuffer>{ pi });
			// thus we need to release the original one
			pb.release();
		}
		else
		{
			AddBind(std::move(pb));
		}
	}

	AddBind(std::make_unique<GPipeline::TransformCbuffer>(gfx, *this));
}
void Mesh::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noxnd
{
	DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
	Drawable::Draw(gfx);
}
DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat4x4(&transform);
}


// Node
Node::Node(std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform) noxnd
	:
meshPtrs(std::move(meshPtrs))
{
	DirectX::XMStoreFloat4x4(&this->transform, transform);
}
void Node::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noxnd
{
	const auto built = DirectX::XMLoadFloat4x4(&transform) * accumulatedTransform;
	// draw recursively
	for (const auto pm : meshPtrs) { pm->Draw(gfx, built); }
	for (const auto& pc : childPtrs) { pc->Draw(gfx, built); }
}
void Node::AddChild(std::unique_ptr<Node> pChild) noxnd
{
	assert(pChild);
	childPtrs.push_back(std::move(pChild));
}


// Model
Model::Model(Graphics& gfx, const std::string fileName)
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
void Model::Draw(Graphics& gfx, DirectX::FXMMATRIX transform) const
{
	// draw on the root, trigger the recursion
	pRoot->Draw(gfx, transform);
}
std::unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx, const aiMesh& mesh)
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
			*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mVertices[i]),
			*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i])
		);
	}
	// iterate through the polygons
	std::vector<unsigned short> indices;
	indices.reserve(mesh.mNumFaces * 3);
	for (unsigned int i = 0; i < mesh.mNumFaces; i++)
	{
		const auto& face = mesh.mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	std::vector<std::unique_ptr<GPipeline::Bindable>> bindablePtrs;

	bindablePtrs.push_back(std::make_unique<GPipeline::VertexBuffer>(gfx, vbuf));

	bindablePtrs.push_back(std::make_unique<GPipeline::IndexBuffer>(gfx, indices));

	auto pvs = std::make_unique<GPipeline::VertexShader>(gfx, L"PhongVS.cso");
	auto pvsbc = pvs->GetBytecode();
	bindablePtrs.push_back(std::move(pvs));

	bindablePtrs.push_back(std::make_unique<GPipeline::PixelShader>(gfx, L"PhongPS.cso"));

	bindablePtrs.push_back(std::make_unique<GPipeline::InputLayout>(gfx, vbuf.GetLayout().GetD3DLayout(), pvsbc));
	// temporary material
	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 color = { 0.6f,0.6f,0.8f };
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];
	} pmc;
	bindablePtrs.push_back(std::make_unique<GPipeline::PixelConstantBuffer<PSMaterialConstant>>(gfx, pmc, 1u));
	// create the mesh drawable
	return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));
}
std::unique_ptr<Node> Model::ParseNode(const aiNode& node)
{
	namespace dx = DirectX;
	// change between row/colomn major
	const auto transform = dx::XMMatrixTranspose(dx::XMLoadFloat4x4(
		reinterpret_cast<const dx::XMFLOAT4X4*>(&node.mTransformation)
	));
	// maintain the mesh_ptr in the curr child node
	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);
	for (size_t i = 0; i < node.mNumMeshes; i++)
	{
		const auto meshIdx = node.mMeshes[i];
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