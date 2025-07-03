#pragma once

#include "Common/CommonInclude.h"


class UUIBase;

class UUIManager
{
public:
	using UIFactoryFunc = function<UUIBase* ()>;

	static void		Initialize();
	static void		Update();
	static void		LateUpdate();
	static void		Render();
	static void		Release();

	static void		Push(EUIType type);
	static void		Pop(EUIType type);

	static void		RegisterUIFactory(EUIType type, UIFactoryFunc factory);

private:
	static void		OnLoad(EUIType type);
	static void		OnComplete(UUIBase* addUI);
	static void		OnFail();

private:
	static unordered_map<EUIType, UIFactoryFunc>		mUIFactories;
	static unordered_map<EUIType, UUIBase*>				mUIs;
	static stack<UUIBase*>								mUIBases;
	static queue<EUIType>								mRequestUIQueue;
	static UUIBase*										mActiveUI;
};



