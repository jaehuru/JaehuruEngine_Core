#pragma once

#include "Common/CommonInclude.h"

namespace huru
{
	class Time
	{
	public:
		static void		Initialize();
		static void		Update();
		static void		Render();

		__forceinline static float DeltaTime() { return DeltaTimeValue; }

	private:
		static LARGE_INTEGER		CPUFrequency;
		static LARGE_INTEGER		PrevFrequency; 
		static LARGE_INTEGER		CurrentFrequency; 
		static float				DeltaTimeValue;
	};
}

