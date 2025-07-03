#include "Component/Rigidbody/huruRigidbody.h"
#include "Component/Transform/huruTransform.h"
#include "Helpers/huruTime.h"
#include "GameObject/huruGameObject.h"

namespace huru
{
	Rigidbody::Rigidbody() :
		Component(eComponentType::Rigidbody),
		mbGround(false),
		mMass(1.f),
		mFriction(10.f),
		mForce(Vector2::Zero),
		mAccelation(Vector2::Zero),
		mVelocity(Vector2::Zero),
		mLimitedVelocity(Vector2(200.f, 1000.f)),
		mGravity(Vector2(0.f, 800.f))
	{

	}

	Rigidbody::~Rigidbody()
	{

	}

	void Rigidbody::Initialize()
	{
		
	}

	void Rigidbody::Update()
	{
		// F = ma
		// a = F/m
		mAccelation = mForce / mMass;

		mVelocity += mAccelation * Time::DeltaTime();

		if (mbGround) // 땅
		{
			Vector2 gravity = mGravity;
			gravity.Normalize();

			float dot = mVelocity.Dot(gravity);
			mVelocity -= gravity * dot;
		}
		else // 공중
		{
			mVelocity += mGravity * Time::DeltaTime();
		}

		// 최대 속도 제한
		Vector2 gravity = mGravity;
		gravity.Normalize();
		float dot = mVelocity.Dot(gravity);
		gravity = gravity * dot;

		Vector2 sideVelocity = mVelocity - gravity;
		if (mLimitedVelocity.y < gravity.Length())
		{
			gravity.Normalize();
			gravity *= mLimitedVelocity.y;
		}
		
		if (mLimitedVelocity.x < sideVelocity.Length())
		{
			sideVelocity.Normalize();
			sideVelocity *= mLimitedVelocity.x;
		}

		mVelocity = gravity + sideVelocity;



		if (!(mVelocity == Vector2::Zero))
		{
			// 속도 반대방향으로 마찰력 작용
			Vector2 friction = -mVelocity;
			friction.Normalize();
			friction = friction * mFriction * mMass * Time::DeltaTime();

			// 마찰력으로 인한 속도 감소량이 현재 속도보다 큰 경우
			if (mVelocity.Length() <= friction.Length())
			{
				mVelocity = Vector2::Zero;
			}
			else
			{
				mVelocity += friction;
			}
		}

		Transform* transform = GetOwner()->GetComponent<Transform>();
		Vector2 pos = transform->GetPosition();
		pos = pos + mVelocity * Time::DeltaTime();
		transform->SetPosition(pos);

		mForce = Vector2::One;
	}

	void Rigidbody::LateUpdate()
	{

	}

	void Rigidbody::Render()
	{

	}
}