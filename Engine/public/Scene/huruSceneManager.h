#pragma once

#include "Common/huruEntity.h"

namespace huru
{
	class Scene;
	class GameObject;

	class SceneManager
	{
	public:
		template<typename T>
		static Scene* CreateScene(const wstring& name)
		{
			T* scene = new T();
			scene->SetName(name);
			scene->Initialize();

			mScene.insert(make_pair(name, scene));

			return scene;
		}

		static Scene*	LoadScene(const wstring& name);

		static void		Initialize();
		static void		Update();
		static void		LateUpdate();
		static void		Render();
		static void		Destroy();
		static void		Release();

		static Scene*				GetActiveScene()		{ return mActiveScene; }
		static Scene*				GetDontDestroyOnLoad()	{ return mDontDestroyOnLoad; }
		static vector<GameObject*>	GetGameObjects(eLayerType layer);

	private:
		static map<wstring, Scene*> mScene;
		static Scene*				mActiveScene;
		static Scene*				mDontDestroyOnLoad;
	};
}

