#pragma once
#include "GRAPHICS_OBJ_Bindable.h"
#include "GRAPHICS_OBJ_DynamicVertex.h"

namespace GPipeline
{
	class VertexShader;

	class InputLayout : public Bindable
	{
	public:
		InputLayout(Graphics& gfx,
			DynamicVertex::VertexLayout layout,
			const VertexShader& vs);
		void Bind(Graphics& gfx) noxnd override;
		const DynamicVertex::VertexLayout GetLayout() const noexcept;
		static std::shared_ptr<InputLayout> Resolve(Graphics& gfx,
			const DynamicVertex::VertexLayout& layout, const VertexShader& vs);
		static std::string GenerateUID(const DynamicVertex::VertexLayout& layout, const VertexShader& vs);
		std::string GetUID() const noexcept override;
	protected:
		std::string vertexShaderUID;
		DynamicVertex::VertexLayout layout;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	};
}