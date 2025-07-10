#include "Component/Camera/JCamera.h"
#include "Actor/AActor.h"
#include "Component/Transform/JTransform.h"
#include "HighLevelInterface/JApplication.h"
#include "Scene/JSceneManager.h"

extern JApplication application;

FMatrix JCamera::ViewMatrix			= FMatrix::Identity;
FMatrix JCamera::ProjectionMatrix	= FMatrix::Identity;

JCamera::JCamera() :
	JComponent(EComponentType::JCamera),
	mProjectionType(EProjectionType::Perspective),
	mViewMatrix(FMatrix::Identity),
	mProjectionMatrix(FMatrix::Identity),
	mAspectRatio(0.0f),
	mNear(1.0f),
	mFar(1000.0f),
	mSize(1.0f)
{

}

JCamera::~JCamera()
{

}

void JCamera::Initialize()
{

}

void JCamera::Update()
{

}

void JCamera::LateUpdate()
{
    CreateViewMatrix();
    CreateProjectionMatrix(mProjectionType);

    ViewMatrix = mViewMatrix;
    ProjectionMatrix = mProjectionMatrix;
}

void JCamera::Render()
{

}

void JCamera::Serialize(json& jsonObject) const
{
    
}

void JCamera::Deserialize(const json& jsonObject)
{
	
}

void JCamera::CreateViewMatrix()
{
	JTransform* transform = GetOwner()->GetComponent<JTransform>();

	const FVector3 pos = transform->GetPosition();
	const FVector3 up = transform->Up();
	const FVector3 forward = transform->Foward();

	mViewMatrix = FMatrix::CreateLookToLH(pos, forward, up);
}

void JCamera::CreateProjectionMatrix(EProjectionType type)
{
	RECT winRect;
	GetClientRect(application.GetWindow().GetHwnd(), &winRect);
	float width = (float)(winRect.right - winRect.left);
	float height = (float)(winRect.bottom - winRect.top);
	mAspectRatio = width / height;

	switch (type)
	{
	case EProjectionType::Perspective:
		mProjectionMatrix = FMatrix::CreatePerspectiveFieldOfViewLH(XM_2PI / 6.0f, mAspectRatio, mNear, mFar);
		break;
	case EProjectionType::Orthographic:
		mProjectionMatrix = FMatrix::CreateOrthographicLH(width / mSize, height / mSize, mNear, mFar);
		break;
	}
}
