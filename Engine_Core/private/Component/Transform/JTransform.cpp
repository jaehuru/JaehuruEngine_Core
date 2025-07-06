#include "Component/Transform/JTransform.h"
#include "Graphics/RGraphics.h"
#include "Component/Camera/JCamera.h"
#include "Graphics/GPUBuffer/RConstantBuffer.h"
#include "Renderer/RRenderer.h"

JTransform::JTransform() :
	JComponent(EComponentType::JTransform),
	mWorldMatrix(FMatrix::Identity),
	mScale(FVector3::One),
	mRotation(FVector3::Zero),
	mPosition(FVector3::Zero)
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
	FMatrix scale = FMatrix::CreateScale(mScale.x, mScale.y, mScale.z);
	FMatrix rotation = FMatrix::CreateRotationX(Radian(mRotation.x));
	rotation *= FMatrix::CreateRotationY(Radian(mRotation.y));
	rotation *= FMatrix::CreateRotationZ(Radian(mRotation.z));
	FMatrix translation = FMatrix::CreateTranslation(mPosition);

	mWorldMatrix = scale * rotation * translation;

	mForward = FVector3::TransformNormal(FVector3::Forward, rotation);
	mRight = FVector3::TransformNormal(FVector3::Right, rotation);
	mUp = FVector3::TransformNormal(FVector3::Up, rotation);
}

void JTransform::Render()
{

}

void JTransform::Serialize(json& jsonObject) const
{
	
}

void JTransform::Deserialize(const json& jsonObject)
{
	
}

void JTransform::Bind()
{
	JTransformCB cbData = {};
	cbData.world = GetWorldMatrix();
	cbData.view = JCamera::GetGpuViewMatrix();
	cbData.projection = JCamera::GetGpuProjectionMatrix();

	RConstantBuffer* cb = renderer::constantBuffers[CBSLOT_TRANSFORM];

	cb->SetData(&cbData);
	cb->Bind(EShaderStage::All);
}