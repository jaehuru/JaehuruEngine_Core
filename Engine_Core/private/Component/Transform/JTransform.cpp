#include "Component/Transform/JTransform.h"

JTransform::JTransform() :
	JComponent(EComponentType::JTransform),
	mPosition(FVector2::Zero),
	mScale(FVector2::One),
	mRotation(0.f)
{

}

JTransform::~JTransform()
{

}

void JTransform::Initialize()
{

}

void JTransform::Update()
{

}

void JTransform::LateUpdate()
{

}

void JTransform::Render()
{

}

void JTransform::Serialize(json& jsonObject) const
{
	JComponent::Serialize(jsonObject);
	jsonObject["PositionX"] = mPosition.x;
	jsonObject["PositionY"] = mPosition.y;
	jsonObject["ScaleX"] = mScale.x;
	jsonObject["ScaleY"] = mScale.y;
	jsonObject["Rotation"] = mRotation;
}

void JTransform::Deserialize(const json& jsonObject)
{
	JComponent::Deserialize(jsonObject);
	mPosition.x = jsonObject["PositionX"];
	mPosition.y = jsonObject["PositionY"];
	mScale.x = jsonObject["ScaleX"];
	mScale.y = jsonObject["ScaleY"];
	mRotation = jsonObject["Rotation"];
}