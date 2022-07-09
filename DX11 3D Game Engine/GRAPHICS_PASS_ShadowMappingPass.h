#pragma once
#include "GRAPHICS_PASS_RenderQueuePass.h"
#include "SYS_SET_Math.h"
#include "GRAPHICS_OBJ_PixelShader.h"
#include "GRAPHICS_OBJ_VertexShader.h"
#include "GRAPHICS_OBJ_Stencil.h"
#include "GRAPHICS_OBJ_Rasterizer.h"
#include "GRAPHICS_OBJ_RenderTarget.h"
#include "GRAPHICS_OBJ_Blender.h"
#include "GRAPHICS_OBJ_NullPixelShader.h"
#include "GRAPHICS_OBJ_ShadowRasterizer.h"
#include "GRAPHICS_OBJ_CubeTexture.h"
#include "GRAPHICS_OBJ_Viewport.h"
#include "GRAPHICS_JOB_Job.h"
#include "GRAPHICS_RG_Source.h"


#include <vector>

class Graphics;

namespace Rgph
{
	class ShadowMappingPass : public RenderQueuePass
	{
	public:
		void BindShadowCamera(const Camera& cam) noexcept
		{
			pShadowCamera = &cam;
		}
		ShadowMappingPass(Graphics& gfx, std::string name)
			:
			RenderQueuePass(std::move(name))
		{
			using namespace GPipeline;
			//depthStencil = std::make_unique<ShaderInputDepthStencil>(gfx, 3, DepthStencil::Usage::ShadowDepth);
			//AddBind(VertexShader::Resolve(gfx, "Solid_VS.cso"));
			pDepthCube = std::make_shared<DepthCubeTexture>(gfx, size, 3);
			AddBind(VertexShader::Resolve(gfx, "Shadow_VS.cso"));
			AddBind(NullPixelShader::Resolve(gfx));
			AddBind(Stencil::Resolve(gfx, Stencil::Mode::Off));
			AddBind(Blender::Resolve(gfx, false));
			AddBind(std::make_shared<Viewport>(gfx, (float)size, (float)size));
			AddBind(std::make_shared<GPipeline::ShadowRasterizer>(gfx, 50, 2.0f, 0.1f));
			RegisterSource(DirectBindableSource<GPipeline::DepthCubeTexture>::Make("map", pDepthCube));

			DirectX::XMStoreFloat4x4(
				&projection,
				DirectX::XMMatrixPerspectiveFovLH(PI / 2.0f, 1.0f, 0.5f, 100.0f)
			);
			// +x
			DirectX::XMStoreFloat3(&cameraDirections[0], DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
			DirectX::XMStoreFloat3(&cameraUps[0], DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			// -x
			DirectX::XMStoreFloat3(&cameraDirections[1], DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f));
			DirectX::XMStoreFloat3(&cameraUps[1], DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			// +y
			DirectX::XMStoreFloat3(&cameraDirections[2], DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			DirectX::XMStoreFloat3(&cameraUps[2], DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
			// -y
			DirectX::XMStoreFloat3(&cameraDirections[3], DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));
			DirectX::XMStoreFloat3(&cameraUps[3], DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f));
			// +z
			DirectX::XMStoreFloat3(&cameraDirections[4], DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
			DirectX::XMStoreFloat3(&cameraUps[4], DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			// -z
			DirectX::XMStoreFloat3(&cameraDirections[5], DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f));
			DirectX::XMStoreFloat3(&cameraUps[5], DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

			// set the depth buffer to *something* so that the rg validation doesn't scream
			SetDepthBuffer(pDepthCube->GetDepthBuffer(0));
		}
		void Execute(Graphics& gfx) const noxnd override
		{
			using namespace DirectX;

			DirectX::XMFLOAT3 returnedPos = pShadowCamera->GetPos();
			const auto pos = XMLoadFloat3(&returnedPos);

			gfx.SetProjection(XMLoadFloat4x4(&projection));
			for (size_t i = 0; i < 6; i++)
			{
				auto d = pDepthCube->GetDepthBuffer(i);
				d->Clear(gfx);
				SetDepthBuffer(std::move(d));
				const auto lookAt = pos + XMLoadFloat3(&cameraDirections[i]);
				gfx.SetCamera(XMMatrixLookAtLH(pos, lookAt, XMLoadFloat3(&cameraUps[i])));
				RenderQueuePass::Execute(gfx);
			}
		}
		void DumpShadowMap(Graphics& gfx, const std::string& path) const
		{
			for (size_t i = 0; i < 6; i++)
			{
				auto d = pDepthCube->GetDepthBuffer(i);
				d->ToSurface(gfx).Save(path + std::to_string(i) + ".png");
			}
		}

		void Reset() noxnd override
		{
			RenderQueuePass::Reset();
		}
	private:
		void SetDepthBuffer(std::shared_ptr<GPipeline::DepthStencil> ds) const
		{
			const_cast<ShadowMappingPass*>(this)->depthStencil = std::move(ds);
		}
		static constexpr UINT size = 1000;
		const Camera* pShadowCamera = nullptr;
		std::shared_ptr<GPipeline::DepthCubeTexture> pDepthCube;
		DirectX::XMFLOAT4X4 projection;
		std::vector<DirectX::XMFLOAT3> cameraDirections{ 6 };
		std::vector<DirectX::XMFLOAT3> cameraUps{ 6 };
	};
}