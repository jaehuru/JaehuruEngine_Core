#include "Component/Camera/JCamera.h"
#include "Actor/AActor.h"
#include "Component/Transform/JTransform.h"
#include "HighLevelInterface/JApplication.h"
#include "Scene/JSceneManager.h"

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

void JCamera::Serialize(json& jsonObject) const
{
    JComponent::Serialize(jsonObject);
    jsonObject["DistanceX"] = mDistance.x;
    jsonObject["DistanceY"] = mDistance.y;
    jsonObject["ResolutionX"] = mResolution.x;
    jsonObject["ResolutionY"] = mResolution.y;
    jsonObject["LookPositionX"] = mLookPosition.x;
    jsonObject["LookPositionY"] = mLookPosition.y;

    if (mTarget)
    {
        wstring_convert<codecvt_utf8<wchar_t>> conv;
        jsonObject["TargetActorName"] = conv.to_bytes(mTarget->GetName());
    }
    else
    {
        jsonObject["TargetActorName"] = "";
    }
}

void JCamera::Deserialize(const json& jsonObject)
{
    JComponent::Deserialize(jsonObject);
    mDistance.x = jsonObject["DistanceX"];
    mDistance.y = jsonObject["DistanceY"];
    mResolution.x = jsonObject["ResolutionX"];
    mResolution.y = jsonObject["ResolutionY"];
    mLookPosition.x = jsonObject["LookPositionX"];
    mLookPosition.y = jsonObject["LookPositionY"];

    string targetActorNameStr = jsonObject.value("TargetActorName", "");
    if (!targetActorNameStr.empty())
    {
        wstring_convert<codecvt_utf8<wchar_t>> conv;
        mTargetActorNameTemp = conv.from_bytes(targetActorNameStr);
    }
}

void JCamera::LinkTargetActor()
{
    if (!mTargetActorNameTemp.empty())
    {
        AActor* targetActor = JSceneManager::FindActorByName(mTargetActorNameTemp);
        if (targetActor)
        {
            SetTarget(targetActor);
        }
        // Clear the temporary name after linking
        mTargetActorNameTemp.clear();
    }
}