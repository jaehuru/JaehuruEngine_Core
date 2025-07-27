#pragma once

#include "Common/IEntity.h"


class JScene;
class AActor;

class JSceneManager
{
public:
	template<typename T>
	static JScene* CreateScene(const wstring& name)
	{
		T* scene = new T();
		mScene.insert(make_pair(name, scene));

		scene->SetName(name);
		scene->Initialize();

		return scene;
	}

	static bool		SetActiveScene(const wstring& name);
	static JScene*	LoadScene(const wstring& name);

	static void		Initialize();
	static void		Update();
	static void		LateUpdate();
	static void		Render();
	static void		EndOfFrame();
	static void		Release();

    static void		Serialize(json& jsonObject);
    static void		Deserialize(const json& jsonObject);

	static AActor*	FindActorByName(const wstring& name);

	static JScene*				GetActiveScene()		{ return mActiveScene; }
	static JScene*				GetDontDestroyOnLoad()	{ return mDontDestroyOnLoad; }
	static vector<AActor*>		GetActors(ELayerType layer);

private:
	static map<wstring, JScene*>	mScene;
	static JScene*					mActiveScene;
	static JScene*					mDontDestroyOnLoad;
};



