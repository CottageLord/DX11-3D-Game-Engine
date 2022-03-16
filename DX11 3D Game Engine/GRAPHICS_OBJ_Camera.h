#pragma once
#include "SYS_CLASS_Graphics.h"

class Camera
{
public:
	Camera(std::string name, DirectX::XMFLOAT3 homePos = { 0.0f,0.0f,0.0f }, float homePitch = 0.0f, float homeYaw = 0.0f) noexcept;
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWidgets() noexcept;
	void Reset() noexcept;
	// rotate according to mouse move deltas 
	void Rotate(float dx, float dy) noexcept;
	void Translate(DirectX::XMFLOAT3 translation) noexcept;
	DirectX::XMFLOAT3 GetPos() const noexcept;
	const std::string& GetName() const noexcept;
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
	std::string name;
	DirectX::XMFLOAT3 homePos;
	float homePitch;
	float homeYaw;
	DirectX::XMFLOAT3 pos;
	float pitch;
	float yaw;
	// speed factors for mouse events
	static constexpr float travelSpeed = 12.0f;
	static constexpr float rotationSpeed = 0.004f;
};