#include "Resource/RResource.h"


RResource::RResource(EResourceType type) :
	mType(type)
{

}

HRESULT RResource::Load(const wstring& path)
{
	return E_NOTIMPL;
}

void RResource::Serialize(json& jsonObject) const
{
    IEntity::Serialize(jsonObject);
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
    jsonObject["Type"] = static_cast<int>(mType);
    jsonObject["Path"] = converter.to_bytes(mPath);
}

void RResource::Deserialize(const json& jsonObject)
{
    IEntity::Deserialize(jsonObject);
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
    // mType is const, so it's initialized in constructor. We only deserialize mPath.
    mPath = converter.from_bytes(jsonObject["Path"]);
}
