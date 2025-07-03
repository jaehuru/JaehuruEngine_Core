#pragma once

#include "Common/FEntity.h"


class JScene;
class AActor;

class JSceneManager
{
public:
	template<typename T>
	static JScene* CreateScene(const wstring& name)
	{
		T* scene = new T();
		scene->SetName(name);
		scene->Initialize();

		mScene.insert(make_pair(name, scene));

		return scene;
	}

	static JScene*	LoadScene(const wstring& name);

	static void		Initialize();
	static void		Update();
	static void		LateUpdate();
	static void		Render();
	static void		Destroy();
	static void		Release();

	static JScene*				GetActiveScene()		{ return mActiveScene; }
	static JScene*				GetDontDestroyOnLoad()	{ return mDontDestroyOnLoad; }
	static vector<AActor*>	GetGameObjects(ELayerType layer);

private:
	static map<wstring, JScene*> mScene;
	static JScene*				mActiveScene;
	static JScene*				mDontDestroyOnLoad;
};


