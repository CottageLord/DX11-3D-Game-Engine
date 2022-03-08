#pragma once
#include "SYS_CLASS_Graphics.h"
#include <d3d11.h>

class Graphics;

class GraphicsResource
{
protected:
	/**
	  * @brief A device context contains the circumstance or setting in which a device is used. 
	  * More specifically, a device context is used to set pipeline state and generate 
	  * rendering commands using the resources owned by a device.
	  */
	static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;
	/**
	  * @brief A device is used to create resources and to enumerate the capabilities of a display adapter.
	  * Each application must have at least one device, most applications only create one device.
	  */
	static ID3D11Device* GetDevice(Graphics& gfx) noexcept;
	/**
	 * @brief Returns the width of the swap chain
	 */
	static DxgiInfoManager& GetInfoManager(Graphics& gfx);
};