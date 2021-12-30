
#include "GRAPHICS_OBJ_Mesh.h"
#include "GRAPHICS_OBJ_Surface.h"
#include "GRAPHICS_OBJ_Blender.h"
#include "GRAPHICS_HELP_MatrixTranslator.h"
#include "GRAPHICS_OBJ_DynamicConstant.h"
#include "GRAPHICS_BUF_ConstantBuffersEx.h"
#include "GRAPHICS_OBJ_LayoutPool.h"
#include "GRAPHICS_OBJ_Stencil.h"

#include "imgui/imgui.h"
#include <unordered_map>
#include <sstream>
#include <filesystem>

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
/*
Mesh::Mesh(Graphics & gfx, std::vector<std::shared_ptr<GPipeline::Bindable>> bindPtrs)
{
	// make sure to share the topology
	AddBind(GPipeline::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	for (auto& pb : bindPtrs)
	{
		AddBind(std::move(pb));
	}

	AddBind(std::make_shared<GPipeline::TransformCbuffer>(gfx, *this));
}*/

void Mesh::Submit(FrameCommander& frame, dx::FXMMATRIX accumulatedTranform) const noxnd
{
	dx::XMStoreFloat4x4(&transform, accumulatedTranform);
	Drawable::Submit(frame);
}

DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat4x4(&transform);
}


// Node
Node::Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform_in) noxnd
	:
	id(id),
	meshPtrs(std::move(meshPtrs)),
	name(name)
{
	dx::XMStoreFloat4x4(&localTransform, transform_in);
	dx::XMStoreFloat4x4(&appliedTransform, dx::XMMatrixIdentity());
}

	void Node::Submit(FrameCommander& frame, DirectX::FXMMATRIX accumulatedTransform) const noxnd
{
	const auto built =
		dx::XMLoadFloat4x4(&appliedTransform) *
		dx::XMLoadFloat4x4(&localTransform) *
		accumulatedTransform;
	// draw recursively
	for (const auto pm : meshPtrs) { pm->Submit(frame, accumulatedTransform); }
	for (const auto& pc : childPtrs) { pc->Submit(frame, accumulatedTransform); }
}
void Node::AddChild(std::unique_ptr<Node> pChild) noxnd
{
	assert(pChild);
	childPtrs.push_back(std::move(pChild));
}

void Node::ShowTree(Node*& pSelectedNode) const noexcept
{
	// if there is no selected node, set selectedId to an impossible value
	const int selectedId = (pSelectedNode == nullptr) ? -1 : pSelectedNode->GetId();

	// build up flags for current node
	const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow // openable
		// Show selected only if the curr id == selected id
		| ((GetId() == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
		| ((childPtrs.empty()) ? ImGuiTreeNodeFlags_Leaf : 0); // show leaf if no child
	// if tree node expanded, recursively render all children
	// render this node
	const auto expanded = ImGui::TreeNodeEx(
		(void*)(intptr_t)GetId(), node_flags, name.c_str()
	);
	// processing for selecting node regardless of selested or not
	if (ImGui::IsItemClicked())
	{
		pSelectedNode = const_cast<Node*>(this);
	}
	// recursive rendering of open node's children
	if (expanded)
	{
		for (const auto& pChild : childPtrs)
		{
			pChild->ShowTree(pSelectedNode);
		}
		ImGui::TreePop();
	}
}
/*
const Dcb::Buffer* Node::GetMaterialConstants() const noxnd
{
	if (meshPtrs.size() == 0)
	{
		return nullptr;
	}
	auto pBindable = meshPtrs.front()->QueryBindable<GPipeline::CachingPixelConstantBufferEX>();
	return &pBindable->GetBuffer();
}

void Node::SetMaterialConstants(const Dcb::Buffer& buf_in) noxnd
{
	auto pcb = meshPtrs.front()->QueryBindable<GPipeline::CachingPixelConstantBufferEX>();
	assert(pcb != nullptr);
	pcb->SetBuffer(buf_in);
}*/

void Node::SetAppliedTransform(DirectX::FXMMATRIX transform) noexcept
{
	dx::XMStoreFloat4x4(&appliedTransform, transform);
}

int Node::GetId() const noexcept { return id; }

const DirectX::XMFLOAT4X4& Node::GetAppliedTransform() const noexcept
{
	return appliedTransform;
}

// Model
class ModelWindow // pImpl idiom, only defined in this .cpp
{
public:
	void Show(Graphics& gfx, const char* windowName, const Node& root) noexcept
	{
		/*
		// window name defaults to "Model"
		windowName = windowName ? windowName : "Model";
		// need an ints to track node indices and selected node
		int nodeIndexTracker = 0;
		if (ImGui::Begin(windowName))
		{
			ImGui::Columns(2, nullptr, true);
			root.ShowTree(pSelectedNode);

			ImGui::NextColumn();
			// search the node transform and make sure we don't reinitialize any nodes
			if (pSelectedNode != nullptr)
			{
				const auto id = pSelectedNode->GetId();
				auto i = transforms.find(id);
				if (i == transforms.end())
				{
					const auto& applied = pSelectedNode->GetAppliedTransform();
					const auto angles = ExtractEulerAngles(applied);
					const auto translation = ExtractTranslation(applied);
					TransformParameters tp;
					tp.roll = angles.z;
					tp.pitch = angles.x;
					tp.yaw = angles.y;
					tp.x = translation.x;
					tp.y = translation.y;
					tp.z = translation.z;
					
					//insert() returns pair<iterator (to the newly inseted info if insertion success), bool (whether success or not)>
					//std::tie destruct the returned pair, ignore the second bool
					
					//std::tie(i, std::ignore) = transforms.insert({ id,tp });
					auto pMatConst = pSelectedNode->GetMaterialConstants();
					auto buf = pMatConst != nullptr ? std::optional<Dcb::Buffer>{ *pMatConst } : std::optional<Dcb::Buffer>{};
					std::tie(i, std::ignore) = transforms.insert({ id,{ tp,false,std::move(buf),false } });
				}
				// link imgui ctrl to our cached transform params
				{
					auto& transform = i->second.tranformParams;
					// dirty check
					auto& dirty = i->second.transformParamsDirty;
					const auto dcheck = [&dirty](bool changed) {dirty = dirty || changed; };
					// widgets
					ImGui::Text("Orientation");
					dcheck(ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f));
					dcheck(ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f));
					dcheck(ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f));
					ImGui::Text("Position");
					dcheck(ImGui::SliderFloat("X", &transform.x, -20.0f, 20.0f));
					dcheck(ImGui::SliderFloat("Y", &transform.y, -20.0f, 20.0f));
					dcheck(ImGui::SliderFloat("Z", &transform.z, -20.0f, 20.0f));
				}
				// link imgui ctrl to our cached material params
				if (i->second.materialCbuf)
				{
					auto& mat = *i->second.materialCbuf;
					// dirty check
					auto& dirty = i->second.materialCbufDirty;
					const auto dcheck = [&dirty](bool changed) {dirty = dirty || changed; };
					// widgets
					ImGui::Text("Material");
					if (auto v = mat["normalMapEnabled"]; v.Exists())
					{
						dcheck(ImGui::Checkbox("Norm Map", &v));
					}
					if (auto v = mat["specularMapEnabled"]; v.Exists())
					{
						dcheck(ImGui::Checkbox("Spec Map", &v));
					}
					if (auto v = mat["hasGlossMap"]; v.Exists())
					{
						dcheck(ImGui::Checkbox("Gloss Map", &v));
					}
					if (auto v = mat["materialColor"]; v.Exists())
					{
						dcheck(ImGui::ColorPicker3("Diff Color", reinterpret_cast<float*>(&static_cast<dx::XMFLOAT3&>(v))));
					}
					if (auto v = mat["specularPower"]; v.Exists())
					{
						dcheck(ImGui::SliderFloat("Spec Power", &v, 0.0f, 100.0f, "%.1f", 1.5f));
					}
					if (auto v = mat["specularColor"]; v.Exists())
					{
						dcheck(ImGui::ColorPicker3("Spec Color", reinterpret_cast<float*>(&static_cast<dx::XMFLOAT3&>(v))));
					}
					if (auto v = mat["specularMapWeight"]; v.Exists())
					{
						dcheck(ImGui::SliderFloat("Spec Weight", &v, 0.0f, 4.0f));
					}
					if (auto v = mat["specularIntensity"]; v.Exists())
					{
						dcheck(ImGui::SliderFloat("Spec Intens", &v, 0.0f, 1.0f));
					}
				}
			}
		}
		ImGui::End();
		*/
	}
	void ApplyParameters() noxnd
	{
		/*
		if (TransformDirty())
		{
			pSelectedNode->SetAppliedTransform(GetTransform());
			ResetTransformDirty();
		}
		if (MaterialDirty())
		{
			pSelectedNode->SetMaterialConstants(GetMaterial());
			ResetMaterialDirty();
		}*/
	}
private:
	dx::XMMATRIX GetTransform() const noxnd
	{
		assert(pSelectedNode != nullptr);
		const auto& transform = transforms.at(pSelectedNode->GetId()).tranformParams;
		return
			dx::XMMatrixRotationRollPitchYaw(transform.roll, transform.pitch, transform.yaw) *
			dx::XMMatrixTranslation(transform.x, transform.y, transform.z);
	}

	const Dcb::Buffer& GetMaterial() const noxnd
	{
		assert(pSelectedNode != nullptr);
		const auto& mat = transforms.at(pSelectedNode->GetId()).materialCbuf;
		assert(mat);
		return *mat;
	}
	bool TransformDirty() const noxnd
	{
		return pSelectedNode && transforms.at(pSelectedNode->GetId()).transformParamsDirty;
	}
	void ResetTransformDirty() noxnd
	{
		transforms.at(pSelectedNode->GetId()).transformParamsDirty = false;
	}
	bool MaterialDirty() const noxnd
	{
		return pSelectedNode && transforms.at(pSelectedNode->GetId()).materialCbufDirty;
	}
	void ResetMaterialDirty() noxnd
	{
		transforms.at(pSelectedNode->GetId()).materialCbufDirty = false;
	}
	bool IsDirty() const noxnd
	{
		return TransformDirty() || MaterialDirty();
	}
private:
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
	struct NodeData
	{
		TransformParameters tranformParams;
		bool transformParamsDirty;
		std::optional<Dcb::Buffer> materialCbuf;
		bool materialCbufDirty;
	};
	std::unordered_map<int, NodeData> transforms;
	
};

// Model
Model::Model(Graphics& gfx, const std::string& pathString, const float scale)
	:
	pWindow(std::make_unique<ModelWindow>())
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(pathString.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	if (pScene == nullptr)
	{
		throw ModelException(__LINE__, __FILE__, imp.GetErrorString());
	}

	// store all meshes
	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		meshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i], pScene->mMaterials, pathString, scale));
	}

	// recursively calls all children
	int nextId = 0;
	pRoot = ParseNode(nextId, *pScene->mRootNode);
}

Model::~Model() noexcept
{}

void Model::Submit(FrameCommander& frame) const noxnd
{
	// I'm still not happy about updating parameters (i.e. mutating a bindable GPU state
	// which is part of a mesh which is part of a node which is part of the model that is
	// const in this call) Can probably do this elsewhere
	pWindow->ApplyParameters();
	pRoot->Submit(frame, dx::XMMatrixIdentity());
}

void Model::ShowWindow(Graphics& gfx, const char* windowName) noexcept
{
	pWindow->Show(gfx, windowName, *pRoot);
}

void Model::SetRootTransform(DirectX::FXMMATRIX tf) noexcept
{
	pRoot->SetAppliedTransform(tf);
}


std::unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials, const std::filesystem::path& path, float scale)
{
	/*
	using namespace std::string_literals;
	using DynamicVertex::VertexLayout;
	using namespace GPipeline;

	const auto rootPath = path.parent_path().string() + "\\";

	bool hasSpecularMap = false;
	bool hasAlphaGloss = false;
	bool hasAlphaDiffuse = false;
	bool hasNormalMap = false;
	bool hasDiffuseMap = false;
	//bool celShading = true;
	
	std::vector<std::shared_ptr<Bindable>> bindablePtrs;

	float shininess = 2.0f;
	dx::XMFLOAT4 specularColor = { 0.18f,0.18f,0.18f,1.0f };
	dx::XMFLOAT4 diffuseColor = { 0.45f,0.45f,0.85f,1.0f };

	// load meshes
	if (mesh.mMaterialIndex >= 0)
	{
		auto& material = *pMaterials[mesh.mMaterialIndex];
		aiString texFileName;

		if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
		{
			auto tex = Texture::Resolve(gfx, rootPath + texFileName.C_Str());
			hasAlphaDiffuse = tex->HasAlpha();
			bindablePtrs.push_back(std::move(tex));
			hasDiffuseMap = true;
		}
		else
		{
			// if we don't have textures, we load diffuse from material file
			material.Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor3D&>(diffuseColor));
		}

		// if textures are loaded successfully
		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
		{
			auto tex = Texture::Resolve(gfx, rootPath + texFileName.C_Str(), 1);
			hasAlphaGloss = tex->HasAlpha();
			bindablePtrs.push_back(std::move(tex));
			hasSpecularMap = true;
		}
		else
		{
			material.Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor3D&>(specularColor));
		}

		if (!hasAlphaGloss)
		{
			// load the shineness from the model file if exists
			material.Get(AI_MATKEY_SHININESS, shininess);
		}
		// get the tangent normals
		if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
		{
			auto tex = Texture::Resolve(gfx, rootPath + texFileName.C_Str(), 2);
			hasAlphaGloss = tex->HasAlpha();
			bindablePtrs.push_back(std::move(tex));
			hasNormalMap = true;
		}
		/*
		if (celShading)
		{
			auto tex = Texture::Resolve(gfx, "Images\\toon2.jpg", 3);
			bindablePtrs.push_back(std::move(tex));
		}
		
		if (hasDiffuseMap || hasSpecularMap || hasNormalMap)
		{
			bindablePtrs.push_back(GPipeline::Sampler::Resolve(gfx));
		}
	}

	const auto meshTag = path.string() + "%" + mesh.mName.C_Str();


	if (hasDiffuseMap && hasNormalMap && hasSpecularMap)
	{
		DynamicVertex::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Tangent)
			.Append(VertexLayout::Bitangent)
			.Append(VertexLayout::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mTangents[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mBitangents[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

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

		bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = VertexShader::Resolve(gfx, "PhongVSNormalMap.cso");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));
		// use different shaders with or without specular map

		bindablePtrs.push_back(PixelShader::Resolve(gfx,
			hasAlphaDiffuse ? "PhongPSSpecNormMask.cso" : "PhongPSSpecNormalMap.cso"
		));

		bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

		Dcb::RawLayout lay;
		lay.Add<Dcb::Bool>("normalMapEnabled");
		lay.Add<Dcb::Bool>("specularMapEnabled");
		lay.Add<Dcb::Bool>("hasGlossMap");
		lay.Add<Dcb::Float>("specularPower");
		lay.Add<Dcb::Float3>("specularColor");
		lay.Add<Dcb::Float>("specularMapWeight");

		auto buf = Dcb::Buffer(std::move(lay));
		buf["normalMapEnabled"] = true;
		buf["specularMapEnabled"] = true;
		buf["hasGlossMap"] = hasAlphaGloss;
		buf["specularPower"] = shininess;
		buf["specularColor"] = dx::XMFLOAT3{ 0.75f,0.75f,0.75f };
		buf["specularMapWeight"] = 0.671f;

		bindablePtrs.push_back(std::make_shared<CachingPixelConstantBufferEX>(gfx, buf, 1u));
	}
	else if (hasDiffuseMap && hasNormalMap)
	{
		DynamicVertex::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Tangent)
			.Append(VertexLayout::Bitangent)
			.Append(VertexLayout::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mTangents[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mBitangents[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

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

		bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = VertexShader::Resolve(gfx, "PhongVSNormalMap.cso");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPSNormalMap.cso"));

		bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

		Dcb::RawLayout layout;
		layout.Add<Dcb::Float>("specularIntensity");
		layout.Add<Dcb::Float>("specularPower");
		layout.Add<Dcb::Bool>("normalMapEnabled");

		auto cbuf = Dcb::Buffer(std::move(layout));
		cbuf["specularIntensity"] = (specularColor.x + specularColor.y + specularColor.z) / 3.0f;
		cbuf["specularPower"] = shininess;
		cbuf["normalMapEnabled"] = true;

		bindablePtrs.push_back(std::make_shared<CachingPixelConstantBufferEX>(gfx, cbuf, 1u));
	}
	else if (hasDiffuseMap && !hasNormalMap && hasSpecularMap)
	{
		DynamicVertex::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

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

		bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = VertexShader::Resolve(gfx, "PhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPSSpec.cso"));

		bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

		Dcb::RawLayout lay;
		lay.Add<Dcb::Float>("specularPower");
		lay.Add<Dcb::Bool>("hasGloss");
		lay.Add<Dcb::Float>("specularMapWeight");

		auto buf = Dcb::Buffer(std::move(lay));
		buf["specularPower"] = shininess;
		buf["hasGloss"] = hasAlphaGloss;
		buf["specularMapWeight"] = 1.0f;

		bindablePtrs.push_back(std::make_unique<GPipeline::CachingPixelConstantBufferEX>(gfx, buf, 1u));
	}
	else if (hasDiffuseMap)
	{ 
	/*
		if (celShading)
		{
			auto tex = Texture::Resolve(gfx, "Images\\toon2.jpg", 1);
			bindablePtrs.push_back(std::move(tex));
		}
		
		DynamicVertex::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

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

		bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = VertexShader::Resolve(gfx, "PhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPS.cso"));

		bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

		Dcb::RawLayout lay;
		lay.Add<Dcb::Float>("specularIntensity");
		lay.Add<Dcb::Float>("specularPower");

		auto buf = Dcb::Buffer(std::move(lay));
		buf["specularIntensity"] = (specularColor.x + specularColor.y + specularColor.z) / 3.0f;
		buf["specularPower"] = shininess;
		buf["specularMapWeight"] = 1.0f;

		bindablePtrs.push_back(std::make_unique<GPipeline::CachingPixelConstantBufferEX>(gfx, buf, 1u));
	}
	else if (!hasDiffuseMap && !hasNormalMap && !hasSpecularMap)
	{
		DynamicVertex::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i])
			);
		}

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

		bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = VertexShader::Resolve(gfx, "PhongVSNotex.cso");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPSNotex.cso"));

		bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

		Dcb::RawLayout lay;
		lay.Add<Dcb::Float4>("materialColor");
		lay.Add<Dcb::Float4>("specularColor");
		lay.Add<Dcb::Float>("specularPower");

		auto buf = Dcb::Buffer(std::move(lay));
		buf["specularPower"] = shininess;
		buf["specularColor"] = specularColor;
		buf["materialColor"] = diffuseColor;

		bindablePtrs.push_back(std::make_unique<GPipeline::CachingPixelConstantBufferEX>(gfx, buf, 1u));
	}
	else
	{
		throw std::runtime_error("terrible combination of textures in material smh");
	}

	// all materials need a blending mode
	//bindablePtrs.push_back(Blender::Resolve(gfx, hasAlphaDiffuse));

	// anything with alpha diffuse is 2-sided IN SPONZA, need a better way
	// of signalling 2-sidedness to be more general in the future
	bindablePtrs.push_back(Rasterizer::Resolve(gfx, hasAlphaDiffuse));

	bindablePtrs.push_back(Blender::Resolve(gfx, false));
	// disable general stencil effect
	bindablePtrs.push_back(std::make_shared<Stencil>(gfx, Stencil::Mode::Off));
	// create the mesh drawable
	return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));
	*/
	return {};
}
std::unique_ptr<Node> Model::ParseNode(int& nextId, const aiNode& node) noexcept
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
	auto pNode = std::make_unique<Node>(nextId++, node.mName.C_Str(), std::move(curMeshPtrs), transform);
	for (size_t i = 0; i < node.mNumChildren; i++)
	{
		pNode->AddChild(ParseNode(nextId, *node.mChildren[i]));
	}
	return pNode;
}