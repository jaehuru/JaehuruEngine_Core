#include "Component/Collider/huruCollider.h"
#include "GameObject/huruGameObject.h"
#include "Component/Script/huruScript.h"

namespace huru
{
	UINT32 Collider::mCollisionID = 1;

	Collider::Collider(eColliderType type) :
		Component(eComponentType::Collider),
		mID(mCollisionID++),
		mOffset(Vector2::Zero),
		mSize(Vector2::One),
		mType(type)
	{

	}

	Collider::~Collider()
	{

	}

	void Collider::Initialize()
	{

	}

	void Collider::Update()
	{

	}

	void Collider::LateUpdate()
	{

	}

	void Collider::Render()
	{

	}

	void Collider::OnCollisionEnter(Collider* other)
	{
		Script* script = GetOwner()->GetComponent<Script>();
		script->OnCollisionEnter(other);
	}

	void Collider::OnCollisionStay(Collider* other)
	{
		Script* script = GetOwner()->GetComponent<Script>();
		script->OnCollisionStay(other);
	}

	void Collider::OnCollisionExit(Collider* other)
	{
		Script* script = GetOwner()->GetComponent<Script>();
		script->OnCollisionExit(other);
	}
}