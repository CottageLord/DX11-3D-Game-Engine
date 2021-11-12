/**
	* @file GRAPHICS_OBJ_VertexLayout.h
	* @brief 
	*/
#pragma once
#include "SYS_SET_ConditionalNoexcept.h"
#include "SYS_CLASS_Graphics.h"
#include "GRAPHICS_SET_Color.h"

#include <vector>
//#include <DirectXMath.h>
#include <type_traits>

namespace DynamicVertex 
{
	/**
	 * @brief a vertex buffer class that could accept various layouts (how data is stored in model file)
	 */
	class VertexLayout
	{
	public:
		enum ElementType
		{
			Position2D,
			Position3D,
			Texture2D,
			Normal,
			Float3Color,
			Float4Color,
			BGRAColor,
			Count
		};
		/**
		 * @brief A compile time look-up table that maps enum types to its info
		 */
		template<ElementType> struct Map;
		// using template specialzation to differenciate maps
		template<> struct Map<Position2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Position"; // for shader recongnition
			static constexpr const char* code = "P2";
		};
		template<> struct Map<Position3D>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Position";
			static constexpr const char* code = "P3";
		};
		template<> struct Map<Texture2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Texcoord";
			static constexpr const char* code = "T2";
		};
		template<> struct Map<Normal>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Normal";
			static constexpr const char* code = "N";
		};
		template<> struct Map<Float3Color>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Color";
			static constexpr const char* code = "C3";
		};
		template<> struct Map<Float4Color>
		{
			using SysType = DirectX::XMFLOAT4;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* semantic = "Color";
			static constexpr const char* code = "C4";
		};
		template<> struct Map<BGRAColor>
		{
			using SysType = ::BGRAColor; // pretended :: means global namespace
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char* semantic = "Color";
			static constexpr const char* code = "C8";
		};
		class Element
		{
		public:
			Element(ElementType type, size_t offset);
			size_t GetOffsetAfter() const noxnd;
			size_t GetOffset() const;
			size_t Size() const noxnd;
			static constexpr size_t SizeOf(ElementType type) noxnd;
			ElementType GetType() const noexcept;
			D3D11_INPUT_ELEMENT_DESC GetDesc() const noxnd;
			const char* GetCode() const noexcept;
		private:
			/**
			 * @brief Generate a description of a single element for the input-assembler stage.
			 */
			template<ElementType type>
			static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDesc(size_t offset) noexcept
			{
				return { 
					Map<type>::semantic,			// SemanticName: "Position"
					0,								// SemanticIndex
					Map<type>::dxgiFormat,			// Format: DXGI_FORMAT_R32G32B32_FLOAT
					0,								// InputSlot: 0 for interleaved vertex data
					(UINT)offset,					// AlignedByteOffset from the start of the vertex
					D3D11_INPUT_PER_VERTEX_DATA,	// InputSlotClass
					0 };							// InstanceDataStepRate
			}
		private:
			VertexLayout::ElementType type;
			size_t offset; // position in the vertex info
		};
	public:
		/**
		 * @brief Retrieve a certain type of data from the vertex.
		 */
		template<ElementType Type>
		const Element& Resolve() const noxnd
		{
			for (auto& e : elements)
			{
				if (e.GetType() == Type)
				{
					return e;
				}
			}
			assert("Could not resolve element type" && false);
			return elements.front();
		}
		/**
		 * @brief Retrieve a certain data from the vertex given its position.
		 * @param size_t i the position (offset) of the desired data member
		 */
		const Element& ResolveByIndex(size_t i) const noxnd;
		VertexLayout& Append(ElementType type) noxnd;
		/**
		 * @brief returns the size of the vertex info in bytes
		 * @return size_t the offset of the very last element in the info std::vector<Element>
		 */
		size_t Size() const noxnd;
		/**
		 * @brief returns the count of elements vector
		 * @return size_t the count of elements in the std::vector<Element>
		 */
		size_t GetElementCount() const noexcept;
		/**
		 * @brief collect and assemble the layout specifications from the inner elem types
		 * @return std::vector<D3D11_INPUT_ELEMENT_DESC>
		 */
		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const noxnd;
		std::string GetCode() const noxnd;
	private:
		std::vector<Element> elements;
	};
	/**
	 * @brief not a concrete class holding vertex data, but a proxy view into the real data buffer
	 */
	class Vertex
	{
		friend class VertexBuffer;
	public:
		/**
		 * @brief 
		 */
		template<VertexLayout::ElementType Type>
		auto& Attr() noxnd
		{
			// Get the actual attribute data
			auto pAttribute = pData + layout.Resolve<Type>().GetOffset();
			// Look up the corresponding real Dx type and reinterpret
			return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute);
		}
		/**
		 * @brief set a certain attribute by its position in the vertex data using perfect forwarding (universal ref)
		 */
		template<typename T>
		void SetAttributeByIndex(size_t i, T&& val) noxnd
		{
			const auto& element = layout.ResolveByIndex(i);
			auto pAttribute = pData + element.GetOffset();
			switch (element.GetType())
			{
			case VertexLayout::Position2D:
				SetAttribute<VertexLayout::Position2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Position3D:
				SetAttribute<VertexLayout::Position3D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Texture2D:
				SetAttribute<VertexLayout::Texture2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Normal:
				SetAttribute<VertexLayout::Normal>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float3Color:
				SetAttribute<VertexLayout::Float3Color>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float4Color:
				SetAttribute<VertexLayout::Float4Color>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::BGRAColor:
				SetAttribute<VertexLayout::BGRAColor>(pAttribute, std::forward<T>(val));
				break;
			default:
				assert("Bad element type" && false);
			}
		}
	protected:
		Vertex(char* pData, const VertexLayout& layout) noxnd;
	private:
		/**
		 * @brief helper to reduce code duplication and do assignable check in SetAttributeByIndex
		 */
		template<typename First, typename ...Rest>
		// enables parameter pack setting of multiple parameters by element index
		void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest) noxnd
		{
			SetAttributeByIndex(i,     std::forward<First>(first));
			SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
		}
		/**
		 * @brief helper to reduce code duplication and do assignable check in SetAttributeByIndex
		 */
		template<VertexLayout::ElementType DestLayoutType, typename SrcSysType>
		void SetAttribute(char* pAttribute, SrcSysType&& val) noxnd
		{
			// Get the Dx type (like DXFLOAT3) according to enum info
			using DestSysType = typename VertexLayout::Map<DestLayoutType>::SysType;

			if constexpr (std::is_assignable<DestSysType, SrcSysType>::value)
			{
				*reinterpret_cast<DestSysType*>(pAttribute) = val;
			}
			else
			{
				assert("Parameter attribute type mismatch" && false);
			}
		}
	private:
		char* pData = nullptr;
		const VertexLayout& layout;
	};
	/**
	 * @brief
	 */
	class ConstVertex
	{
	public:
		ConstVertex(const Vertex& v) noxnd;
		template<VertexLayout::ElementType Type>
		const auto& Attr() const noxnd
		{
			return const_cast<Vertex&>(vertex).Attr<Type>();
		}
	private:
		Vertex vertex;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(VertexLayout layout) noxnd;
		const char* GetData() const noxnd;
		const VertexLayout& GetLayout() const noexcept;
		/**
		 * @brief returns the size of the vertex buffer in vertecies
		 * @return size_t how many vertecies are stored
		 */
		size_t Size() const noxnd;
		size_t SizeBytes() const noxnd;
		/**
		 * @brief adding info at the end of the buffer
		 */
		template<typename ...Params>
		void EmplaceBack(Params&&... params) noxnd
		{
			assert(sizeof...(params) == layout.GetElementCount() && "Param count doesn't match number of vertex elements");
			buffer.resize(buffer.size() + layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}
		/**
		 * @brief returns the last vertex info (the proxy view)
		 */
		Vertex Back() noxnd;
		ConstVertex Back() const noxnd;
		Vertex Front() noxnd;
		ConstVertex Front() const noxnd;
		Vertex operator[](size_t i) noxnd;
		ConstVertex operator[](size_t i) const noxnd;
	private:
		/// All info in the vertex
		std::vector<char> buffer;
		/// Layout if the info items
		VertexLayout layout;
	};
}