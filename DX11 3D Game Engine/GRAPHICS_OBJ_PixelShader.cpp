#include "GRAPHICS_OBJ_PixelShader.h"
#include "SYS_SET_GraphicsThrowMacros.h"
#include "SYS_HELP_Utility.h"
#include "GRAPHICS_OBJ_BindablePool.h"

namespace GPipeline
{
	PixelShader::PixelShader(Graphics& gfx, const std::string& path)
		:
		path(path) 
	{
		GET_INFO_MAN(gfx);

		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		// convert to narrow string for unified uid
		GFX_THROW_INFO(D3DReadFileToBlob(ToWide(path).c_str(), &pBlob));
		GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	}

	void PixelShader::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}

	std::shared_ptr<PixelShader> PixelShader::Resolve(Graphics& gfx, const std::string& path)
	{
		return BindablePool::Resolve<PixelShader>(gfx, path);
	}
	std::string PixelShader::GenerateUID(const std::string& path)
	{
		using namespace std::string_literals;
		return typeid(PixelShader).name() + "#"s + path;
	}
	std::string PixelShader::GetUID() const noexcept
	{
		return GenerateUID(path);
	}
}