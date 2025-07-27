#include "Scene/JSceneManager.h"
#include "Scene/JDontDestroyOnLoad.h"
#include "Component/Camera/JCamera.h"
#include "Layer/JLayer.h"
#include "Actor/AActor.h"
#include "Event/FActorEvent.h"



map<wstring, JScene*> JSceneManager::mScene = {};
JScene* JSceneManager::mActiveScene = nullptr;
JScene* JSceneManager::mDontDestroyOnLoad = nullptr;
JEventQueue JSceneManager::mEventQueue;

void JSceneManager::Initialize()
{
	mDontDestroyOnLoad = CreateScene<JDontDestroyOnLoad>(L"JDontDestroyOnLoad");
    InitializeEventHandlers();
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

void JSceneManager::EndOfFrame()
{
    mActiveScene->EndOfFrame();
    mDontDestroyOnLoad->EndOfFrame();

    mEventQueue.Process();
}

void JSceneManager::Release()
{
    for (auto& iter : mScene)
    {
        delete iter.second;
        iter.second = nullptr;
    }
}

void JSceneManager::InitializeEventHandlers()
{
    // 이벤트 핸들러 등록
    mEventQueue.RegisterHandler<ActorCreatedEvent>([](ActorCreatedEvent& e) -> bool
        {
            JSceneManager::ActorCreated(e.GetActor(), e.GetScene());
            return true;
        });

    mEventQueue.RegisterHandler<ActorDestroyedEvent>([](ActorDestroyedEvent& e) -> bool
        {
            JSceneManager::ActorDestroyed(e.GetActor(), e.GetScene());
            return true;
        });

    // 기본 핸들러 등록
    mEventQueue.SetCallback([](IEvent& e)
        {
            std::cout << "[Application] Unhandled Event: " << e.ToString() << std::endl;
        });
}

void JSceneManager::ActorCreated(AActor* actor, JScene* scene)
{
    scene->AddActor(actor, actor->GetLayerType());
}

void JSceneManager::ActorDestroyed(AActor* actor, JScene* scene)
{
    scene->EraseActor(actor);
}

JScene* JSceneManager::LoadScene(const wstring& name)
{
    if (mActiveScene)
        mActiveScene->OnExit();


    if (!SetActiveScene(name))
        return nullptr;

    mActiveScene->OnEnter();

    return mActiveScene;
}

AActor* JSceneManager::FindActorByName(const wstring& name)
{
    for (const auto& scenePair : mScene)
    {
        JScene* scene = scenePair.second;
        for (UINT i = 0; i < (UINT)ELayerType::Max; ++i)
        {
            JLayer* layer = scene->GetLayer((ELayerType)i);
            if (layer == nullptr) continue;

            for (AActor* actor : layer->GetActors())
            {
                if (actor->GetName() == name)
                {
                    return actor;
                }
            }
        }
    }
    return nullptr;
}

void JSceneManager::Serialize(json& jsonObject)
{
    json scenesArray = json::array();
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;

    for (const auto& pair : mScene)
    {
        json sceneJson;
        sceneJson["Name"] = converter.to_bytes(pair.first);
        pair.second->Serialize(sceneJson);
        scenesArray.push_back(sceneJson);
    }
    jsonObject["Scenes"] = scenesArray;

    if (mDontDestroyOnLoad)
    {
        json dontDestroyJson;
        mDontDestroyOnLoad->Serialize(dontDestroyJson);
        jsonObject["DontDestroyOnLoad"] = dontDestroyJson;
    }
}

void JSceneManager::Deserialize(const json& jsonObject)
{
    Release();

    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;

    const json& scenesArray = jsonObject["Scenes"];
    for (const auto& sceneJson : scenesArray)
    {
        wstring name = converter.from_bytes(sceneJson["Name"]);
        JScene* newScene = nullptr;

        if (name == L"JDontDestroyOnLoad")
        {
            newScene = new JDontDestroyOnLoad();
        }
        else
        {
            newScene = new JScene();
        }
        
        newScene->Deserialize(sceneJson);
        mScene.insert(make_pair(name, newScene));
    }

    if (jsonObject.contains("DontDestroyOnLoad"))
    {
        const json& dontDestroyJson = jsonObject["DontDestroyOnLoad"];
        wstring name = converter.from_bytes(dontDestroyJson["Name"]);
        if (name == L"JDontDestroyOnLoad")
        {
            mDontDestroyOnLoad = new JDontDestroyOnLoad();
            mDontDestroyOnLoad->Deserialize(dontDestroyJson);
        }
    }
}

vector<AActor*> JSceneManager::GetActors(ELayerType layer)
{
    vector<AActor*> actors = mActiveScene->GetLayer(layer)->GetActors();
    vector<AActor*> dontDestroyOnLoad = mDontDestroyOnLoad->GetLayer(layer)->GetActors();

    actors.insert(actors.end(), dontDestroyOnLoad.begin(), dontDestroyOnLoad.end());

    return actors;
}

bool JSceneManager::SetActiveScene(const wstring& name)
{
    map<wstring, JScene*>::iterator iter = mScene.find(name);

    if (iter == mScene.end())
        return false;

    mActiveScene = iter->second;
    return true;
}


