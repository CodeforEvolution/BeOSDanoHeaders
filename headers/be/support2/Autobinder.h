#ifndef _SUPPORT2_AUTOBINDER_H_
#define _SUPPORT2_AUTOBINDER_H_

#include <support2/Value.h>

namespace B {
namespace Private {

class BAutobinderDummiClass
{
	virtual void Method();
};

typedef void (BAutobinderDummiClass::*BSomeMethod)();

} } // namespace B::Private


namespace B {
namespace Support2 {

struct BPointerToMemberFunction
{
	short delta;				// offset of this pointer
	short index;				//	index into vtable  or negative if not virtual
	union {
		int32 func;				// address of nonvirtual function
		short offset;			// offset if vtable pointer
	};
};

typedef struct BParameterInfo
{
	uint32 type;	// if type == 0 then this is not a primitive type
};

struct BEffectMethodDef
{
	BParameterInfo					returnType;
	uint32							paramCount;
	const BParameterInfo			*paramTypes;
	union {
		B::Private::BSomeMethod		pointer;
		BPointerToMemberFunction	pmf;
	};
};

struct BAutobinderDef
{
	const BEffectMethodDef	*put;
	const BEffectMethodDef	*get;
	const BEffectMethodDef	*invoke;
	int32					classOffset;	// B_FIND_CLASS_OFFSET(LSuck, ISuck)
};

/*!
	Unmarshall the parameters from the BValue,
	call the function, and put the return value
	from that function into rv

	Object must be the base class where the virtual
	method you're trying to call was originally
	declared.
*/
status_t
InvokeAutobinderFunction(	const BEffectMethodDef	* def,
							void					* object,
							const BValue			& args,
							BValue					* returnValue
							);

/*!
	Marshall the parameters, make the remote
	binder call, and unmarshall the return
	value.
*/
void
PerformRemoteBinderCall(	int						ebp,
							const BEffectMethodDef	*def,
							unsigned char			*returnValue,
							IBinder::sptr_ref		remote,
							BValue					binding,
							uint32					which
							);


} } // namespace B::Support2


typedef void (*untyped_func)(void);

/*!
	Use this macro to create a static array of func_info structs.
	It's not really necessary, but it does the casting for you.
	
XXX Put example here.
*/
#define B_FUNC_INFO(method)		{(B::Private::BSomeMethod)&(method)}


/*!
	
*/
#define B_FIND_BINDER_I_CLASS_OFFSET(L) ((((int)static_cast<IInterface*>((L*)10))-10)) // use 10-10 because 0 is special cased for NULL


/*!
	
*/
#define B_IMPLEMENT_MARSHAL_PLAN(def, returnValue, remote, binding, which) \
		{ \
			int ebp;									\
			__asm__ __volatile__ ("	movl %%ebp, %0;" :"=r"(ebp) );		\
			PerformRemoteBinderCall(ebp, (def), (unsigned char *)(returnValue), (remote), (binding), (which));	\
		}


#endif // _SUPPORT2_AUTOBINDER_H_
