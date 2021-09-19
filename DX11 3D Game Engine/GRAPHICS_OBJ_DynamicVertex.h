/**
	* @file GRAPHICS_OBJ_VertexLayout.h
	* @brief 
	*/
#pragma once
#include "SYS_CLASS_Graphics.h"
#include <vector>
//#include <DirectXMath.h>
#include <type_traits>

namespace DynamicVertex {
	struct BGRAColor
	{
		unsigned char a;
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

	/**
	 * @brief 
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
		};
		template<> struct Map<Position3D>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Position";
		};
		template<> struct Map<Texture2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Texcoord";
		};
		template<> struct Map<Normal>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Normal";
		};
		template<> struct Map<Float3Color>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Color";
		};
		template<> struct Map<Float4Color>
		{
			using SysType = DirectX::XMFLOAT4;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* semantic = "Color";
		};
		template<> struct Map<BGRAColor>
		{
			using SysType = DynamicVertex::BGRAColor;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char* semantic = "Color";
		};
		class Element
		{
		public:
			Element(VertexLayout::ElementType type, size_t offset)
				:
				type(type),
				offset(offset)
			{}
			size_t GetOffsetAfter() const noexcept(!IS_DEBUG)
			{
				return offset + Size();
			}
			size_t GetOffset() const
			{
				return offset;
			}
			size_t Size() const noexcept(!IS_DEBUG)
			{
				return SizeOf(type);
			}
			static constexpr size_t SizeOf(VertexLayout::ElementType type) noexcept(!IS_DEBUG)
			{
				switch (type)
				{
				case Position2D:
					return sizeof(Map<Position2D>::SysType);
				case Position3D:
					return sizeof(Map<Position3D>::SysType);
				case Texture2D:
					return sizeof(Map<Texture2D>::SysType);
				case Normal:
					return sizeof(Map<Normal>::SysType);
				case Float3Color:
					return sizeof(Map<Float3Color>::SysType);
				case Float4Color:
					return sizeof(Map<Float4Color>::SysType);
				case BGRAColor:
					return sizeof(Map<BGRAColor>::SysType);
				}
				assert("Invalid element type" && false);
				return 0u;
			}
			VertexLayout::ElementType GetType() const noexcept{	return type; }
			D3D11_INPUT_ELEMENT_DESC GetDesc() const noexcept(!IS_DEBUG)
			{
				switch (type)
				{
				case Position2D:
					return GenerateDesc<Position2D>(GetOffset());
				case Position3D:
					return GenerateDesc<Position3D>(GetOffset());
				case Texture2D:
					return GenerateDesc<Texture2D>(GetOffset());
				case Normal:
					return GenerateDesc<Normal>(GetOffset());
				case Float3Color:
					return GenerateDesc<Float3Color>(GetOffset());
				case Float4Color:
					return GenerateDesc<Float4Color>(GetOffset());
				case BGRAColor:
					return GenerateDesc<BGRAColor>(GetOffset());
				}
				assert("Invalid element type" && false);
				return { "INVALID",0,DXGI_FORMAT_UNKNOWN,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 };
			}
		private:
			/**
			 * @brief Generate a description of a single element for the input-assembler stage.
			 */
			template<ElementType type>
			static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDesc(size_t offset) noexcept(!IS_DEBUG)
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
		const Element& Resolve() const noexcept(!IS_DEBUG)
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
		const Element& ResolveByIndex(size_t i) const noexcept(!IS_DEBUG)
		{
			return elements[i];
		}
		VertexLayout& Append(ElementType type) noexcept(!IS_DEBUG)
		{
			elements.emplace_back(type, Size());
			return *this;
		}
		/**
		 * @brief returns the size of the vertex info in bytes
		 * @return size_t the offset of the very last element in the info std::vector<Element>
		 */
		size_t Size() const noexcept(!IS_DEBUG)
		{
			return elements.empty() ? 0u : elements.back().GetOffsetAfter();
		}
		/**
		 * @brief returns the size of the vertex info in nums of elements
		 * @return size_t the count of elements in the std::vector<Element>
		 */
		size_t GetElementCount() const noexcept
		{
			return elements.size();
		}
		/**
		 * @brief collect and assemble the layout specifications from the inner elem types
		 * @return std::vector<D3D11_INPUT_ELEMENT_DESC>
		 */
		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const noexcept(!IS_DEBUG)
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
			desc.reserve(GetElementCount());
			for (const auto& e : elements)
			{
				desc.push_back(e.GetDesc());
			}
			return desc;
		}
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
		auto& Attr() noexcept(!IS_DEBUG)
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
		void SetAttributeByIndex(size_t i, T&& val) noexcept(!IS_DEBUG)
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
		Vertex(char* pData, const VertexLayout& layout) noexcept(!IS_DEBUG)
			:
			pData(pData),
			layout(layout)
		{
			assert(pData != nullptr);
		}
	private:
		/**
		 * @brief helper to reduce code duplication and do assignable check in SetAttributeByIndex
		 */
		template<typename First, typename ...Rest>
		// enables parameter pack setting of multiple parameters by element index
		void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest) noexcept(!IS_DEBUG)
		{
			SetAttributeByIndex(i,     std::forward<First>(first));
			SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
		}
		/**
		 * @brief helper to reduce code duplication and do assignable check in SetAttributeByIndex
		 */
		template<VertexLayout::ElementType DestLayoutType, typename SrcSysType>
		void SetAttribute(char* pAttribute, SrcSysType&& val) noexcept(!IS_DEBUG)
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
		ConstVertex(const Vertex& v) noexcept(!IS_DEBUG)
			:
			vertex(v)
		{}
		template<VertexLayout::ElementType Type>
		const auto& Attr() const noexcept(!IS_DEBUG)
		{
			return const_cast<Vertex&>(vertex).Attr<Type>();
		}
	private:
		Vertex vertex;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(VertexLayout layout) noexcept(!IS_DEBUG)	:	layout(std::move(layout)){}
		const char* GetData() const noexcept(!IS_DEBUG)	{return buffer.data();}
		const VertexLayout& GetLayout() const noexcept{return layout;}
		/**
		 * @brief returns the size of the vertex buffer in vertecies
		 * @return size_t how many vertecies are stored
		 */
		size_t Size() const noexcept(!IS_DEBUG){return buffer.size() / layout.Size();}
		size_t SizeBytes() const noexcept(!IS_DEBUG){return buffer.size();}
		/**
		 * @brief adding info at the end of the buffer
		 */
		template<typename ...Params>
		void EmplaceBack(Params&&... params) noexcept(!IS_DEBUG)
		{
			assert(sizeof...(params) == layout.GetElementCount() && "Param count doesn't match number of vertex elements");
			buffer.resize(buffer.size() + layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}
		/**
		 * @brief returns the last vertex info (the proxy view)
		 */
		Vertex Back() noexcept(!IS_DEBUG)
		{
			assert(buffer.size() != 0u);
			return Vertex{ buffer.data() + buffer.size() - layout.Size(),layout };
		}
		ConstVertex Back() const noexcept(!IS_DEBUG){return const_cast<VertexBuffer*>(this)->Back();}
		Vertex Front() noexcept(!IS_DEBUG)
		{
			assert(buffer.size() != 0u);
			return Vertex{ buffer.data(),layout };
		}
		ConstVertex Front() const noexcept(!IS_DEBUG){return const_cast<VertexBuffer*>(this)->Front();}
		Vertex operator[](size_t i) noexcept(!IS_DEBUG)
		{
			assert(i < Size());
			return Vertex{ buffer.data() + layout.Size() * i,layout };
		}
		ConstVertex operator[](size_t i) const noexcept(!IS_DEBUG){return const_cast<VertexBuffer&>(*this)[i];}
	private:
		/// All info in the vertex
		std::vector<char> buffer;
		/// Layout if the info items
		VertexLayout layout;
	};
}