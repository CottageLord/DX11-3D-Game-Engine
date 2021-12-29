#include "GRAPHICS_OBJ_Rasterizer.h"
#include "SYS_SET_GraphicsThrowMacros.h"
#include "GRAPHICS_OBJ_BindablePool.h"

namespace GPipeline
{
	Rasterizer::Rasterizer(Graphics& gfx, bool twoSided)
		:
		twoSided(twoSided)
	{
		GET_INFO_MAN(gfx);
		// CD3D is the class version instead of a struct version
		D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{}); // set everything else to default
		rasterDesc.CullMode = twoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK;

		GFX_THROW_INFO(GetDevice(gfx)->CreateRasterizerState(&rasterDesc, &pRasterizer));
	}

	void Rasterizer::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->RSSetState(pRasterizer.Get());
	}

	std::shared_ptr<Rasterizer> Rasterizer::Resolve(Graphics& gfx, bool twoSided)
	{
		return BindablePool::Resolve<Rasterizer>(gfx, twoSided);
	}
	std::string Rasterizer::GenerateUID(bool twoSided)
	{
		using namespace std::string_literals;
		return typeid(Rasterizer).name() + "#"s + (twoSided ? "2s" : "1s");
	}
	std::string Rasterizer::GetUID() const noexcept
	{
		return GenerateUID(twoSided);
	}
}