#pragma once
#include "GRAPHICS_OBJ_Bindable.h"

namespace GPipeline
{
	class Stencil : public Bindable
	{
	public:
		enum class Mode
		{
			Off,	// No action
			Write,	// Write to stencil buf when the pixel draw the geometry
			Mask	// Masking things out, exclude pixels
		};
		/*
		typedef struct D3D11_DEPTH_STENCIL_DESC {
			BOOL                       DepthEnable;			Enable depth testing.
			D3D11_DEPTH_WRITE_MASK     DepthWriteMask;		Turn on/off writes to the depth-stencil buffer.
			D3D11_COMPARISON_FUNC      DepthFunc;			source (?) dest, comparision passes
			BOOL                       StencilEnable;		Enable stencil testing.
			UINT8                      StencilReadMask;		Bit mask AND applied to stencil data
			UINT8                      StencilWriteMask;	Identify a portion of the depth-stencil buffer for writing stencil data.
			D3D11_DEPTH_STENCILOP_DESC FrontFace;			Work with frontface geometry
			D3D11_DEPTH_STENCILOP_DESC BackFace;
		} D3D11_DEPTH_STENCIL_DESC;

		typedef struct D3D11_DEPTH_STENCILOP_DESC {
			D3D11_STENCIL_OP      StencilFailOp;			The stencil operation to perform when stencil testing fails.
			D3D11_STENCIL_OP      StencilDepthFailOp;		stencil testing passes and depth testing fails.
			D3D11_STENCIL_OP      StencilPassOp;			stencil testing and depth testing both pass.
			D3D11_COMPARISON_FUNC StencilFunc;				A function that compares stencil data against existing stencil data. 
		} D3D11_DEPTH_STENCILOP_DESC;

		typedef enum D3D11_STENCIL_OP {
			D3D11_STENCIL_OP_KEEP,			Keep the existing stencil data. Do nothing
			D3D11_STENCIL_OP_ZERO,			Set the stencil data to 0.
			D3D11_STENCIL_OP_REPLACE,		Set the stencil data to the reference value set by calling
			D3D11_STENCIL_OP_INCR_SAT,		Increment the stencil value by 1, and clamp the result.
			D3D11_STENCIL_OP_DECR_SAT,		Decrement the stencil value by 1, and clamp the result.
			D3D11_STENCIL_OP_INVERT,		Invert the stencil data.
			D3D11_STENCIL_OP_INCR,			Increment the stencil value by 1, and wrap the result if necessary.
			D3D11_STENCIL_OP_DECR			Increment the stencil value by 1, and wrap the result if necessary.
		} ;

		typedef enum D3D11_COMPARISON_FUNC {
			D3D11_COMPARISON_NEVER,			Never pass the comparison.
			D3D11_COMPARISON_LESS,			If the source data is less than the destination data, the comparison passes.
			D3D11_COMPARISON_EQUAL,
			D3D11_COMPARISON_LESS_EQUAL,
			D3D11_COMPARISON_GREATER,
			D3D11_COMPARISON_NOT_EQUAL,
			D3D11_COMPARISON_GREATER_EQUAL,
			D3D11_COMPARISON_ALWAYS
		} ;
		*/
		/*
		To draw geometry outlines:
			1. draw geometries normally
			2. generate mask in stencil buffer ("draw again")
			3. scaling up the mask a bit and mask the new shape with the stencil data
		*/
		Stencil(Graphics& gfx, Mode mode)
		{
			D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

			if (mode == Mode::Write)
			{
				dsDesc.StencilEnable = TRUE;
				dsDesc.StencilWriteMask = 0xFF;	// use all the bits
				dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;	// always set pixels on render target
				dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE; // write whatever value we want correspond to the geometry we drew
			}
			else if (mode == Mode::Mask)
			{
				dsDesc.DepthEnable = FALSE;	// the outline is not the normal thing, therefore shouldn't been involved in the depth test
				dsDesc.StencilEnable = TRUE;
				dsDesc.StencilReadMask = 0xFF;
				dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL; // exclude the overlapping area
				dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; // keep the stencil data, do not pollute it
				dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			}

			GetDevice(gfx)->CreateDepthStencilState(&dsDesc, &pStencil);
		}
		void Bind(Graphics& gfx) noexcept override
		{
			GetContext(gfx)->OMSetDepthStencilState(pStencil.Get(), 0xFF); // value written in buffer ans used for comparision
		}
		//static std::shared_ptr<Stencil> Resolve( Graphics& gfx,bool blending,std::optional<float> factor = {} );
		//static std::string GenerateUID( bool blending,std::optional<float> factor );
		//std::string GetUID() const noexcept override;
	private:
		// same state obj controlling both depth and stencil
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pStencil;
	};
}