#include "Common/FEntity.h"


FEntity::FEntity() :
    mName(L"")
{

}

FEntity::~FEntity()
{

}

void FEntity::Serialize(json& jsonObject) const
{
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
    jsonObject["Name"] = converter.to_bytes(mName);
}

void FEntity::Deserialize(const json& jsonObject)
{
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
    mName = converter.from_bytes(jsonObject["Name"]);
}

