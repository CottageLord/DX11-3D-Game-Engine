#include "GRAPHICS_OBJ_NullPixelShader.h"
#include "SYS_SET_GraphicsThrowMacros.h"
#include "GRAPHICS_OBJ_BindablePool.h"
#include "SYS_HELP_Utility.h"

namespace GPipeline
{
	NullPixelShader::NullPixelShader(Graphics& gfx)
	{
	}
	void NullPixelShader::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetShader(nullptr, nullptr, 0u);
	}
	std::shared_ptr<NullPixelShader> NullPixelShader::Resolve(Graphics& gfx)
	{
		return BindablePool::Resolve<NullPixelShader>(gfx);
	}
	std::string NullPixelShader::GenerateUID()
	{
		return typeid(NullPixelShader).name();
	}
	std::string NullPixelShader::GetUID() const noexcept
	{
		return GenerateUID();
	}
}