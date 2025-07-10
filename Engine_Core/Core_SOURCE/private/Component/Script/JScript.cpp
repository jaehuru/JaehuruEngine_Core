#include "Component/Script/JScript.h"
#include "Component/Collider/JCollider.h"


JScript::JScript() :
	JComponent(EComponentType::JScript)
{

}

JScript::~JScript()
{

}

void JScript::Initialize()
{

}

void JScript::Update()
{

}

void JScript::LateUpdate()
{

}

void JScript::Render()
{

}

void JScript::Serialize(json& jsonObject) const
{
    JComponent::Serialize(jsonObject);
}

void JScript::Deserialize(const json& jsonObject)
{
    JComponent::Deserialize(jsonObject);
}

void JScript::OnCollisionEnter(JCollider* other)
{

}

void JScript::OnCollisionStay(JCollider* other)
{

}

void JScript::OnCollisionExit(JCollider* other)
{

}