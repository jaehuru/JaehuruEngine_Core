#pragma once

#include "Component/Camera/huruCamera.h"
#include "GraphicDevice/huruGraphicDevice_DX11.h"
#include "Resource/huruMesh.h"
#include "GraphicDevice/GPUBuffer/huruConstantBuffer.h"


namespace huru::renderer
{
	extern Camera*				mainCamera;

	extern Mesh*				mesh;

	extern ConstantBuffer		constantBuffers[(UINT)eCBType::End];
	extern ID3D11Buffer*		constantBuffer;

	extern ID3D11InputLayout*	inputLayouts;

	void	Initialize();
	void	Release();
}

