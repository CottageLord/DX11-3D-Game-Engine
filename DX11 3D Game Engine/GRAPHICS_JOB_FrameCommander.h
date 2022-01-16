#pragma once
#include <array>
#include "SYS_CLASS_Graphics.h"
#include "SYS_HELP_PerfLog.h"
#include "GRAPHICS_SET_BindableCommon.h"
#include "GRAPHICS_OBJ_DynamicConstant.h"
#include "GRAPHICS_OBJ_DepthStencil.h"
#include "GRAPHICS_OBJ_RenderTarget.h"
#include "GRAPHICS_OBJ_BlurPack.h"
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
		rt1(gfx, gfx.GetWidth() / 2, gfx.GetHeight() / 2),
		rt2(gfx, gfx.GetWidth() / 2, gfx.GetHeight() / 2),
		blur(gfx, 7, 2.6f, "BlurOutline_PS.cso")
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

		// setup fullscreen shaders
		pVsFull = GPipeline::VertexShader::Resolve(gfx, "Fullscreen_VS.cso");
		pLayoutFull = GPipeline::InputLayout::Resolve(gfx, lay, pVsFull->GetBytecode());
		pSamplerFullPoint = GPipeline::Sampler::Resolve(gfx, false, true);
		pSamplerFullBilin = GPipeline::Sampler::Resolve(gfx, true, true);
		pBlenderMerge = GPipeline::Blender::Resolve(gfx, true);
	}
	void Accept(Job job, size_t target) noexcept
	{
		passes[target].Accept(job);
	}
	void Execute(Graphics& gfx) noxnd
	{
		using namespace GPipeline;
		// normally this would be a loop with each pass defining its setup / etc.
		// and later on it would be a complex graph with parallel execution contingent
		// on input / output requirements

		// setup render target used for normal passes
		ds.Clear(gfx);
		rt1.Clear(gfx);
		gfx.BindSwapBuffer(ds);
		// main phong lighting pass
		Blender::Resolve(gfx, false)->Bind(gfx);
		Stencil::Resolve(gfx, Stencil::Mode::Off)->Bind(gfx);
		passes[0].Execute(gfx);
		// outline masking pass
		Stencil::Resolve(gfx, Stencil::Mode::Write)->Bind(gfx);
		NullPixelShader::Resolve(gfx)->Bind(gfx);
		passes[1].Execute(gfx);
		// outline drawing pass
		rt1.BindAsTarget(gfx);
		Stencil::Resolve(gfx, Stencil::Mode::Off)->Bind(gfx);
		passes[2].Execute(gfx);
		// fullscreen blur horizontal-pass
		rt2.BindAsTarget(gfx); // output
		rt1.BindAsTexture(gfx, 0); // input
		pVbFull->Bind(gfx);
		pIbFull->Bind(gfx);
		pVsFull->Bind(gfx);
		pLayoutFull->Bind(gfx);
		pSamplerFullPoint->Bind(gfx);
		blur.Bind(gfx);
		blur.SetHorizontal(gfx);
		gfx.DrawIndexed(pIbFull->GetCount());
		// fullscreen blur vertical-pass + combine
		gfx.BindSwapBuffer(ds);
		rt2.BindAsTexture(gfx, 0u); // input
		pBlenderMerge->Bind(gfx);
		pSamplerFullBilin->Bind(gfx);
		Stencil::Resolve(gfx, Stencil::Mode::Mask)->Bind(gfx);
		blur.SetVertical(gfx);
		gfx.DrawIndexed(pIbFull->GetCount());
	}
	void Reset() noexcept
	{
		for (auto& p : passes)
		{
			p.Reset();
		}
	}

	/**
	 * @brief render the UI window for controlleable target(s)
	 */
	void ShowWindows(Graphics& gfx)
	{
		blur.ShowWindow(gfx);
	}
private:
	std::array<Pass, 3> passes;
	DepthStencil ds;
	RenderTarget rt1;
	RenderTarget rt2;
	BlurPack blur;
	std::shared_ptr<GPipeline::VertexBuffer> pVbFull;
	std::shared_ptr<GPipeline::IndexBuffer> pIbFull;
	std::shared_ptr<GPipeline::VertexShader> pVsFull;
	std::shared_ptr<GPipeline::InputLayout> pLayoutFull;
	std::shared_ptr<GPipeline::Sampler> pSamplerFullPoint;
	std::shared_ptr<GPipeline::Sampler> pSamplerFullBilin;
	std::shared_ptr<GPipeline::Blender> pBlenderMerge;
};