#pragma once
#include "GRAPHICS_OBJ_Bindable.h"
#include "GRAPHICS_OBJ_DynamicVertex.h"
namespace GPipeline
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout(Graphics& gfx,
			DynamicVertex::VertexLayout layout,
			ID3DBlob* pVertexShaderBytecode);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<InputLayout> Resolve(Graphics& gfx,
			const DynamicVertex::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode);

		static std::string GenerateUID(const DynamicVertex::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode = nullptr);
		std::string GetUID() const noexcept override;
	protected:
		DynamicVertex::VertexLayout layout;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	};
}