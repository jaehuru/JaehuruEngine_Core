#pragma once

#include "Component/Camera/JCamera.h"
#include "Graphics/RGraphicDevice_DX11.h"
#include "Resource/RMesh.h"
#include "Graphics/GPUBuffer/RConstantBuffer.h"

namespace renderer
{
	extern JCamera* mainCamera;

	extern RConstantBuffer		constantBuffers[(UINT)ECBType::End];

	extern Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerStates[(UINT)ESamplerType::End];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerStates[(UINT)ERasterizerState::End];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState>			blendStates[(UINT)EBlendState::End];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilStates[(UINT)EDepthStencilState::End];

	void	Initialize();
	void	Release();
}


