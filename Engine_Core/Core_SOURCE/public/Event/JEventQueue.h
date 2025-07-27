#pragma once

#include "Event/IEvent.h"

class JEventQueue
{
public:
	JEventQueue();
	~JEventQueue();

	template<typename T>
	void RegisterHandler(function<bool(T&)> handler)
	{
		mHandlers[T::GetStaticType()] = [handler](IEvent& e) -> bool 
			{
				return handler(static_cast<T&>(e));
			};
	}

	void Push(IEvent* event)
	{
		mQueue.push(event);
	}

	void Process()
	{
		while (!mQueue.empty())
		{
			IEvent* event = mQueue.front();
			mQueue.pop();

			EventDispatcher dispatcher(*event);

			// 등록된 핸들러 실행
			auto handler = mHandlers.find(event->GetEventType());
			if (handler != mHandlers.end())
			{
				event->Handled != handler->second(*event);
			}

			// 기본 핸들러 실행 (처리되지 않은 경우)
			if (!event->Handled && mCallback)
			{
				mCallback(*event);
			}

			delete event;
			event = nullptr;
		}
	}

	void SetCallback(const EventCallbackFn& callback)
	{
		mCallback = callback;
	}


private:
	queue<IEvent*> mQueue;
	EventCallbackFn mCallback;
	unordered_map<EEventType, HandlerCallbackFn> mHandlers;
};

