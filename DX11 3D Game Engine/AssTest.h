#pragma once
#include "GRAPHICS_OBJ_TestObject.h"
#include "GRAPHICS_BUF_ConstantBuffers.h"

class AssTest : public TestObject<AssTest>
{
public:
	AssTest(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		DirectX::XMFLOAT3 material,
		float scale);
};