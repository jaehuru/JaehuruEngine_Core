#include "Component/Transform/JTransform.h"


JTransform::JTransform() :
	JComponent(EComponentType::JTransform),
	mPosition(FVector2::Zero),
	mScale(FVector2::One),
	mRotation(0.f)
{

}

JTransform::~JTransform()
{

}

void JTransform::Initialize()
{

}

void JTransform::Update()
{

}

void JTransform::LateUpdate()
{

}

void JTransform::Render()
{

}
