#include "Helpers/huruTime.h"

namespace huru
{
	
	LARGE_INTEGER Time::CPUFrequency		= {};
	LARGE_INTEGER Time::PrevFrequency		= {};
	LARGE_INTEGER Time::CurrentFrequency	= {};
	float Time::DeltaTimeValue				= 0.f;

	void Time::Initialize()
	{
		QueryPerformanceFrequency(&CPUFrequency);
		QueryPerformanceCounter(&PrevFrequency);
	}

	void Time::Update()
	{
		QueryPerformanceCounter(&CurrentFrequency);

		float differenceFrequency = 
			static_cast<float>(CurrentFrequency.QuadPart - PrevFrequency.QuadPart);

		DeltaTimeValue = 
			differenceFrequency / static_cast<float>(CPUFrequency.QuadPart);

		PrevFrequency.QuadPart = CurrentFrequency.QuadPart;
	}

	void Time::Render()
	{
		
	}
}