#pragma once

#include "Component/Camera/huruCamera.h"
#include "Graphics/huruGraphicDevice_DX11.h"
#include "Resource/huruMesh.h"
#include "Graphics/GPUBuffer/huruConstantBuffer.h"


namespace huru::renderer
{
	extern Camera*				mainCamera;

	extern ConstantBuffer		constantBuffers[(UINT)eCBType::End];

	void	Initialize();
	void	Release();
}

