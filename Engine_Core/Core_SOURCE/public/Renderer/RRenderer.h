#pragma once

#include "Graphics/RGraphicDevice_DX11.h"


class JCamera;
class AActor;
class RConstantBuffer;
class RRenderTarget;


namespace renderer
{
	extern JCamera* mainCamera;
	extern AActor*	selectedActor;

	extern RConstantBuffer*	constantBuffers[(UINT)ECBType::End];

	extern Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerStates[(UINT)ESamplerType::End];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerStates[(UINT)ERasterizerState::End];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState>			blendStates[(UINT)EBlendState::End];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilStates[(UINT)EDepthStencilState::End];

	extern RRenderTarget* FrameBuffer;

	void	Initialize();
	void	Release();
}


