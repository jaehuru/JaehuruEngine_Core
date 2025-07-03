#include "Component/Rigidbody/JRigidbody.h"
#include "Component/Transform/JTransform.h"
#include "Helpers/Time.h"
#include "Actor/AActor.h"


JRigidbody::JRigidbody() :
	JComponent(EComponentType::JRigidbody),
	mbGround(false),
	mMass(1.f),
	mFriction(10.f),
	mForce(FVector2::Zero),
	mAccelation(FVector2::Zero),
	mVelocity(FVector2::Zero),
	mLimitedVelocity(FVector2(200.f, 1000.f)),
	mGravity(FVector2(0.f, 800.f))
{

}

JRigidbody::~JRigidbody()
{

}

void JRigidbody::Initialize()
{
		
}

void JRigidbody::Update()
{
	// F = ma
	// a = F/m
	mAccelation = mForce / mMass;

	mVelocity += mAccelation * Time::DeltaTime();

	if (mbGround) // 땅
	{
		FVector2 gravity = mGravity;
		gravity.Normalize();

		float dot = mVelocity.Dot(gravity);
		mVelocity -= gravity * dot;
	}
	else // 공중
	{
		mVelocity += mGravity * Time::DeltaTime();
	}

	// 최대 속도 제한
	FVector2 gravity = mGravity;
	gravity.Normalize();
	float dot = mVelocity.Dot(gravity);
	gravity = gravity * dot;

	FVector2 sideVelocity = mVelocity - gravity;
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



	if (!(mVelocity == FVector2::Zero))
	{
		// 속도 반대방향으로 마찰력 작용
		FVector2 friction = -mVelocity;
		friction.Normalize();
		friction = friction * mFriction * mMass * Time::DeltaTime();

		// 마찰력으로 인한 속도 감소량이 현재 속도보다 큰 경우
		if (mVelocity.Length() <= friction.Length())
		{
			mVelocity = FVector2::Zero;
		}
		else
		{
			mVelocity += friction;
		}
	}

	JTransform* transform = GetOwner()->GetComponent<JTransform>();
	FVector2 pos = transform->GetPosition();
	pos = pos + mVelocity * Time::DeltaTime();
	transform->SetPosition(pos);

	mForce = FVector2::One;
}

void JRigidbody::LateUpdate()
{

}

void JRigidbody::Render()
{

}
