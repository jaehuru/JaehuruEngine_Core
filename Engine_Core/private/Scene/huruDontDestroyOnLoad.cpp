#include "Scene/huruDontDestroyOnLoad.h"

namespace huru
{
	DontDestroyOnLoad::DontDestroyOnLoad()
	{
		
	}

	DontDestroyOnLoad::~DontDestroyOnLoad()
	{
		
	}

	void DontDestroyOnLoad::Initialize()
	{
		Scene::Initialize();
	}

	void DontDestroyOnLoad::Update()
	{
		Scene::Update();
	}

	void DontDestroyOnLoad::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void DontDestroyOnLoad::Render()
	{
		Scene::Render();
	}

	void DontDestroyOnLoad::OnEnter()
	{
		Scene::OnEnter();
	}

	void DontDestroyOnLoad::OnExit()
	{
		Scene::OnExit();
	}
}