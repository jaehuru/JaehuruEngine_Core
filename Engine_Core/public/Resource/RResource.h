#pragma once

#include "Common/FEntity.h"


class RResource : public FEntity
{
public:
	RResource(EResourceType type);
	virtual ~RResource() = default;
		
	virtual HRESULT Save(const wstring& path) = 0;
	virtual HRESULT Load(const wstring& path) = 0;

    void Serialize(json& jsonObject) const	override;
    void Deserialize(const json& jsonObject) override;

	EResourceType	GetType() const					{ return mType; }
	const wstring&	GetPath() const					{ return mPath; }

	void			SetPath(const wstring& path)	{ mPath = path; }

private:
	const EResourceType		mType;
	wstring					mPath;
};
