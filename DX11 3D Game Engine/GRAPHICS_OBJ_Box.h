#pragma once
#include "GRAPHICS_OBJ_StaticDrawInfo.h"

class Box : public StaticDrawInfo<Box>
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
		std::uniform_real_distribution<float>& bdist);
	void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	// positional
	float randVal;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
	// model transform
	DirectX::XMFLOAT3X3 modelTransform;
};