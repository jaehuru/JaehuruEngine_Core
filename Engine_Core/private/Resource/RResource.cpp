#include "Resource/RResource.h"


RResource::RResource(EResourceType type) :
	mType(type)
{

}

HRESULT RResource::Load(const wstring& path)
{
	return E_NOTIMPL;
}
