#pragma once
#include <array>
#include "SYS_CLASS_Graphics.h"
#include "SYS_HELP_PerfLog.h"
#include "GRAPHICS_SET_BindableCommon.h"
#include "GRAPHICS_OBJ_DynamicConstant.h"
#include "GRAPHICS_OBJ_DepthStencil.h"
#include "GRAPHICS_OBJ_RenderTarget.h"
#include "GRAPHICS_JOB_Job.h"
#include "GRAPHICS_JOB_Pass.h"
#include <array>

/**
 * @brief manages all render passes, specifying the operations for each pass
 */
class FrameCommander
{
public:
	FrameCommander(Graphics& gfx)
		:
		ds(gfx, gfx.GetWidth(), gfx.GetHeight()),
		rt(gfx, gfx.GetWidth(), gfx.GetHeight())
	{
		namespace dx = DirectX;

		// setup fullscreen geometry
		DynamicVertex::VertexLayout lay;
		lay.Append(DynamicVertex::VertexLayout::Position2D);
		DynamicVertex::VertexBuffer bufFull{ lay };
		bufFull.EmplaceBack(dx::XMFLOAT2{ -1,1 });
		bufFull.EmplaceBack(dx::XMFLOAT2{ 1,1 });
		bufFull.EmplaceBack(dx::XMFLOAT2{ -1,-1 });
		bufFull.EmplaceBack(dx::XMFLOAT2{ 1,-1 });
		pVbFull = GPipeline::VertexBuffer::Resolve(gfx, "$Full", std::move(bufFull));
		std::vector<unsigned short> indices = { 0,1,2,1,3,2 };
		pIbFull = GPipeline::IndexBuffer::Resolve(gfx, "$Full", std::move(indices));

		// set the pixel constant buffer
		/*
		Dcb::RawLayout pscLayout;
		pscLayout.Add<Dcb::Float2>("TextureSizeWH");
		Dcb::Buffer buf{ std::move(pscLayout) };
		buf["TextureSizeWH"].SetIfExists(dx::XMFLOAT2{1024, 1024});
		*/
		// setup fullscreen shaders
		pPsFull = GPipeline::PixelShader::Resolve(gfx, "Funk_PS.cso");
		pVsFull = GPipeline::VertexShader::Resolve(gfx, "Fullscreen_VS.cso");
		pLayoutFull = GPipeline::InputLayout::Resolve(gfx, lay, pVsFull->GetBytecode());
	}
	void Accept(Job job, size_t target) noexcept
	{
		passes[target].Accept(job);
	}
	void Execute(Graphics& gfx) const noxnd
	{
		using namespace GPipeline; 
		// normally this would be a loop with each pass defining it setup / etc.
		// and later on it would be a complex graph with parallel execution contingent
		// on input / output requirements
		// setup render target used for normal passes
		ds.Clear(gfx);
		rt.BindAsTarget(gfx, ds);
		// main phong lighting pass
		Stencil::Resolve(gfx, Stencil::Mode::Off)->Bind(gfx);
		passes[0].Execute(gfx);
		// outline masking pass
		Stencil::Resolve(gfx, Stencil::Mode::Write)->Bind(gfx);
		NullPixelShader::Resolve(gfx)->Bind(gfx); // only update stencil, no rendering
		passes[1].Execute(gfx);
		// outline drawing pass
		Stencil::Resolve(gfx, Stencil::Mode::Mask)->Bind(gfx);
		passes[2].Execute(gfx);
		// fullscreen funky pass
		gfx.BindSwapBuffer();
		rt.BindAsTexture(gfx, 0);
		pVbFull->Bind(gfx);
		pIbFull->Bind(gfx);
		pVsFull->Bind(gfx);
		pPsFull->Bind(gfx);
		pLayoutFull->Bind(gfx);
		gfx.DrawIndexed(pIbFull->GetCount());
	}
	void Reset() noexcept
	{
		for (auto& p : passes)
		{
			p.Reset();
		}
	}
private:
	std::array<Pass, 3> passes;
	DepthStencil ds;
	RenderTarget rt;
	std::shared_ptr<GPipeline::VertexBuffer> pVbFull;
	std::shared_ptr<GPipeline::IndexBuffer> pIbFull;
	std::shared_ptr<GPipeline::VertexShader> pVsFull;
	std::shared_ptr<GPipeline::PixelShader> pPsFull;
	std::shared_ptr<GPipeline::InputLayout> pLayoutFull;
};