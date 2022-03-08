#include "GRAPHICS_OBJ_VertexShader.h"
#include "SYS_SET_GraphicsThrowMacros.h"
#include "SYS_HELP_Utility.h"
#include "GRAPHICS_OBJ_BindablePool.h"
#include <typeinfo>

namespace GPipeline
{

	VertexShader::VertexShader(Graphics& gfx, const std::string& path)
		:
		path(path)
	{
		GET_INFO_MAN(gfx);

		GFX_THROW_INFO(D3DReadFileToBlob(ToWide(path).c_str(), &pBytecodeBlob));
		GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(
			pBytecodeBlob->GetBufferPointer(),
			pBytecodeBlob->GetBufferSize(),
			nullptr,
			&pVertexShader
		));
	}

	void VertexShader::Bind(Graphics& gfx) noxnd
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u));
	}

	ID3DBlob* VertexShader::GetBytecode() const noexcept
	{
		return pBytecodeBlob.Get();
	}

	/**
	* @brief returns a existed bindable shader, or create a new one if same shader not stored before
	*/
	std::shared_ptr<VertexShader> VertexShader::Resolve(Graphics& gfx, const std::string& path)
	{
		return BindablePool::Resolve<VertexShader>(gfx, path);
	}
	std::string VertexShader::GenerateUID(const std::string& path)
	{
		using namespace std::string_literals;
		return typeid(VertexShader).name() + "#"s + path;
	}
	std::string VertexShader::GetUID() const noexcept
	{
		return GenerateUID(path);
	}
}