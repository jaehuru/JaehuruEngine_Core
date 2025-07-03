#include "Scene/huruSceneManager.h"
#include "Scene/huruDontDestroyOnLoad.h"

namespace huru
{
	map<wstring, Scene*> SceneManager::mScene = {};
	Scene* SceneManager::mActiveScene = nullptr;
	Scene* SceneManager::mDontDestroyOnLoad = nullptr;

	Scene* SceneManager::LoadScene(const wstring& name)
	{
		if (mActiveScene)
			mActiveScene->OnExit();


		map<wstring, Scene*>::iterator iter
			= mScene.find(name);

		if (iter == mScene.end())
			return nullptr;

		mActiveScene = iter->second;
		mActiveScene->OnEnter();

		return iter->second;
	}

	vector<GameObject*> SceneManager::GetGameObjects(eLayerType layer)
	{
		vector<GameObject*> gameObjects = mActiveScene->GetLayer(layer)->GetGameObjects();
		vector<GameObject*> dontDestroyOnLoad = mDontDestroyOnLoad->GetLayer(layer)->GetGameObjects();

		gameObjects.insert(gameObjects.end(), dontDestroyOnLoad.begin(), dontDestroyOnLoad.end());

		return gameObjects;
	}

	void SceneManager::Initialize()
	{
		mDontDestroyOnLoad =
			CreateScene<DontDestroyOnLoad>(L"DontDestroyOnLoad");
	}

	void SceneManager::Update()
	{
		mActiveScene->Update();
		mDontDestroyOnLoad->Update();
	}

	void SceneManager::LateUpdate()
	{
		mActiveScene->LateUpdate();
		mDontDestroyOnLoad->LateUpdate();
	}

	void SceneManager::Render()
	{
		mActiveScene->Render();
		mDontDestroyOnLoad->Render();
	}

	void SceneManager::Destroy()
	{
		mActiveScene->Destroy();
		mDontDestroyOnLoad->Destroy();
	}

	void SceneManager::Release()
	{
		for (auto& iter : mScene)
		{
			delete iter.second;
			iter.second = nullptr;
		}
	}
}