#include "GRAPHICS_BUF_VertexBuffer.h"
#include "GRAPHICS_OBJ_BindablePool.h"

namespace GPipeline
{
	/**
	* @brief Constructor for vBuf that is not in the BindablePool system
	*/
	VertexBuffer::VertexBuffer(Graphics& gfx, const DynamicVertex::VertexBuffer& vbuf)
		:
		VertexBuffer(gfx, "?", vbuf)
	{}

	VertexBuffer::VertexBuffer(Graphics& gfx, const std::string& tag, const DynamicVertex::VertexBuffer& vbuf)
		:
		stride((UINT)vbuf.GetLayout().Size()),
		tag(tag),
		layout(vbuf.GetLayout())
	{
		GET_INFO_MAN(gfx);

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(vbuf.SizeBytes());
		bd.StructureByteStride = stride;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vbuf.GetData();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));
	}

	void VertexBuffer::Bind(Graphics& gfx) noxnd
	{
		const UINT offset = 0u;
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset));
	}

	const DynamicVertex::VertexLayout& VertexBuffer::GetLayout() const noexcept
	{
		return layout;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Resolve(Graphics& gfx, const std::string& tag,
		const DynamicVertex::VertexBuffer& vbuf)
	{
		// cannot use Bindable system when constructed without tag
		assert(tag != "?");
		return BindablePool::Resolve<VertexBuffer>(gfx, tag, vbuf);
	}
	std::string VertexBuffer::GenerateUID_(const std::string& tag)
	{
		using namespace std::string_literals;
		return typeid(VertexBuffer).name() + "#"s + tag;
	}
	std::string VertexBuffer::GetUID() const noexcept
	{
		return GenerateUID(tag);
	}
}