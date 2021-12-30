#pragma once
#include "GRAPHICS_OBJ_Bindable.h"
#include "SYS_SET_GraphicsThrowMacros.h"
#include "GRAPHICS_OBJ_DynamicVertex.h"

namespace GPipeline
{
	class VertexBuffer : public Bindable
	{
	public:
		
		VertexBuffer(Graphics& gfx, const std::string& tag, const DynamicVertex::VertexBuffer& vbuf);
		VertexBuffer(Graphics& gfx, const DynamicVertex::VertexBuffer& vbuf);
		void Bind(Graphics& gfx) noexcept override;
		const DynamicVertex::VertexLayout& GetLayout() const noexcept;
		static std::shared_ptr<VertexBuffer> Resolve(Graphics& gfx, const std::string& tag,
			const DynamicVertex::VertexBuffer& vbuf);
		/**
		* @brief Consider the size of the VBuf, we allows an input tag as uid source
		*/
		template<typename...Ignore>
		static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
		{
			return GenerateUID_(tag);
		}
		std::string GetUID() const noexcept override;
		/*
		Templates get inlined by default. Therefore they are not visible to the linker.
		And the file that contains main() cannot see the definition of the templates.
		We usually put the implementation in header file or in a .tpp file
		*/
		/*
		template<class V>
		VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
			:
			stride(sizeof(V))
		{
			GET_INFO_MAN(gfx);

			D3D11_BUFFER_DESC bd = {};
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0u;
			bd.MiscFlags = 0u;
			bd.ByteWidth = UINT(sizeof(V) * vertices.size());
			bd.StructureByteStride = sizeof(V);
			D3D11_SUBRESOURCE_DATA sd = {};
			sd.pSysMem = vertices.data();
			GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));
		}*/
	private:
		static std::string GenerateUID_(const std::string& tag);
	protected:
		std::string tag;
		UINT stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
		DynamicVertex::VertexLayout layout;
	};
}