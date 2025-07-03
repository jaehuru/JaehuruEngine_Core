#include "Component/Collider/huruBoxCollider2D.h"
#include "Component/Transform/huruTransform.h"
#include "Component/Camera/huruCamera.h"
#include "GameObject/huruGameObject.h"
#include "Renderer/huruRenderer.h"

namespace huru
{
	BoxCollider2D::BoxCollider2D() :
		Collider(eColliderType::Rect2D)
	{

	}

	BoxCollider2D::~BoxCollider2D()
	{

	}

	void BoxCollider2D::Initialize()
	{

	}

	void BoxCollider2D::Update()
	{

	}

	void BoxCollider2D::LateUpdate()
	{

	}

	void BoxCollider2D::Render()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();
	}
}