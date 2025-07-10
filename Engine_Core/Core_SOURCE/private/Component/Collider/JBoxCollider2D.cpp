#include "Component/Collider/JBoxCollider2D.h"
#include "Component/Transform/JTransform.h"
#include "Component/Camera/JCamera.h"
#include "Actor/AActor.h"
#include "Renderer/RRenderer.h"


JBoxCollider2D::JBoxCollider2D() :
	JCollider(EColliderType::Rect2D)
{

}

JBoxCollider2D::~JBoxCollider2D()
{

}

void JBoxCollider2D::Initialize()
{

}

void JBoxCollider2D::Update()
{

}

void JBoxCollider2D::LateUpdate()
{

}

void JBoxCollider2D::Render()
{
	JTransform* transform = GetOwner()->GetComponent<JTransform>();
	FVector3 pos = transform->GetPosition();
}

void JBoxCollider2D::Serialize(json& jsonObject) const
{
    JCollider::Serialize(jsonObject);
}

void JBoxCollider2D::Deserialize(const json& jsonObject)
{
    JCollider::Deserialize(jsonObject);
}