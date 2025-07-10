#pragma once

#include "Graphics/RGraphicDevice_DX11.h"


class RIndexBuffer : public FGpuBuffer
{
public:
	RIndexBuffer();
	~RIndexBuffer();

	bool	Create(const vector<UINT>& indices);
	void	Bind() const;

	UINT	GetIndexCount() const	{ return mIndexCount; }

private:
	UINT		mIndexCount;
};


