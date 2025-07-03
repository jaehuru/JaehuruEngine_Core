#pragma once

#include "Graphics/huruGraphicDevice_DX11.h"


namespace huru::graphics
{
	class VertexBuffer : public GpuBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		bool	Create(const vector<Vertex>& vertexes);
		void	Bind() const;

	private:
	};
}

