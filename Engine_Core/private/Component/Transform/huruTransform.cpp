#include "Component/Transform/huruTransform.h"

namespace huru
{
	Transform::Transform() :
		Component(eComponentType::Transform),
		mPosition(Vector2::Zero),
		mScale(Vector2::One),
		mRotation(0.f)
	{

	}

	Transform::~Transform()
	{

	}

	void Transform::Initialize()
	{

	}

	void Transform::Update()
	{

	}

	void Transform::LateUpdate()
	{

	}

	void Transform::Render()
	{

	}
}