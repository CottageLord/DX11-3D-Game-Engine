#pragma once
#include "SYS_CLASS_Graphics.h"

class Camera
{
public:
	Camera() noexcept;
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	// rotate according to mouse move deltas 
	void Rotate(float dx, float dy) noexcept;
	void Translate(DirectX::XMFLOAT3 translation) noexcept;
private:
	/*
	float distFromOrigin = 20.0f;
	// travel around laptitute
	float theta = 0.0f;
	// travel around longitute
	float phi = 0.0f;
	// orientation
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
	*/
	DirectX::XMFLOAT3 pos;
	float pitch;
	float yaw;
	// speed factors for mouse events
	static constexpr float travelSpeed = 12.0f;
	static constexpr float rotationSpeed = 0.004f;
};