#pragma once
#ifndef WIRERESOURCEIDENTIFIER_H
#define WIRERESOURCEIDENTIFIER_H

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ ResourceIdentifier
{

public:
	// Abstract base class. The destructor is *NOT* virtual so that the
	// derived-class destructors hide it. This is intentional to avoid a
	// virtual function table pointer, a safe thing here because the base
	// class has no data. This allows the derived classes that represent
	// vertex buffer information to store the input attributes first, and
	// allow typecasting of the following form.
	//
	//   class VBufferIdentifier : public ResourceIdentifier
	//   {
	//   public:  Attributes IAttr;
	//   }
	//   VBufferIdentifier* pkID = <some identifier>;
	//   Attributes& rkIAttr = *(Attributes*)pkID;

	~ResourceIdentifier() {/**/}

protected:
	ResourceIdentifier() {/**/}
};

}

#endif
