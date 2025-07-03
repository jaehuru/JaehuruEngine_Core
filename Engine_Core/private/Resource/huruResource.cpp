#include "Resource/huruResource.h"

namespace huru
{
	Resource::Resource(eResourceType type) :
		mType(type)
	{

	}

	HRESULT Resource::Load(const wstring& path)
	{
		return E_NOTIMPL;
	}
}