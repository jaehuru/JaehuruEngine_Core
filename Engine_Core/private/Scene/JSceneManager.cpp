#include "Scene/JSceneManager.h"
#include "Scene/JDontDestroyOnLoad.h"


map<wstring, JScene*> JSceneManager::mScene = {};
JScene* JSceneManager::mActiveScene = nullptr;
JScene* JSceneManager::mDontDestroyOnLoad = nullptr;

JScene* JSceneManager::LoadScene(const wstring& name)
{
	if (mActiveScene)
		mActiveScene->OnExit();


	map<wstring, JScene*>::iterator iter
		= mScene.find(name);

	if (iter == mScene.end())
		return nullptr;

	mActiveScene = iter->second;
	mActiveScene->OnEnter();

	return iter->second;
}

vector<AActor*> JSceneManager::GetGameObjects(ELayerType layer)
{
	vector<AActor*> gameObjects = mActiveScene->GetLayer(layer)->GetGameObjects();
	vector<AActor*> dontDestroyOnLoad = mDontDestroyOnLoad->GetLayer(layer)->GetGameObjects();

	gameObjects.insert(gameObjects.end(), dontDestroyOnLoad.begin(), dontDestroyOnLoad.end());

	return gameObjects;
}

void JSceneManager::Initialize()
{
	mDontDestroyOnLoad =
		CreateScene<JDontDestroyOnLoad>(L"JDontDestroyOnLoad");
}

void JSceneManager::Update()
{
	mActiveScene->Update();
	mDontDestroyOnLoad->Update();
}

void JSceneManager::LateUpdate()
{
	mActiveScene->LateUpdate();
	mDontDestroyOnLoad->LateUpdate();
}

void JSceneManager::Render()
{
	mActiveScene->Render();
	mDontDestroyOnLoad->Render();
}

void JSceneManager::Destroy()
{
	mActiveScene->Destroy();
	mDontDestroyOnLoad->Destroy();
}

void JSceneManager::Release()
{
	for (auto& iter : mScene)
	{
		delete iter.second;
		iter.second = nullptr;
	}
}
