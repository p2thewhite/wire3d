//----------------------------------------------------------------------------
#define WIRE_DECLARE_RTTI                                                    \
public:                                                                      \
	static const Rtti Type;                                                  \
	virtual const Rtti& GetType() const { return Type; }

//----------------------------------------------------------------------------
#define WIRE_IMPLEMENT_RTTI(className, baseClassName)                        \
const Rtti className::Type(&baseClassName::Type)
