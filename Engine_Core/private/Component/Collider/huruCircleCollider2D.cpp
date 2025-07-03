#include "Component/Collider/huruCircleCollider2D.h"
#include "Component/Transform/huruTransform.h"
#include "Component/Camera/huruCamera.h"
#include "GameObject/huruGameObject.h"
#include "Renderer/huruRenderer.h"

namespace huru
{
	CircleCollider2D::CircleCollider2D() : 
		Collider(eColliderType::Circle2D),
		mRadius(0.0f)
	{

	}

	CircleCollider2D::~CircleCollider2D()
	{

	}

	void CircleCollider2D::Initialize()
	{

	}

	void CircleCollider2D::Update()
	{

	}

	void CircleCollider2D::LateUpdate()
	{

	}

	void CircleCollider2D::Render()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();
	}
}