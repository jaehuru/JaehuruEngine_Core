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
	
}

void JRigidbody::LateUpdate()
{

}

void JRigidbody::Render()
{

}

void JRigidbody::Serialize(json& jsonObject) const
{
    JComponent::Serialize(jsonObject);
    jsonObject["Ground"] = mbGround;
    jsonObject["Mass"] = mMass;
    jsonObject["Friction"] = mFriction;
    jsonObject["ForceX"] = mForce.x;
    jsonObject["ForceY"] = mForce.y;
    jsonObject["AccelationX"] = mAccelation.x;
    jsonObject["AccelationY"] = mAccelation.y;
    jsonObject["VelocityX"] = mVelocity.x;
    jsonObject["VelocityY"] = mVelocity.y;
    jsonObject["LimitedVelocityX"] = mLimitedVelocity.x;
    jsonObject["LimitedVelocityY"] = mLimitedVelocity.y;
    jsonObject["GravityX"] = mGravity.x;
    jsonObject["GravityY"] = mGravity.y;
}

void JRigidbody::Deserialize(const json& jsonObject)
{
    JComponent::Deserialize(jsonObject);
    mbGround = jsonObject["Ground"];
    mMass = jsonObject["Mass"];
    mFriction = jsonObject["Friction"];
    mForce.x = jsonObject["ForceX"];
    mForce.y = jsonObject["ForceY"];
    mAccelation.x = jsonObject["AccelationX"];
    mAccelation.y = jsonObject["AccelationY"];
    mVelocity.x = jsonObject["VelocityX"];
    mVelocity.y = jsonObject["VelocityY"];
    mLimitedVelocity.x = jsonObject["LimitedVelocityX"];
    mLimitedVelocity.y = jsonObject["LimitedVelocityY"];
    mGravity.x = jsonObject["GravityX"];
    mGravity.y = jsonObject["GravityY"];
}