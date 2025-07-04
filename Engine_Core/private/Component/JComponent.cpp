#include "Component/JComponent.h"


JComponent::JComponent(EComponentType type) :
	mOwner(nullptr),
	mType(type)
{
		
}

JComponent::~JComponent()
{

}

void JComponent::Initialize()
{

}

void JComponent::Update()
{

}

void JComponent::LateUpdate()
{

}

void JComponent::Render()
{

}

void JComponent::Serialize(nlohmann::json& jsonObject) const
{
    FEntity::Serialize(jsonObject);
    jsonObject["Type"] = static_cast<int>(mType);
}

void JComponent::Deserialize(const nlohmann::json& jsonObject)
{
    FEntity::Deserialize(jsonObject);
    mType = static_cast<EComponentType>(jsonObject["Type"]);
}