#pragma once
#include <DirectXMath.h>
#include <string>
#include "GRAPHICS_OBJ_Projection.h"
#include "GRAPHICS_OBJ_CameraIndicator.h"

class Graphics;
namespace Rgph
{
	class RenderGraph;
}

class Camera
{
public:
	Camera(Graphics& gfx, std::string name, DirectX::XMFLOAT3 homePos = { 0.0f,0.0f,0.0f }, 
		float homePitch = 0.0f, float homeYaw = 0.0f, bool tethered = false) noexcept;
	void BindToGraphics(Graphics& gfx) const;
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWidgets(Graphics& gfx) noexcept;
	void Reset(Graphics& gfx) noexcept;
	/**
	 * @brief rotate according to mouse move deltas
	 */
	void Rotate(float dx, float dy) noexcept;
	void Translate(DirectX::XMFLOAT3 translation) noexcept;
	DirectX::XMFLOAT3 GetPos() const noexcept;
	void SetPos(const DirectX::XMFLOAT3& pos) noexcept;
	const std::string& GetName() const noexcept;
	void LinkTechniques(Rgph::RenderGraph& rg);
	void Submit(size_t channel) const;
private:
	bool tethered;
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
	/// Home values are the intial values when initialized
	DirectX::XMFLOAT3 homePos;
	float homePitch;
	float homeYaw;
	DirectX::XMFLOAT3 pos;
	float pitch;
	float yaw;
	// speed factors for mouse events
	static constexpr float travelSpeed = 12.0f;
	static constexpr float rotationSpeed = 0.004f;
	bool enableCameraIndicator = true;
	bool enableFrustumIndicator = true;
	Projection proj;
	CameraIndicator indicator;
};