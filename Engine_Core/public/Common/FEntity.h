#pragma once

#include "CommonInclude.h"


class FEntity
{
public:
	FEntity();
	virtual ~FEntity();


	wstring&	GetName() 						{ return mName; }

	void		SetName(const wstring& name)	{ mName = name; }

private:
	wstring		mName;
};



