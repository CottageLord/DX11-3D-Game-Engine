#pragma once
#include <array>
#include "SYS_CLASS_Graphics.h"
#include "GRAPHICS_SET_BindableCommon.h"
#include "GRAPHICS_JOB_Job.h"
#include "GRAPHICS_JOB_Pass.h"

/**
 * @brief manages all render passes, specifying the operations for each pass
 */
class FrameCommander
{
public:
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

		// main phong lighting pass
		Stencil::Resolve(gfx, Stencil::Mode::Off)->Bind(gfx);
		passes[0].Execute(gfx);
		// outline masking pass
		Stencil::Resolve(gfx, Stencil::Mode::Write)->Bind(gfx);
		NullPixelShader::Resolve(gfx)->Bind(gfx); // only update stencil, no rendering
		passes[1].Execute(gfx);
		// outline drawing pass
		Stencil::Resolve(gfx, Stencil::Mode::Mask)->Bind(gfx);
		struct SolidColorBuffer
		{
			DirectX::XMFLOAT4 color = { 1.0f,0.4f,0.4f,1.0f };
		} scb;
		PixelConstantBuffer<SolidColorBuffer>::Resolve(gfx, scb, 1u)->Bind(gfx);
		passes[2].Execute(gfx);
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
};