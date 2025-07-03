#include "UI/UUIManager.h"
#include "UI/UUIBase.h"
#include "UI/UHUD.h"
#include "UI/UButton.h"


unordered_map<EUIType, UUIManager::UIFactoryFunc> UUIManager::mUIFactories = { };
unordered_map<EUIType, UUIBase*> UUIManager::mUIs = { };
stack<UUIBase*> UUIManager::mUIBases = { };
queue<EUIType> UUIManager::mRequestUIQueue = { };
UUIBase* UUIManager::mActiveUI = nullptr;

void UUIManager::Initialize()
{
	RegisterUIFactory(EUIType::HUD, []() { return new UHUD(); });
	RegisterUIFactory(EUIType::Button, []() { return new UButton(); });
}

void UUIManager::Update()
{
	stack<UUIBase*> uiBases = mUIBases;
	while (!uiBases.empty())
	{
		UUIBase* uiBase = uiBases.top();
		if (uiBase)
		{
			uiBase->Update();
			uiBases.pop();
		}
	}

	if (mRequestUIQueue.size() > 0)
	{
		EUIType requestUI = mRequestUIQueue.front();
		mRequestUIQueue.pop();
		OnLoad(requestUI);
	}
}

void UUIManager::LateUpdate()
{
	stack<UUIBase*> uiBases = mUIBases;
	while (!uiBases.empty())
	{
		UUIBase* uiBase = uiBases.top();
		if (uiBase)
		{
			uiBase->LateUpdate();
			uiBases.pop();
		}
	}

	if (mRequestUIQueue.size() > 0)
	{
		EUIType requestUI = mRequestUIQueue.front();
		mRequestUIQueue.pop();
		OnLoad(requestUI);
	}
}

void UUIManager::Render()
{
	stack<UUIBase*> uiBases = mUIBases;
	while (!uiBases.empty())
	{
		UUIBase* uiBase = uiBases.top();
		if (uiBase)
		{
			uiBase->Render();
			uiBases.pop();
		}
	}

	if (mRequestUIQueue.size() > 0)
	{
		EUIType requestUI = mRequestUIQueue.front();
		mRequestUIQueue.pop();
		OnLoad(requestUI);
	}
}

void UUIManager::Release()
{
	for (auto iter : mUIs)
	{
		delete iter.second;
		iter.second = nullptr;
	}
	mUIs.clear();
}

void UUIManager::Push(EUIType type)
{
	mRequestUIQueue.push(type);
}

void UUIManager::Pop(EUIType type)
{
	if (mUIBases.size() <= 0)
		return;

	stack<UUIBase*> tempStack;

	UUIBase* uibase = nullptr;
	while (mUIBases.size() > 0)
	{
		uibase = mUIBases.top();
		mUIBases.pop();

		if (uibase->GetType() != type)
		{
			tempStack.push(uibase);
			continue;
		}

		if (uibase->IsFullScreen())
		{
			stack<UUIBase*> uiBases = mUIBases;
			while (!uiBases.empty())
			{
				UUIBase* uiBase = uiBases.top();
				uiBases.pop();
				if (uiBase)
				{
					uiBase->Active();
					break;
				}
			}
		}

		uibase->UIClear();
	}

	while (tempStack.size() > 0)
	{
		uibase = tempStack.top();
		tempStack.pop();
		mUIBases.push(uibase);
	}
}

void UUIManager::RegisterUIFactory(EUIType type, UIFactoryFunc factory)
{
	mUIFactories[type] = factory;
}

void UUIManager::OnLoad(EUIType type)
{
	auto itFind = mUIs.find(type);
	if (itFind != mUIs.end())
	{
		OnComplete(itFind->second);
		return;
	}

	auto it = mUIFactories.find(type);
	if (it == mUIFactories.end())
	{
		OnFail();
		return;
	}
	UUIBase* ui = it->second();
	mUIs.insert(make_pair(type, ui));
	OnComplete(ui);
}

void UUIManager::OnComplete(UUIBase* addUI)
{
	if (addUI == nullptr)
		return;

	addUI->Initialize();
	addUI->Active();
	addUI->Update();

	// 만약에 현재 추가된 ui가 전체화면이라면
	// 전체화면인 ui 말고 나머지를 전부 비활성화
	if (addUI->IsFullScreen())
	{
		stack<UUIBase*> uiBases = mUIBases;
		while (!uiBases.empty())
		{
			UUIBase* uiBase = uiBases.top();
			uiBases.pop();
			if (uiBase)
			{
				uiBase->InActive();
			}
		}
	}

	mUIBases.push(addUI);
	mActiveUI = nullptr;

}

void UUIManager::OnFail()
{
	mActiveUI = nullptr;
}
