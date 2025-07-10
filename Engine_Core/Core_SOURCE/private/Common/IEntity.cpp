#include "Common/IEntity.h"


IEntity::IEntity() :
    mName(L"")
{

}

IEntity::~IEntity()
{

}

void IEntity::Serialize(json& jsonObject) const
{
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
    jsonObject["Name"] = converter.to_bytes(mName);
}

void IEntity::Deserialize(const json& jsonObject)
{
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
    mName = converter.from_bytes(jsonObject["Name"]);
}

