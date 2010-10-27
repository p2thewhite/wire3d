//----------------------------------------------------------------------------
#define WIRE_DECLARE_RTTI                                                    \
public:                                                                      \
	static const Rtti TYPE;                                                  \
	virtual const Rtti& GetType() const { return TYPE; }

//----------------------------------------------------------------------------
#define WIRE_IMPLEMENT_RTTI(className, baseClassName)                        \
const Rtti className::TYPE(&baseClassName::TYPE)
