#pragma once

#include "Common/IEntity.h"
#include "Event/JEventQueue.h"


class JScene;
class AActor;
class IEvent;

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

	static void		Initialize();
	static void		Update();
	static void		LateUpdate();
	static void		Render();
	static void		EndOfFrame();
	static void		Release();

	static void			InitializeEventHandlers();
	static void			ActorCreated(AActor* actor, JScene* scene);
	static void			ActorDestroyed(AActor* actor, JScene* scene);

	static JScene*	LoadScene(const wstring& name);
	static AActor*	FindActorByName(const wstring& name);

    static void		Serialize(json& jsonObject);
    static void		Deserialize(const json& jsonObject);

	static void		PushEvent(IEvent* e) { mEventQueue.Push(e); }

	static vector<AActor*>		GetActors(ELayerType layer);
	static JScene*				GetActiveScene()		{ return mActiveScene; }
	static JScene*				GetDontDestroyOnLoad()	{ return mDontDestroyOnLoad; }

	static bool					SetActiveScene(const wstring& name);

private:
	static map<wstring, JScene*>	mScene;
	static JScene*					mActiveScene;
	static JScene*					mDontDestroyOnLoad;
	static JEventQueue				mEventQueue;
};



