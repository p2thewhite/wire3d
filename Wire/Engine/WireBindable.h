#pragma once
#ifndef WIREBINDABLE_H
#define WIREBINDABLE_H

#include "WireRenderer.h"
#include "WireResourceIdentifier.h"
#include "../Foundation/WireTArray.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Bindable
{

public:
	Bindable();
	~Bindable();

	// Use this function when the resource has a unique representation in
	// VRAM (all resources except for vertex buffers).
	ResourceIdentifier* GetIdentifier(Renderer* pUser) const;

	// Use these functions when the resource has multiple representations in
	// VRAM (vertex buffers).
	UInt GetInfoQuantity() const;
	UInt GetInfoQuantity(Renderer* pUser) const;
	ResourceIdentifier* GetIdentifier(UInt i, Renderer* pUser) const;

	void Release ();

private:
	void OnLoad(Renderer* pUser, Renderer::ReleaseFunction release,
		ResourceIdentifier* pID);

	void OnRelease(Renderer* pUser, ResourceIdentifier* pID);

	struct Info
	{
		// The renderer to which the resource is bound.
		Renderer* User;

		// The renderer function to call to release the resource.
		Renderer::ReleaseFunction Release;

		// The identifier of the resource for the renderer's use.
		ResourceIdentifier* ID;
	};

	TArray<Info> mInfoArray;
};

}

#endif
