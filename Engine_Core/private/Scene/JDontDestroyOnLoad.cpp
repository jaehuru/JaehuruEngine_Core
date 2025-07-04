#include "Scene/JDontDestroyOnLoad.h"


JDontDestroyOnLoad::JDontDestroyOnLoad()
{
		
}

JDontDestroyOnLoad::~JDontDestroyOnLoad()
{
		
}

void JDontDestroyOnLoad::Initialize()
{
	JScene::Initialize();
}

void JDontDestroyOnLoad::Update()
{
	JScene::Update();
}

void JDontDestroyOnLoad::LateUpdate()
{
	JScene::LateUpdate();
}

void JDontDestroyOnLoad::Render()
{
	JScene::Render();
}

void JDontDestroyOnLoad::Serialize(json& jsonObject) const
{
    JScene::Serialize(jsonObject);
}

void JDontDestroyOnLoad::Deserialize(const json& jsonObject)
{
    JScene::Deserialize(jsonObject);
}

void JDontDestroyOnLoad::OnEnter()
{
	JScene::OnEnter();
}

void JDontDestroyOnLoad::OnExit()
{
	JScene::OnExit();
}