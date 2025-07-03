#include "Component/Collider/JCollider.h"
#include "Actor/AActor.h"
#include "Component/Script/JScript.h"


UINT32 JCollider::mCollisionID = 1;

JCollider::JCollider(EColliderType type) :
	JComponent(EComponentType::JCollider),
	mID(mCollisionID++),
	mOffset(FVector2::Zero),
	mSize(FVector2::One),
	mType(type)
{

}

JCollider::~JCollider()
{

}

void JCollider::Initialize()
{

}

void JCollider::Update()
{

}

void JCollider::LateUpdate()
{

}

void JCollider::Render()
{

}

void JCollider::OnCollisionEnter(JCollider* other)
{
	JScript* script = GetOwner()->GetComponent<JScript>();
	script->OnCollisionEnter(other);
}

void JCollider::OnCollisionStay(JCollider* other)
{
	JScript* script = GetOwner()->GetComponent<JScript>();
	script->OnCollisionStay(other);
}

void JCollider::OnCollisionExit(JCollider* other)
{
	JScript* script = GetOwner()->GetComponent<JScript>();
	script->OnCollisionExit(other);
}
