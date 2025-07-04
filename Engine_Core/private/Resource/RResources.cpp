#include "Resource/RResources.h"
#include "Resource/RTexture.h"
#include "Resource/RMesh.h"
#include "Resource/RMaterial.h"
#include "Resource/RShader.h"
#include "Resource/RAudioClip.h"
#include <string>
#include <locale>
#include <codecvt>


map<wstring, RResource*> RResources::mResources = {};

void RResources::Serialize(json& jsonObject)
{
    json resourcesArray = json::array();
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;

    for (const auto& pair : mResources)
    {
        json resourceJson;
        resourceJson["Name"] = converter.to_bytes(pair.first);
        resourceJson["Type"] = static_cast<int>(pair.second->GetType());
        pair.second->Serialize(resourceJson);
        resourcesArray.push_back(resourceJson);
    }
    jsonObject["Resources"] = resourcesArray;
}

void RResources::Deserialize(const json& jsonObject)
{
    Release();

    const json& resourcesArray = jsonObject["Resources"];
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;

    for (const auto& resourceJson : resourcesArray)
    {
        wstring name = converter.from_bytes(resourceJson["Name"]);
        EResourceType type = static_cast<EResourceType>(resourceJson["Type"]);
        RResource* newResource = nullptr;

        switch (type)
        {
        case EResourceType::RTexture:
            newResource = new RTexture();
            break;
        case EResourceType::RMesh:
            newResource = new RMesh();
            break;
        case EResourceType::RMaterial:
            newResource = new RMaterial();
            break;
        case EResourceType::RShader:
            newResource = new RShader();
            break;
        case EResourceType::RAudioClip:
            newResource = new RAudioClip();
            break;
        default:
            break;
        }

        if (newResource != nullptr)
        {
            newResource->Deserialize(resourceJson);
            mResources.insert(make_pair(name, newResource));
        }
    }
}
