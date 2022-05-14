#pragma once
#include "GRAPHICS_OBJ_Drawable.h"

class Frustum : public Drawable
{
public:
	/**
	 * @brief sets frustum draw procedures.
	 */
	Frustum(Graphics& gfx, float width, float height, float nearZ, float farZ);
	/**
	 * @brief calculates the exact shape of the frustum
	 */
	void SetVertices(Graphics& gfx, float width, float height, float nearZ, float farZ);
	/**
	 * @brief updates the position of the frustum according to newest camera position
	 */
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	/**
	 * @brief updates the rotation of the frustum according to newest camera rotation
	 */
	void SetRotation(DirectX::XMFLOAT3 pos) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rot = { 0.0f,0.0f,0.0f };
};