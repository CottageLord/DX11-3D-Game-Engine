#pragma once
#include "GRAPHICS_OBJ_Bindable.h"
#include "SYS_SET_GraphicsThrowMacros.h"

class VertexBuffer : public Bindable
{
public:
	/*
	Templates get inlined by default. Therefore they are not visible to the linker. 
	And the file that contains main() cannot see the definition of the templates.
	We usually put the implementation in header file or in a .tpp file
	*/
	template<class V>
	VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
		:
		stride( sizeof( V ) )
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
	}
	void Bind(Graphics& gfx) noexcept override;
protected:
	UINT stride = 0;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};