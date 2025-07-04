#include "Component/Collider/JCircleCollider2D.h"
#include "Component/Transform/JTransform.h"
#include "Component/Camera/JCamera.h"
#include "Actor/AActor.h"
#include "Renderer/RRenderer.h"

JCircleCollider2D::JCircleCollider2D() : 
	JCollider(EColliderType::Circle2D),
	mRadius(0.0f)
{

}

JCircleCollider2D::~JCircleCollider2D()
{

}

void JCircleCollider2D::Initialize()
{

}

void JCircleCollider2D::Update()
{

}

void JCircleCollider2D::LateUpdate()
{

}

void JCircleCollider2D::Render()
{
	JTransform* tr = GetOwner()->GetComponent<JTransform>();
	FVector2 pos = tr->GetPosition();
}

void JCircleCollider2D::Serialize(json& jsonObject) const
{
    JCollider::Serialize(jsonObject);
    jsonObject["RadiusX"] = mRadius.x;
    jsonObject["RadiusY"] = mRadius.y;
}

void JCircleCollider2D::Deserialize(const json& jsonObject)
{
    JCollider::Deserialize(jsonObject);
    mRadius.x = jsonObject["RadiusX"];
    mRadius.y = jsonObject["RadiusY"];
}