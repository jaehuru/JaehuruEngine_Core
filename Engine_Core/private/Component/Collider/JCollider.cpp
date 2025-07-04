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

void JCollider::Serialize(json& jsonObject) const
{
    JComponent::Serialize(jsonObject);
    jsonObject["ID"] = mID;
    jsonObject["OffsetX"] = mOffset.x;
    jsonObject["OffsetY"] = mOffset.y;
    jsonObject["SizeX"] = mSize.x;
    jsonObject["SizeY"] = mSize.y;
    jsonObject["ColliderType"] = static_cast<int>(mType);
}

void JCollider::Deserialize(const json& jsonObject)
{
    JComponent::Deserialize(jsonObject);
    mID = jsonObject["ID"];
    mOffset.x = jsonObject["OffsetX"];
    mOffset.y = jsonObject["OffsetY"];
    mSize.x = jsonObject["SizeX"];
    mSize.y = jsonObject["SizeY"];
    mType = static_cast<EColliderType>(jsonObject["ColliderType"]);
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
