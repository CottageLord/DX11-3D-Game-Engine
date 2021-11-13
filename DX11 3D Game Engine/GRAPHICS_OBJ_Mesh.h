#pragma once
#include "SYS_SET_ConditionalNoexcept.h"
#include "GRAPHICS_SET_BindableCommon.h"
//#include "GRAPHICS_OBJ_StaticDrawInfo.h"
#include "GRAPHICS_OBJ_Drawable.h"
#include "GRAPHICS_OBJ_DynamicVertex.h"
#include "GRAPHICS_BUF_ConstantBuffers.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <type_traits>
#include "imgui/imgui.h"

#include <optional>
#include <filesystem>

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
	struct PSMaterialConstantFullmonte
	{
		BOOL  normalMapEnabled = TRUE;
		BOOL  specularMapEnabled = TRUE;
		BOOL  hasGlossMap = FALSE;
		float specularPower = 3.1f;
		DirectX::XMFLOAT3 specularColor = { 0.75f,0.75f,0.75f };
		float specularMapWeight = 0.671f;
	};
	struct PSMaterialConstantNotex
	{
		DirectX::XMFLOAT4 materialColor = { 0.447970f,0.327254f,0.176283f,1.0f };
		DirectX::XMFLOAT4 specularColor = { 0.65f,0.65f,0.65f,1.0f };
		float specularPower = 120.0f;
		float padding[3];
	};
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
	// a tester
	template<class T>
	bool ControlMeDaddy(Graphics& gfx, T& c)
	{
		if (meshPtrs.empty())
		{
			return false;
		}

		if constexpr (std::is_same<T, PSMaterialConstantFullmonte>::value)
		{
			if (auto pcb = meshPtrs.front()->QueryBindable<GPipeline::PixelConstantBuffer<T>>())
			{
				ImGui::Text("Material");

				bool normalMapEnabled = (bool)c.normalMapEnabled;
				ImGui::Checkbox("Norm Map", &normalMapEnabled);
				c.normalMapEnabled = normalMapEnabled ? TRUE : FALSE;

				bool specularMapEnabled = (bool)c.specularMapEnabled;
				ImGui::Checkbox("Spec Map", &specularMapEnabled);
				c.specularMapEnabled = specularMapEnabled ? TRUE : FALSE;

				bool hasGlossMap = (bool)c.hasGlossMap;
				ImGui::Checkbox("Gloss Alpha", &hasGlossMap);
				c.hasGlossMap = hasGlossMap ? TRUE : FALSE;

				ImGui::SliderFloat("Spec Weight", &c.specularMapWeight, 0.0f, 2.0f);

				ImGui::SliderFloat("Spec Pow", &c.specularPower, 0.0f, 1000.0f, "%f", 5.0f);

				ImGui::ColorPicker3("Spec Color", reinterpret_cast<float*>(&c.specularColor));

				pcb->Update(gfx, c);
				return true;
			}
		}
		else if constexpr (std::is_same<T, PSMaterialConstantNotex>::value)
		{
			if (auto pcb = meshPtrs.front()->QueryBindable<GPipeline::PixelConstantBuffer<T>>())
			{
				ImGui::Text("Material");

				ImGui::ColorPicker3("Spec Color", reinterpret_cast<float*>(&c.specularColor));

				ImGui::SliderFloat("Spec Pow", &c.specularPower, 0.0f, 1000.0f, "%f", 5.0f);

				ImGui::ColorPicker3("Diff Color", reinterpret_cast<float*>(&c.materialColor));

				pcb->Update(gfx, c);
				return true;
			}
		}
		return false;
	}
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
	Model(Graphics& gfx, const std::string& pathString, float scale = 1.0f);
	~Model() noexcept;
	void SetRootTransform(DirectX::FXMMATRIX tf) noexcept;
	void Draw(Graphics& gfx) const noxnd;
	void ShowWindow(Graphics& gfx, const char* windowName = nullptr) noexcept;
private:
	/**
	* @brief parse and store (vertex/index/normal/tc...) data from the mesh file
	* @param mesh A reference to the model itself
	* @param pMaterials An array of ptrs to materials
	*/
	static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials, const std::filesystem::path& path, float scale);
	/**
	* @brief parse the node structure from the assimp node input
	*/
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node) noexcept;
private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	std::unique_ptr<class ModelWindow> pWindow;
}; 
