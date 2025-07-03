#pragma once

#include "Common/huruEntity.h"

namespace huru
{
	class Resource : public Entity
	{
	public:
		Resource(eResourceType type);
		virtual ~Resource() = default;
		
		virtual HRESULT Save(const wstring& path) = 0;
		virtual HRESULT Load(const wstring& path) = 0;

		const wstring&	GetPath() const					{ return mPath; }

		void			SetPath(const wstring& path)	{ mPath = path; }

	private:
		const eResourceType		mType;
		wstring					mPath;
	};
}
