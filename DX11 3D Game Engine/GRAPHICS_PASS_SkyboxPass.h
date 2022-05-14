#pragma once
#include "GRAPHICS_PASS_RenderQueuePass.h"
#include "GRAPHICS_JOB_Job.h"
#include "GRAPHICS_RG_Sink.h"

#include "GRAPHICS_RG_Source.h"
#include "GRAPHICS_OBJ_Stencil.h"
#include "GRAPHICS_OBJ_Camera.h"
#include "GRAPHICS_OBJ_Sampler.h"
#include "GRAPHICS_OBJ_Rasterizer.h"
#include "GRAPHICS_OBJ_DepthStencil.h"
#include "GRAPHICS_OBJ_Cube.h"
#include "GRAPHICS_OBJ_CubeTexture.h"
#include "GRAPHICS_OBJ_Topology.h"
#include "GRAPHICS_OBJ_InputLayout.h"
#include "GRAPHICS_OBJ_Sphere.h"

#include "GRAPHICS_BUF_SkyboxTransformCbuf.h"
#include "GRAPHICS_BUF_VertexBuffer.h"
#include "GRAPHICS_BUF_IndexBuffer.h"

#include "imgui/imgui.h"

#include <vector>
class Graphics;

namespace Rgph
{
	/**
	 * @brief This pass does not submit any job(s), only binds resources to pipeline, thus a BindingPass
	 */
	class SkyboxPass : public BindingPass
	{
	public:
		SkyboxPass(Graphics& gfx, std::string name)
			:
			BindingPass(std::move(name))
		{
			using namespace GPipeline;
			RegisterSink(DirectBufferSink<RenderTarget>::Make("renderTarget", renderTarget));
			RegisterSink(DirectBufferSink<DepthStencil>::Make("depthStencil", depthStencil));
			AddBind(std::make_shared<CubeTexture>(gfx, "Images\\MountBox"/*SpaceBox"*/));
			AddBind(Stencil::Resolve(gfx, Stencil::Mode::DepthFirst));
			AddBind(Sampler::Resolve(gfx, Sampler::Type::Bilinear));
			AddBind(Rasterizer::Resolve(gfx, true));
			AddBind(std::make_shared<SkyboxTransformCbuf>(gfx));
			AddBind(PixelShader::Resolve(gfx, "Skybox_PS.cso"));
			AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
			{ // geometry-related
				auto pvs = GPipeline::VertexShader::Resolve(gfx, "Skybox_VS.cso");
				{ // cube
					auto model = Cube::Make();
					const auto geometryTag = "$cube_map";
					pCubeVertices = VertexBuffer::Resolve(gfx, geometryTag, std::move(model.vertices));
					pCubeIndices = IndexBuffer::Resolve(gfx, geometryTag, std::move(model.indices));
					cubeCount = (UINT)model.indices.size();
					// layout is shared between cube and sphere; use cube data to generate
					AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), *pvs));
				}
				{ // sphere
					auto model = Sphere::Make();
					const auto geometryTag = "$sphere_map";
					pSphereVertices = VertexBuffer::Resolve(gfx, geometryTag, std::move(model.vertices));
					pSphereIndices = IndexBuffer::Resolve(gfx, geometryTag, std::move(model.indices));
					sphereCount = (UINT)model.indices.size();
				}
				AddBind(std::move(pvs));
			}
			RegisterSource(DirectBufferSource<RenderTarget>::Make("renderTarget", renderTarget));
			RegisterSource(DirectBufferSource<DepthStencil>::Make("depthStencil", depthStencil));
		}
		/**
		 * @brief Allowing outside to set current render camera, in case order of passes messes up camera setting
		 */
		void BindMainCamera(const Camera& cam) noexcept
		{
			pMainCamera = &cam;
		}
		void Execute(Graphics& gfx) const noxnd override
		{
			assert(pMainCamera);
			UINT indexCount;
			pMainCamera->BindToGraphics(gfx);
			if (useSphere)
			{
				pSphereVertices->Bind(gfx);
				pSphereIndices->Bind(gfx);
				indexCount = sphereCount;
			}
			else
			{
				pCubeVertices->Bind(gfx);
				pCubeIndices->Bind(gfx);
				indexCount = cubeCount;
			}
			BindAll(gfx);
			gfx.DrawIndexed(indexCount);
		}
		void RenderWindow()
		{
			if (ImGui::Begin("Skybox"))
			{
				ImGui::Checkbox("Use sphere", &useSphere);
			}
			ImGui::End();
		}
	private:
		bool useSphere = true;
		const Camera* pMainCamera = nullptr;
		std::shared_ptr<GPipeline::VertexBuffer> pCubeVertices;
		std::shared_ptr<GPipeline::IndexBuffer> pCubeIndices;
		UINT cubeCount;
		std::shared_ptr<GPipeline::VertexBuffer> pSphereVertices;
		std::shared_ptr<GPipeline::IndexBuffer> pSphereIndices;
		UINT sphereCount;
	};
}