#pragma once
#include "GRAPHICS_OBJ_TestObject.h"

class Box : public TestObject<Box>
{
public:
	/**
	 * @brief initialize a box primitive, assign all attributes randomly (by feeding different distributions a randVal)
	 * @param gfx the graphics obj we are going to draw upon
	 * @param rng a mt19937 random number generator
	 * @param (x)dist different uniform distributions that will produce attributes randomly
	 */
	Box( Graphics& gfx,std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& bdist,
		DirectX::XMFLOAT3 material);
	//void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	// model transform
	DirectX::XMFLOAT3X3 modelTransform;
};