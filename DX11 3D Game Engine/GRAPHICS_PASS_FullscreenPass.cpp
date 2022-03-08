#include "GRAPHICS_PASS_FullscreenPass.h"
#include "GRAPHICS_SET_BindableCommon.h"


namespace Rgph
{
	namespace dx = DirectX;

	FullscreenPass::FullscreenPass(const std::string name, Graphics& gfx) noxnd
		:
	BindingPass(std::move(name))
	{
		// setup fullscreen geometry
		DynamicVertex::VertexLayout lay;
		lay.Append(DynamicVertex::VertexLayout::Position2D);
		DynamicVertex::VertexBuffer bufFull{ lay };
		bufFull.EmplaceBack(dx::XMFLOAT2{ -1,1 });
		bufFull.EmplaceBack(dx::XMFLOAT2{ 1,1 });
		bufFull.EmplaceBack(dx::XMFLOAT2{ -1,-1 });
		bufFull.EmplaceBack(dx::XMFLOAT2{ 1,-1 });
		AddBind(GPipeline::VertexBuffer::Resolve(gfx, "$Full", std::move(bufFull)));
		std::vector<unsigned short> indices = { 0,1,2,1,3,2 };
		AddBind(GPipeline::IndexBuffer::Resolve(gfx, "$Full", std::move(indices)));
		// setup other common fullscreen bindables
		auto vs = GPipeline::VertexShader::Resolve(gfx, "Fullscreen_VS.cso");
		AddBind(GPipeline::InputLayout::Resolve(gfx, lay, vs->GetBytecode()));
		AddBind(std::move(vs));
		AddBind(GPipeline::Topology::Resolve(gfx));
		AddBind(GPipeline::Rasterizer::Resolve(gfx, false));
	}

	void FullscreenPass::Execute(Graphics& gfx) const noxnd
	{
		BindAll(gfx);
		gfx.DrawIndexed(6u);
	}
}