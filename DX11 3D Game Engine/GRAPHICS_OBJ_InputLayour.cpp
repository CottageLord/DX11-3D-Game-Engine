#include "GRAPHICS_OBJ_InputLayout.h"

InputLayout::InputLayout(Graphics& gfx,
	/*
	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	*/
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
	ID3DBlob* pVertexShaderBytecode)
{
	GET_INFO_MAN(gfx);

	GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
		/*
		An array of the input-assembler stage input data types; 
		each type is described by an element description 
		*/
		layout.data(), (UINT)layout.size(),
		/*
		A pointer to the compiled shader. The compiled shader code contains a input signature 
		which is validated against the array of elements.
		*/
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout
	));
}

void InputLayout::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}