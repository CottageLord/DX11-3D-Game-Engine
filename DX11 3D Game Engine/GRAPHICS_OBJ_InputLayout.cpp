#include "GRAPHICS_OBJ_InputLayout.h"
#include "SYS_SET_GraphicsThrowMacros.h"
#include "GRAPHICS_OBJ_BindablePool.h"

namespace GPipeline
{
	InputLayout::InputLayout(Graphics& gfx,
		/*
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		*/
		DynamicVertex::VertexLayout layout_in,
		ID3DBlob* pVertexShaderBytecode)
		:
		layout(std::move(layout_in))
	{
		GET_INFO_MAN(gfx);

		const auto d3dLayout = layout.GetD3DLayout();

		GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
			/*
			An array of the input-assembler stage input data types;
			each type is described by an element description
			*/
			d3dLayout.data(), (UINT)d3dLayout.size(),
			/*
			A pointer to the compiled shader. The compiled shader code contains a input signature
			which is validated against the array of elements.
			*/
			pVertexShaderBytecode->GetBufferPointer(),
			pVertexShaderBytecode->GetBufferSize(),
			&pInputLayout
		));
	}

	const DynamicVertex::VertexLayout InputLayout::GetLayout() const noexcept
	{
		return layout;
	}

	void InputLayout::Bind(Graphics& gfx) noxnd
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetInputLayout(pInputLayout.Get()));
	}

	std::shared_ptr<InputLayout> InputLayout::Resolve(Graphics& gfx,
		const DynamicVertex::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode)
	{
		return BindablePool::Resolve<InputLayout>(gfx, layout, pVertexShaderBytecode);
	}
	std::string InputLayout::GenerateUID(const DynamicVertex::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode)
	{
		using namespace std::string_literals;
		// concatenate all elements' codes to form a unique id
		return typeid(InputLayout).name() + "#"s + layout.GetCode();
	}
	std::string InputLayout::GetUID() const noexcept
	{
		return GenerateUID(layout);
	}
}