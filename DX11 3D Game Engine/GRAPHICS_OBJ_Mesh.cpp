
#include "GRAPHICS_OBJ_Mesh.h"
#include "imgui/imgui.h"
#include <unordered_map>
#include <sstream>

namespace dx = DirectX;

ModelException::ModelException(int line, const char* file, std::string note) noexcept
	:
	MFException(line, file),
	note(std::move(note))
{}

const char* ModelException::what() const noexcept
{
	std::ostringstream oss;
	oss << MFException::what() << std::endl
		<< "[Note] " << GetNote();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* ModelException::GetType() const noexcept
{
	return "Chili Model Exception";
}

const std::string& ModelException::GetNote() const noexcept
{
	return note;
}

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
Node::Node(const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform_in) noxnd
	:
	meshPtrs(std::move(meshPtrs)),
	name(name)
{
	dx::XMStoreFloat4x4(&localTransform, transform_in);
	dx::XMStoreFloat4x4(&appliedTransform, dx::XMMatrixIdentity());
}

void Node::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noxnd
{
	const auto built =
		dx::XMLoadFloat4x4(&appliedTransform) *
		dx::XMLoadFloat4x4(&localTransform) *
		accumulatedTransform;
	// draw recursively
	for (const auto pm : meshPtrs) { pm->Draw(gfx, built); }
	for (const auto& pc : childPtrs) { pc->Draw(gfx, built); }
}
void Node::AddChild(std::unique_ptr<Node> pChild) noxnd
{
	assert(pChild);
	childPtrs.push_back(std::move(pChild));
}

void Node::ShowTree(int& nodeIndexTracked, std::optional<int>& selectedIndex, Node*& pSelectedNode) const noexcept
{
	// nodeIndex serves as the uid for gui tree nodes, incremented throughout recursion
	const int currentNodeIndex = nodeIndexTracked;
	nodeIndexTracked++;
	// build up flags for current node
	const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow // openable
		// Show selected only if the curr id == selected id
		| ((currentNodeIndex == selectedIndex.value_or(-1)) ? ImGuiTreeNodeFlags_Selected : 0)
		| ((childPtrs.empty()) ? ImGuiTreeNodeFlags_Leaf : 0); // show leaf if no child
	// if tree node expanded, recursively render all children
	// render this node
	const auto expanded = ImGui::TreeNodeEx(
		(void*)(intptr_t)currentNodeIndex, node_flags, name.c_str()
	);
	// processing for selecting node regardless of selested or not
	if (ImGui::IsItemClicked())
	{
		selectedIndex = currentNodeIndex;
		pSelectedNode = const_cast<Node*>(this);
	}
	// recursive rendering of open node's children
	if (expanded)
	{
		for (const auto& pChild : childPtrs)
		{
			pChild->ShowTree(nodeIndexTracked, selectedIndex, pSelectedNode);
		}
		ImGui::TreePop();
	}
}

void Node::SetAppliedTransform(DirectX::FXMMATRIX transform) noexcept
{
	dx::XMStoreFloat4x4(&appliedTransform, transform);
}

// Model
class ModelWindow // pImpl idiom, only defined in this .cpp
{
public:
	void Show(const char* windowName, const Node& root) noexcept
	{
		// window name defaults to "Model"
		windowName = windowName ? windowName : "Model";
		// need an ints to track node indices and selected node
		int nodeIndexTracker = 0;
		if (ImGui::Begin(windowName))
		{
			ImGui::Columns(2, nullptr, true);
			root.ShowTree(nodeIndexTracker, selectedIndex, pSelectedNode);

			ImGui::NextColumn();
			if (pSelectedNode != nullptr)
			{
				auto& transform = transforms[*selectedIndex];
				ImGui::Text("Orientation");
				ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f);
				ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f);
				ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f);
				ImGui::Text("Position");
				ImGui::SliderFloat("X", &transform.x, -20.0f, 20.0f);
				ImGui::SliderFloat("Y", &transform.y, -20.0f, 20.0f);
				ImGui::SliderFloat("Z", &transform.z, -20.0f, 20.0f);
			}
		}
		ImGui::End();
	}
	dx::XMMATRIX GetTransform() const noexcept
	{
		const auto& transform = transforms.at(*selectedIndex);
		return
			dx::XMMatrixRotationRollPitchYaw(transform.roll, transform.pitch, transform.yaw) *
			dx::XMMatrixTranslation(transform.x, transform.y, transform.z);
	}

	Node* GetSelectedNode() const noexcept
	{
		return pSelectedNode;
	}
private:
	std::optional<int> selectedIndex;
	Node* pSelectedNode;
	struct TransformParameters
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};
	/// <summary>
	/// allows us to keep track of each node's transform data
	/// </summary>
	std::unordered_map<int, TransformParameters> transforms;
	
};

// Model
Model::Model(Graphics& gfx, const std::string fileName)
	:
	pWindow(std::make_unique<ModelWindow>())
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(fileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals
	);

	if (pScene == nullptr)
	{
		throw ModelException(__LINE__, __FILE__, imp.GetErrorString());
	}

	// store all meshes
	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		meshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i]));
	}

	// recursively calls all children
	pRoot = ParseNode(*pScene->mRootNode);
}

void Model::Draw(Graphics& gfx) const noxnd
{
	if (auto node = pWindow->GetSelectedNode())
	{
		node->SetAppliedTransform(pWindow->GetTransform());
	}
	pRoot->Draw(gfx, dx::XMMatrixIdentity());
}

void Model::ShowWindow(const char* windowName) noexcept
{
	pWindow->Show(windowName, *pRoot);
}

Model::~Model() noexcept
{}

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
std::unique_ptr<Node> Model::ParseNode(const aiNode& node) noexcept
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
	auto pNode = std::make_unique<Node>(node.mName.C_Str(), std::move(curMeshPtrs), transform);
	for (size_t i = 0; i < node.mNumChildren; i++)
	{
		pNode->AddChild(ParseNode(*node.mChildren[i]));
	}
	return pNode;
}