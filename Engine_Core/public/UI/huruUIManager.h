#pragma once

#include "Common/CommonInclude.h"

namespace huru
{
	class UIBase;

	class UIManager
	{
	public:
		using UIFactoryFunc = function<UIBase* ()>;

		static void		Initialize();
		static void		Update();
		static void		LateUpdate();
		static void		Render();
		static void		Release();

		static void		Push(eUIType type);
		static void		Pop(eUIType type);

		static void		RegisterUIFactory(eUIType type, UIFactoryFunc factory);

	private:
		static void		OnLoad(eUIType type);
		static void		OnComplete(UIBase* addUI);
		static void		OnFail();

	private:
		static unordered_map<eUIType, UIFactoryFunc>		mUIFactories;
		static unordered_map<eUIType, UIBase*>				mUIs;
		static stack<UIBase*>								mUIBases;
		static queue<eUIType>								mRequestUIQueue;
		static UIBase*										mActiveUI;
	};
}


