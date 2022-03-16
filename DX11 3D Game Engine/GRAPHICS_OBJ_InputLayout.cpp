#include "GRAPHICS_OBJ_InputLayout.h"
#include "SYS_SET_GraphicsThrowMacros.h"
#include "GRAPHICS_OBJ_BindablePool.h"
#include "GRAPHICS_OBJ_VertexShader.h"

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
		const VertexShader& vs)
		:
		layout(std::move(layout_in))
	{
		GET_INFO_MAN(gfx);

		const auto d3dLayout = layout.GetD3DLayout();

		const auto pBytecode = vs.GetBytecode();

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
			pBytecode->GetBufferPointer(),
			pBytecode->GetBufferSize(),
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
		const DynamicVertex::VertexLayout& layout, const VertexShader& vs)
	{
		return BindablePool::Resolve<InputLayout>(gfx, layout, vs);
	}
	std::string InputLayout::GenerateUID(const DynamicVertex::VertexLayout& layout, const VertexShader& vs)
	{
		using namespace std::string_literals;
		return typeid(InputLayout).name() + "#"s + layout.GetCode() + "#"s + vs.GetUID();
	}
	std::string InputLayout::GetUID() const noexcept
	{
		using namespace std::string_literals;
		return typeid(InputLayout).name() + "#"s + layout.GetCode() + "#"s + vertexShaderUID;
	}
}