#ifndef INPUTDEVICE_H_
#define INPUTDEVICE_H_

#include "WireObject.h"
#include "WireRtti.h"
#include "WireInputDataBuffer.h"
#include <vector>
#include <map>

namespace Wire
{

class InputCapability;

class InputDevice : public Object
{
	WIRE_DECLARE_RTTI;

public:
	InputDevice();
	virtual ~InputDevice();

	Bool HasCapability(const Rtti& pCapabilityType) const;
	const InputDataBuffer* GetDataBuffer() const;
	void SetDataBuffer(const InputDataBuffer* pInputData);
	const std::vector<const InputCapability*>& GetCapabilities() const;
	const InputCapability* GetCapability(const Rtti& pCapabilityType) const;
	virtual void SetUp() = 0;

protected:
	std::map<const Rtti*, InputCapability*> mCapabilitiesByType;

	void RegisterCapability(InputCapability* pInputCapability);

private:
	const InputDataBuffer* mpDataBuffer;

};


}

#endif // INPUTDEVICE_H_
