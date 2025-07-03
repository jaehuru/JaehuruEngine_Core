#pragma once

#include "Graphics/RGraphicDevice_DX11.h"



class RVertexBuffer : public FGpuBuffer
{
public:
	RVertexBuffer();
	~RVertexBuffer();

	bool	Create(const vector<FVertex>& vertexes);
	void	Bind() const;

private:
};


