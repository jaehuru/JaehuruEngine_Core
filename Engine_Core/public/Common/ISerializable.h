#pragma once

#include "Common/CommonInclude.h"

class ISerializable
{
public:
    virtual ~ISerializable() = default;


    virtual void Serialize(json & jsonObject) const = 0;
    virtual void Deserialize(const json & jsonObject) = 0;
};

