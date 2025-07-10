#pragma once

#include "Graphics/RGraphicDevice_DX11.h"


class RConstantBuffer : public FGpuBuffer
{
public:
	RConstantBuffer(ECBType type);
	~RConstantBuffer();

	bool	Create(UINT size, void* data = NULL);

	void	SetData(void* data) const;
	void	Bind(EShaderStage stage) const;

private:
	UINT		mSize;
	ECBType		mType;
};


