#pragma once

#include "Graphics/huruGraphicDevice_DX11.h"

namespace huru::graphics
{
	class IndexBuffer : public GpuBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		bool	Create(const vector<UINT>& indices);
		void	Bind() const;

	private:
	};
}

