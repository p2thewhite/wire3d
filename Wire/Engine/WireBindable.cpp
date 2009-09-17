#include "WireBindable.h"

using namespace Wire;

//----------------------------------------------------------------------------
Bindable::Bindable()
{
	// TODO: resize array?
}

//----------------------------------------------------------------------------
Bindable::~Bindable()
{
}

//----------------------------------------------------------------------------
UInt Bindable::GetInfoQuantity() const
{
	return mInfoArray.GetQuantity();
}

//----------------------------------------------------------------------------
UInt Bindable::GetInfoQuantity(Renderer* pUser) const
{
	UInt quantity = 0;
	for (UInt i = 0; i < mInfoArray.GetQuantity(); i++)
	{
		const Info& rInfo = mInfoArray[i];
		if (rInfo.User == pUser)
		{
			quantity++;
		}
	}

	return quantity;
}

//----------------------------------------------------------------------------
ResourceIdentifier* Bindable::GetIdentifier(Renderer* pUser) const
{
	for (UInt i = 0; i < mInfoArray.GetQuantity(); i++)
	{
		const Info& rInfo = mInfoArray[i];
		if (rInfo.User == pUser)
		{
			return rInfo.ID;
		}
	}

	// The resource is not yet bound to the renderer.
	return NULL;
}

//----------------------------------------------------------------------------
ResourceIdentifier* Bindable::GetIdentifier(UInt i, Renderer* pUser) const
{
	if (i < mInfoArray.GetQuantity())
	{
		const Info& rInfo = mInfoArray[i];
		if (rInfo.User == pUser)
		{
			return rInfo.ID;
		}
	}

	// The resource is not yet bound to the renderer.
	return 0;
}

//----------------------------------------------------------------------------
void Bindable::Release()
{
	while (mInfoArray.GetQuantity() > 0)
	{
		Info& rInfo = mInfoArray[0];
		(rInfo.User->*rInfo.Release)(this);
	}
}

//----------------------------------------------------------------------------
void Bindable::OnLoad(Renderer* pUser, Renderer::ReleaseFunction release,
	ResourceIdentifier* pID)
{
	Info info;
	info.User = pUser;
	info.Release = release;
	info.ID = pID;
	mInfoArray.Append(info);
}

//----------------------------------------------------------------------------
void Bindable::OnRelease(Renderer* pUser, ResourceIdentifier* pID)
{
	UInt quantity = mInfoArray.GetQuantity();

	for (UInt i = 0; i < quantity; i++)
	{
		const Info& rInfo = mInfoArray[i];
		if (rInfo.User == pUser && rInfo.ID == pID)
		{
			// Move the last array element to the current slot, if necessary.
			if (i < --quantity)
			{
				mInfoArray[i] = mInfoArray[quantity];
			}

			// Remove the last array element.
			mInfoArray.Remove(mInfoArray.GetQuantity() - 1);
			return;
		}
	}
}
