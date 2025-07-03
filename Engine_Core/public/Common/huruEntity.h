#pragma once

#include "CommonInclude.h"

namespace huru
{
	class Entity
	{
	public:
		Entity();
		virtual ~Entity();


		wstring&	GetName() 						{ return mName; }

		void		SetName(const wstring& name)	{ mName = name; }

	private:
		wstring		mName;
	};
}


