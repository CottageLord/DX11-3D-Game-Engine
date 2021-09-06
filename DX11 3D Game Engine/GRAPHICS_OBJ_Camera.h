#pragma once
#include "SYS_CLASS_Graphics.h"

class Camera
{
public:
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
private:
	float distFromOrigin = 20.0f;
	// travel around laptitute
	float theta = 0.0f;
	// travel around longitute
	float phi = 0.0f;
	// orientation
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
};