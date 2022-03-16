#pragma once
#include <array>
#include <optional>
#include "SYS_SET_ConditionalNoexcept.h"
#include "SYS_CLASS_Graphics.h"
#include "SYS_HELP_PerfLog.h"
#include "GRAPHICS_SET_BindableCommon.h"
#include "GRAPHICS_OBJ_DepthStencil.h"
#include "GRAPHICS_OBJ_DynamicConstant.h"
#include "GRAPHICS_OBJ_DepthStencil.h"
#include "GRAPHICS_OBJ_RenderTarget.h"
#include "GRAPHICS_OBJ_BlurPack.h"
#include "GRAPHICS_JOB_Job.h"
#include "GRAPHICS_JOB_Pass.h"
#include <array>


namespace GPipeline
{
	class VertexBuffer;
	class IndexBuffer;
	class VertexShader;
	class InputLayout;
	class Sampler;
	class Sampler;
	class Blender;
}

/**
 * @brief manages all render passes, specifying the operations for each pass
 */
class FrameCommander
{
public:
	FrameCommander(Graphics& gfx);
	void Accept(Job job, size_t target) noexcept;
	void Execute(Graphics& gfx) noxnd;
	void Reset() noexcept;
	/**
	 * @brief render the UI window for controlleable target(s)
	 */
	void ShowWindows(Graphics& gfx);
private:
	std::array<Pass, 3> passes;
	int downFactor = 1;
	DepthStencil ds;
	std::optional<RenderTarget> rt1;
	std::optional<RenderTarget> rt2;
	BlurPack blur;
	std::shared_ptr<GPipeline::VertexBuffer> pVbFull;
	std::shared_ptr<GPipeline::IndexBuffer> pIbFull;
	std::shared_ptr<GPipeline::VertexShader> pVsFull;
	std::shared_ptr<GPipeline::InputLayout> pLayoutFull;
	std::shared_ptr<GPipeline::Sampler> pSamplerFullPoint;
	std::shared_ptr<GPipeline::Sampler> pSamplerFullBilin;
	std::shared_ptr<GPipeline::Blender> pBlenderMerge;
};