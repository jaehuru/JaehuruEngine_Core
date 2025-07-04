#pragma once

#include "Common/ISerializable.h"

class FEntity : public ISerializable
{
public:
    FEntity();
    virtual ~FEntity();

    virtual void Serialize(json& jsonObject) const override;
    virtual void Deserialize(const json& jsonObject) override;

    wstring&	GetName() 						{ return mName; }

    void		SetName(const wstring& name)	{ mName = name; }

private:
    wstring		mName;
};
