#pragma once

#include "Common/CommonInclude.h"

class ISerializable
{
public:
    virtual ~ISerializable() = default;


    virtual void Serialize(nlohmann::json & jsonObject) const = 0;
    virtual void Deserialize(const nlohmann::json & jsonObject) = 0;
};

