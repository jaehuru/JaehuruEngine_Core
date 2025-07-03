#pragma once

#include "Common/FEntity.h"


class RResource : public FEntity
{
public:
	RResource(EResourceType type);
	virtual ~RResource() = default;
		
	virtual HRESULT Save(const wstring& path) = 0;
	virtual HRESULT Load(const wstring& path) = 0;

	const wstring&	GetPath() const					{ return mPath; }

	void			SetPath(const wstring& path)	{ mPath = path; }

private:
	const EResourceType		mType;
	wstring					mPath;
};

