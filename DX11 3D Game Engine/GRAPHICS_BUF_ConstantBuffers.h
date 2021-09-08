/**
 * @GRAPHICS_BUF_ConstantBuffer.h
 * @brief A constant buffer obj that manages vertex/pixel buffers
 */
#pragma once
#include "GRAPHICS_OBJ_Bindable.h"
#include "SYS_SET_GraphicsThrowMacros.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	/**
	 * @brief Update the target const buffer per frame. write new info into the registered buffer interface
	 * @param gfx the Graphics object we made to manage DX3D interfaces
	 * @param consts the new buffer data to be placed into the DX3D system buffer
	 */
	void Update(Graphics& gfx, const C& consts)
	{
		GET_INFO_MAN(gfx);
		D3D11_MAPPED_SUBRESOURCE msr;
		/*Gets a pointer to the data contained in a subresource,
		and denies the GPU access to that subresource.*/
		GFX_THROW_INFO(GetContext(gfx)->Map( // lock
			pConstantBuffer.Get(), 0u,
			// the app doesn't need to keep the old vertex or index data in the buffer.
			D3D11_MAP_WRITE_DISCARD, 0u, 
			// get the buffer subresource section address that we are going to fill
			&msr
		));
		memcpy(msr.pData, &consts, sizeof(consts)); // write
		GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u); // unlock
	}
	/**
	 * @brief Init const buffer with given initialized data struct
	 * @param consts the data struct to be moved into the const buffer
	 */
	ConstantBuffer(Graphics& gfx, const C& consts, UINT slot = 0u)
		:
		slot(slot)
	{
		GET_INFO_MAN(gfx);

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer));
	}
	ConstantBuffer(Graphics& gfx, UINT slot = 0u)
		:
		slot(slot)
	{
		GET_INFO_MAN(gfx);

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer));
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	UINT slot;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	// template inheritance losses those refs and need explicit import or this->/:: qualifier
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx) noexcept override
	{
		GetContext(gfx)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx) noexcept override
	{
		GetContext(gfx)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
};