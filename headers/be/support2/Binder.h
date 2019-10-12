
#ifndef _SUPPORT2_BINDER_H_
#define _SUPPORT2_BINDER_H_

#include <support2/IBinder.h>
#include <support2/StaticValue.h>
#include <support2/SupportDefs.h>
#include <support2/Value.h>
#include <support2/IContext.h>

namespace B {
namespace Support2 {

/*-------------------------------------------------------------*/
/*------- effect_action_def helpers ---------------------------*/

struct BAutobinderDef;

struct effect_action_def {
	typedef status_t	(*put_func)		(IInterface::sptr_ref target,
										 const BValue& value);
	typedef BValue		(*get_func)		(IInterface::sptr_ref target);
	typedef BValue		(*invoke_func)	(IInterface::sptr_ref target,
										 const BValue& value);
	
	size_t				struct_size;	//!< always sizeof(effect_action_def)

	const void*			raw_key;		//!< the binder key this action is for
	const BValue&		key() const;	//!< convenience for retrieving key
	BAutobinderDef*		parameters;		//!< parameters for function
	
	put_func			put;			//!< function for put action, or NULL
	get_func			get;			//!< function for get action, or NULL
	invoke_func			invoke;			//!< function for invoke action, or NULL
};

//! Flags for execute_effect()
enum {
	B_ACTIONS_SORTED_BY_KEY		= 0x00000001
};

enum {
	B_NO_ACTION					= 0x00000000,
	B_INVOKE_ACTION				= 0x00000001,
	B_GET_ACTION,
	B_PUT_ACTION
};

status_t execute_effect(IInterface::sptr_ref target,
						const BValue &in, const BValue &inBindings,
						const BValue &outBindings, BValue *out,
						const effect_action_def* actions, size_t num_actions,
						uint32 flags = 0);
												
/*-------------------------------------------------------------*/
/*------- BBinder Implementation ------------------------------*/

class BBinder : public IBinder
{
	public:
		virtual IContext::sptr			Context() const;
		
		virtual	BValue					Inspect(const BValue &which,
												uint32 flags = 0);
		virtual	status_t				Link(	IBinder::sptr_ref target,
												const BValue &bindings,
												uint32 flags = 0);
		virtual	status_t				Unlink(	IBinder::wptr_ref target,
												const BValue &bindings,
												uint32 flags = 0);
		virtual	status_t				Effect(	const BValue &in,
												const BValue &inBindings,
												const BValue &outBindings,
												BValue *out);

		virtual	status_t				Transact(	uint32 code,
													BParcel& data,
													BParcel* reply = NULL,
													uint32 flags = 0);
		
		virtual	bool					IsBinderAlive() const;
		virtual	status_t				PingBinder();
		
		virtual	BBinder*				LocalBinder();
		virtual	B::Private::RBinder*	RemoteBinder();
		
	protected:
										BBinder();
										BBinder(IContext::sptr_ref context);
										BBinder(const BBinder& other);
		virtual							~BBinder();

		virtual	status_t				HandleEffect(	const BValue &in,
														const BValue &inBindings,
														const BValue &outBindings,
														BValue *out);
		
		// These functions are called in response to Put(), Get(), and
		// Invoke(), respectively.  Will be removed.
		virtual	status_t				Told(BValue &in);
		virtual	status_t				Asked(const BValue &outBindings, BValue &out);
		virtual	status_t				Called(	BValue &in,
												const BValue &outBindings,
												BValue &out);
		
				bool					IsLinked() const;
		virtual	status_t				Push(const BValue &out);
		virtual	status_t				Pull(BValue *inMaskAndDefaultsOutData);

	private:
				void					BeginEffectContext(const struct EffectCache &cache);
				void					EndEffectContext();

		static	int32					gBinderTLS;
		
		struct extensions;

		extensions *	m_extensions;
		IContext::sptr	m_context;
};

/*-------------------------------------------------------------*/
/*------- LInterface Implementation ---------------------------*/

//! This is the base implementation for a local IInterface.
template<class INTERFACE>
class LInterface : public INTERFACE, public BBinder
{
	public:
		virtual	BValue					Inspect(const BValue &which, uint32 flags = 0)
			{ (void)flags; return which * BValue(descriptor,BValue::Binder(this)); };
	
	protected:
		inline							LInterface() { }
		inline							LInterface(IContext::sptr_ref context)
											: BBinder(context) { }
		inline							LInterface(const LInterface<INTERFACE>& other)
											: INTERFACE(), BBinder(other) { }
		inline virtual					~LInterface() { }
		
		virtual	IBinder::sptr			AsBinderImpl()			{ return this; }
		virtual	IBinder::const_sptr		AsBinderImpl() const	{ return this; }
};

/*-------------------------------------------------------------*/
/*------- RAtom Implementation --------------------------------*/

//! This is the BAtom protocol implementation for a remote interface
/*!	You don't normally use this directly -- it is included as part of the
	RInterface<> implementation below.
*/
class RAtom : public virtual BAtom
{
	protected:
										RAtom(IBinder::sptr_ref o);
		virtual							~RAtom();
		virtual	status_t				Acquired(const void* id);
		virtual	status_t				Released(const void* id);
		virtual	status_t				AcquireAttempted(const void* id);
		virtual	status_t				DeleteAtom(const void* id);
		
		inline	IBinder*				Remote()				{ return m_remote; }
		// NOTE: This removes constness from the remote binder.
		inline	IBinder*				Remote() const			{ return m_remote; }
				
	private:
				IBinder* const			m_remote;
				int32					m_state;
				size_t					_reserved;
};

/*-------------------------------------------------------------*/
/*------- RInterface Implementation ---------------------------*/

//! This is the base implementation for a remote IInterface.
template<class INTERFACE>
class RInterface : public INTERFACE, public RAtom
{
	protected:
		inline							RInterface(IBinder::sptr_ref o)	: RAtom(o) { }
		inline virtual					~RInterface() { }
		
		virtual	IBinder::sptr			AsBinderImpl()
											{ return IBinder::sptr(Remote()); }
		virtual	IBinder::const_sptr		AsBinderImpl() const
											{ return IBinder::const_sptr(Remote()); }
};

/*-------------------------------------------------------------*/
/*---- No user serviceable parts after this -------------------*/

inline const BValue& effect_action_def::key() const
{
	return *reinterpret_cast<const BValue*>(raw_key);
}

} } // namespace B::Support2

#endif	/* _SUPPORT2_BINDER_H_ */
