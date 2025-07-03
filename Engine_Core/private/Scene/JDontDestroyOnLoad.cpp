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

void JDontDestroyOnLoad::OnEnter()
{
	JScene::OnEnter();
}

void JDontDestroyOnLoad::OnExit()
{
	JScene::OnExit();
}
