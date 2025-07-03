#include "Component/Camera/JCamera.h"
#include "Actor/AActor.h"
#include "Component/Transform/JTransform.h"
#include "HighLevelInterface/JApplication.h"

extern JApplication application;


JCamera::JCamera() :
	JComponent(EComponentType::JCamera),
	mTarget(nullptr),
	mDistance(FVector2::Zero),
	mResolution(FVector2::Zero),
	mLookPosition(FVector2::Zero)
{

}

JCamera::~JCamera()
{

}

void JCamera::Initialize()
{
	mResolution.x = (float)application.GetWidth();
	mResolution.y = (float)application.GetHeight();
}

void JCamera::Update()
{
	if (mTarget)
	{
		JTransform* tr = mTarget->GetComponent<JTransform>();
		mLookPosition = tr->GetPosition();
	}
	else
	{
		JTransform* cameraTr = GetOwner()->GetComponent<JTransform>();
		mLookPosition = cameraTr->GetPosition();
	}

	mDistance = mLookPosition - (mResolution / 2.f);
}

void JCamera::LateUpdate()
{

}

void JCamera::Render()
{

}
