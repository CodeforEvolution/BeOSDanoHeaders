/***************************************************************************
//
//	File:			support2/BAtom.h
//
//	Description:	BAtom is a reference counted base class.
//					atom_ptr<> is a template class that looks like a
//					pointer to an atom and holds a primary reference on it.
//					atom_ref<> is a template class that looks like a
//					pointer to an atom and holds a secondary reference on it.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _SUPPORT2_ATOM_H
#define _SUPPORT2_ATOM_H

#include <support2/SupportDefs.h>
#include <typeinfo>

namespace std {
	struct nothrow_t;
}

namespace B { namespace Private { struct atom_debug; } }

namespace B {
namespace Support2 {

class ITextOutput;
class BAtomTracker;

/**************************************************************************************/

//!	Use this macro inside of a class to create standard atom_ptr<> and
//!	atom_ref<> typedefs.
#define B_STANDARD_ATOM_TYPEDEFS(cname)												\
		/* Strong pointers */														\
		typedef	::B::Support2::atom_ptr<cname>					sptr;				\
		typedef	::B::Support2::atom_ptr<const cname>			const_sptr;			\
		typedef	const ::B::Support2::atom_ptr<cname> &			sptr_ref;			\
		typedef	const ::B::Support2::atom_ptr<const cname> &	const_sptr_ref;		\
		/* Weak pointers */															\
		typedef	::B::Support2::atom_ref<cname>					wptr;				\
		typedef	::B::Support2::atom_ref<const cname>			const_wptr;			\
		typedef	const ::B::Support2::atom_ref<cname> &			wptr_ref;			\
		typedef	const ::B::Support2::atom_ref<const cname> &	const_wptr_ref;		\
		/* Old type definions.  Don't use! */										\
		typedef	::B::Support2::atom_ref<cname>					ref;				\
		typedef	::B::Support2::atom_ref<const cname>			const_ref;			\
		typedef	::B::Support2::atom_ptr<cname>					ptr;				\
		typedef	::B::Support2::atom_ptr<const cname>			const_ptr;			\
		typedef	const ::B::Support2::atom_ptr<cname> &			arg;				\
		typedef	const ::B::Support2::atom_ptr<const cname> &	const_arg;			\

/**************************************************************************************/

//!	Flags for BAtom debugging reports
enum {
	B_ATOM_REPORT_FORCE_LONG		= 0x0001,	//!< Print full stack crawls.
	B_ATOM_REPORT_FORCE_SHORT		= 0x0002,	//!< Print one-line stack crawls.
	B_ATOM_REPORT_FORCE_SUMMARY		= 0x0003,	//!< Print only holder summary.
	B_ATOM_REPORT_FORCE_MASK		= 0x000f,	//!< Mask for report type.
	
	B_ATOM_REPORT_REMOVE_HEADER		= 0x0010	//!< Don't print atom header.
};

//!	Base class for a reference-counted object.
class BAtom
{
public:
			//@{
			//!	BAtom must use its own new and delete operators.
			void*			operator new(size_t size);
			void*			operator new(size_t size, const std::nothrow_t&);
			void			operator delete(void* ptr, size_t size);
			//@}
			
			//!	Increment the atom's strong pointer count.  The first time
			//!	you call this will result in Acquired() being called.
			int32			IncStrong(const void* id = NULL) const;
			
			//!	Decrement the atom's strong pointer count.  If this is the
			//!	last strong pointer on it, Released() will be called.
			int32			DecStrong(const void* id = NULL) const;
			
			// Backwards compatibility.
	inline	int32			Acquire(const void* id = NULL) const { return IncStrong(id); }
	inline	int32			Release(const void* id = NULL) const { return DecStrong(id); }
			
			//!	Increment the atom's weak pointer count.
			int32			IncWeak(const void* id = NULL) const;
			
			//!	Decrement the atom's weak pointer count.  If this is the
			//!	last weak pointer on it, the object will be completely deallocated.
			int32			DecWeak(const void* id = NULL) const;
			
			// Backwards compatibility.
	inline	int32		 	IncRefs(const void* id = NULL) const { return IncWeak(id); }
	inline	int32		 	DecRefs(const void* id = NULL) const { return DecWeak(id); }
			
			//!	Try to acquire a strong pointer on this atom.
			bool			AttemptIncStrong(const void* id = NULL) const;
	inline	bool			AttemptAcquire(const void* id = NULL) const { return AttemptIncStrong(id); }
	
			//!	Perform a DecStrong() if there any strong pointers remaining.
			bool			AttemptDecStrong(const void* id = NULL) const;
	inline	bool			AttemptRelease(const void* id = NULL) const { return AttemptDecStrong(id); }
			
			//!	Acquire a strong pointer on the atom, even if it doesn't have one.
			int32			ForceIncStrong(const void* id = NULL) const;
	inline	int32			ForceAcquire(const void* id = NULL) const { return ForceIncStrong(id); }
			
			//!	Try to acquire a weak pointer on this atom.
			bool			AttemptIncWeak(const void* id = NULL) const;
			
			//!	Return true if this atom has any strong acquires on it.
			/*!	\note Be careful how you use this information, as it can
				change from "true" to "false" even before you get the result.
			*/
			bool			HasStrongPointers() const;
			
			//!	Return true if this atom has more than one strong acquire on it.
			/*!	\note Be careful how you use this information.  You must already
				hold a strong pointer on the atom, and even then it can change
				from "true" to "false" before you get the result.
			*/
			bool			HasManyStrongPointers() const;
			
			//!	Return true if this atom has any weak acquires on it.
			/*!	\note Be careful how you use this information, as it can
				change from "true" to "false" even before you get the result.
			*/
			bool			HasWeakPointers() const;
			
			//!	Return true if this atom has more than one weak acquire on it.
			/*!	\note Be careful how you use this information.  You must already
				hold a weak pointer on the atom, and even then it can change
				from "true" to "false" before you get the result.
			*/
			bool			HasManyWeakPointers() const;
	
			//!	Change an owner ID associated with this atom.
			/*! This is useful, for example, if you have a piece of memory
				with a pointer to an atom, whose memory address you have used
				for the reference ID.  If you are moving your memory to a new
				location with memcpy(), you can use this function to change
				the ID stored in the atom for your reference.
				
				This function does nothing if atom debugging is not currently
				enabled.
			*/
			void			RenameOwnerID(const void* newID, const void* oldID) const;
			
			//!	Perform a memcpy() of an array of BAtom*, updating owner IDs
			//! as needed.
	static	void			MovePointersBefore(BAtom** newPtr, BAtom** oldPtr, size_t num = 1);
			
			//!	Perform a memmove() of an array of BAtom*, updating owner IDs
			//! as needed.
	static	void			MovePointersAfter(BAtom** newPtr, BAtom** oldPtr, size_t num = 1);
	
			//	************************ DEBUGGING SUPPORT ***********************
			//	******* DO NOT CALL THE REMAINING FUNCTIONS IN NORMAL CODE *******
			
			//@{
			//!	Return the number of strong/weak pointers currently
			//!	on the atom.
			int32			StrongCount() const;
			int32			WeakCount() const;
			//@}
			
			//	The remaining functions are only available when BAtom is built for
			//	debugging.
			
			//!	Print information state and references on this atom to \a io.
			void			Report(const atom_ptr<ITextOutput>& io, uint32 flags=0) const;
			
			//	BAtom leak tracking.
			
			//!	Start a new BAtom leak context and returns its identifier.
	static	int32			MarkLeakReport();
	
			//@{
			//!	Print information about atoms in a leak context.
	static	void			LeakReport(	const atom_ptr<ITextOutput>& io, int32 mark=0, int32 last=-1,
										uint32 flags=0);
	static	void			LeakReport(	int32 mark=0, int32 last=-1,
										uint32 flags=0);
			//@}
			
			//!	Check for atom existance and acquire primary reference.
	static	bool			ExistsAndIncStrong(BAtom* atom);
	static	bool			ExistsAndIncWeak(BAtom* atom);
			
			//@{
			//!	Register a particular class type for watching atom operations.
	static	void			StartWatching(const std::type_info* type);
	static	void			StopWatching(const std::type_info* type);
			//@}
	
protected:
							BAtom();
	virtual					~BAtom();
	
			//!	Called the first time an atom is acquired.  (Will become AtomInit.)
	virtual	status_t		Acquired(const void* id);
	
			//!	Called the last time an atom is released.  (Will become AtomUninit.)
	virtual	status_t		Released(const void* id);
	
			//!	Called during AttemptAcquire() after an atom has been released.  (Will become IncStrongAttempted.)
	virtual	status_t		AcquireAttempted(const void* id);
	
			//!	Called after last DecRefs() when the life of an atom is extended.
	virtual	status_t		DeleteAtom(const void* id);

private:
			friend class	BAtomTracker;
			
							BAtom(const BAtom&);
			
			void			destructor_impl();
			void			delete_impl(size_t size);
			
			// ----- private debugging support -----
			
	static	void			move_pointers(BAtom** newPtr, BAtom** oldPtr, size_t num);
			
			void			init_atom();
			void			term_atom();
			
			void			lock_atom() const;
			void			unlock_atom() const;
			
			int32*			strong_addr() const;
			int32*			weak_addr() const;
			
			int32			strong_count() const;
			int32			weak_count() const;
			
			void			watch_action(const char* description) const;
			void			do_report(const atom_ptr<class ITextOutput>& io, uint32 flags) const;
			
			void			add_incstrong(const void* id, int32 cnt) const;
			void			add_decstrong(const void* id) const;
			void			add_incweak(const void* id) const;
			void			add_decweak(const void* id) const;
	
			void			add_incstrong_raw(const void* id, int32 cnt) const;
			void			add_decstrong_raw(const void* id) const;
			void			add_incweak_raw(const void* id) const;
			void			add_decweak_raw(const void* id) const;
	
			struct base_data;
			base_data*		m_base;
			
			union {
				mutable	int32					m_strongCount;
				mutable	Private::atom_debug*	m_debugPtr;
			};
	mutable	int32			m_weakCount;
};

/**************************************************************************************/

// forward reference
template <class TYPE> class atom_ref;
template <class TYPE> class safe_ptr;
template <class TYPE> class safe_ref;

//!	This is a smart-pointer template for BAtom-derived classes that
//!	maintains a primary reference on the object.
template <class TYPE>
class atom_ptr {
public:
		//!	Initialize to NULL pointer.
		atom_ptr();
		//!	Initialize from another strong pointer.
		atom_ptr(const atom_ptr<TYPE>& p);
		//! Initialize from a raw pointer.
		atom_ptr(TYPE* p);
		//!	Release strong reference on object.
		~atom_ptr();

		//!	Static conversion to a strong pointer to another type of BAtom subclass.
		template <class NEWTYPE> operator atom_ptr<NEWTYPE>() const;
		//!	Static conversion to a weak pointer on another type of BAtom subclass.
		template <class NEWTYPE> operator atom_ref<NEWTYPE>() const;
		
		//!	Dereference pointer.
		TYPE& operator *() const;
		//!	Member dereference.
		TYPE* operator ->() const;
		
		//!	Return the raw pointer of this object, keeping the object and
		//!	its reference count as-is.
		TYPE* ptr() const;
		//!	Clear and return the raw pointer of this object.  You now own
		//!	its strong reference and must manually call DecStrong().
		TYPE* detach() const;
		
		//!	Assignment from a raw pointer.
		const atom_ptr<TYPE> &operator =(TYPE* p);
		//!	Assignment from another strong pointer.
		const atom_ptr<TYPE> &operator =(const atom_ptr<TYPE>& p);

		// Give comparison operators access to our pointer.
		#define COMPARE_FRIEND(op)												\
			bool operator op (const TYPE* p2) const								\
				{ return m_ptr op p2; }											\
			template<class T> friend bool operator op (	const atom_ptr<T>& p,	\
														const atom_ptr<T>& p);	\
			template<class T> friend bool operator op (	const atom_ref<T>& p,	\
														const atom_ptr<T>& p);	\
			template<class T> friend bool operator op (	const atom_ptr<T>& p,	\
														const atom_ref<T>& p);	\
		
		COMPARE_FRIEND(==);
		COMPARE_FRIEND(!=);
		COMPARE_FRIEND(<=);
		COMPARE_FRIEND(<);
		COMPARE_FRIEND(>);
		COMPARE_FRIEND(>=);
		
		#undef COMPARE_FRIEND

private:
		friend class atom_ref<TYPE>;
		friend class safe_ptr<TYPE>;
		friend class safe_ref<TYPE>;
		
		// Special constructor for more efficient atom_ref::promote().
		atom_ptr(TYPE* p, bool attempt_acquire);
		
		TYPE *m_ptr;
};

/**************************************************************************************/

//!	This is a smart-pointer template for BAtom-derived classes that
//!	maintains a secondary reference on the object.
template <class TYPE>
class atom_ref {
public:
		//!	Initialize to NULL pointer.
		atom_ref();
		//!	Initialize from a strong pointer.
		atom_ref(const atom_ptr<TYPE>& p);
		//!	Initialize from another weak pointer.
		atom_ref(const atom_ref<TYPE>& p);
		//! Initialize from a raw pointer.
		atom_ref(TYPE* p);
		//!	Release weak reference on object.
		~atom_ref();
		
		//!	Conversion to other atom types.
		template <class NEWTYPE> operator atom_ref<NEWTYPE>() const;
		
		//!	Retrieve raw pointer.  This is ONLY safe if you know that
		//!	the atom is designed to remain valid after all strong
		//! pointers are removed, or will only access the safe parts of BAtom.
		TYPE* unsafe_ptr_access() const;
		
		//!	Assignment from a raw pointer.
		const atom_ref<TYPE> &operator =(TYPE* p);
		//!	Assignment from a strong pointer.
		const atom_ref<TYPE> &operator =(const atom_ptr<TYPE>& p);
		//! Assigment from another weak pointer.
		const atom_ref<TYPE> &operator =(const atom_ref<TYPE>& p);

		//!	Attempt to promote this secondary reference to a primary reference.
		//!	The returned atom_ptr<> will be NULL if it failed.
		const atom_ptr<TYPE> promote() const;
		
		// Give comparison operators access to our pointer.
		#define COMPARE_FRIEND(op)												\
			bool operator op (const TYPE* p2) const								\
				{ return m_ptr op p2; }											\
			template<class T> friend bool operator op (	const atom_ref<T>& p,	\
														const atom_ref<T>& p);	\
			template<class T> friend bool operator op (	const atom_ref<T>& p,	\
														const atom_ptr<T>& p);	\
			template<class T> friend bool operator op (	const atom_ptr<T>& p,	\
														const atom_ref<T>& p);	\
		
		COMPARE_FRIEND(==);
		COMPARE_FRIEND(!=);
		COMPARE_FRIEND(<=);
		COMPARE_FRIEND(<);
		COMPARE_FRIEND(>);
		COMPARE_FRIEND(>=);
		
		#undef COMPARE_FRIEND
		
		//! Explicitly increment reference count of this atom.
		//! Should very rarely be used.
		int32 inc_weak(const void* id) const;
		//! Explicitly decrement reference count of this atom.
		//!	Should very rarely be used.
		int32 dec_weak(const void* id) const;
		
private:
		friend class atom_ptr<TYPE>;
		friend class safe_ptr<TYPE>;
		friend class safe_ref<TYPE>;
		
		TYPE *m_ptr;
};

/**************************************************************************************/

// A zillion kinds of comparison operators.
#define COMPARE(op)														\
template<class TYPE> inline												\
bool operator op(const atom_ptr<TYPE>& p1, const atom_ptr<TYPE>& p2)	\
	{ return p1.m_ptr op p2.m_ptr; }									\
template<class TYPE> inline												\
bool operator op(const atom_ref<TYPE>& p1, const atom_ref<TYPE>& p2)	\
	{ return p1.m_ptr op p2.m_ptr; }									\
template<class TYPE> inline												\
bool operator op(const atom_ptr<TYPE>& p1, const atom_ref<TYPE>& p2)	\
	{ return p1.m_ptr op p2.m_ptr; }									\
template<class TYPE> inline												\
bool operator op(const atom_ref<TYPE>& p1, const atom_ptr<TYPE>& p2)	\
	{ return p1.m_ptr op p2.m_ptr; }									\

COMPARE(==);
COMPARE(!=);
COMPARE(<=);
COMPARE(<);
COMPARE(>);
COMPARE(>=);

#undef COMPARE

/*-------------------------------------------------------------*/
/*---- No user serviceable parts after this -------------------*/

/* ----------------- atom_ptr Implementation ------------------*/

template<class TYPE> inline
atom_ptr<TYPE>::atom_ptr()								: m_ptr(NULL)	{ }
template<class TYPE> inline
atom_ptr<TYPE>::atom_ptr(const atom_ptr<TYPE>& p)		: m_ptr(p.m_ptr){ if (m_ptr) m_ptr->IncStrong(this); }
template<class TYPE> inline
atom_ptr<TYPE>::atom_ptr(TYPE* p)						: m_ptr(p)		{ if (p) p->IncStrong(this); }
template<class TYPE> inline
atom_ptr<TYPE>::~atom_ptr()								{ if (m_ptr) m_ptr->DecStrong(this); }

template<class TYPE> inline
atom_ptr<TYPE>::atom_ptr(TYPE* p, bool)					: m_ptr((p && p->AttemptIncStrong(this)) ? p : NULL) { }

template<class TYPE> template<class NEWTYPE> inline
atom_ptr<TYPE>::operator atom_ptr<NEWTYPE>() const		{ return atom_ptr<NEWTYPE>(m_ptr); }
template<class TYPE> template<class NEWTYPE> inline
atom_ptr<TYPE>::operator atom_ref<NEWTYPE>() const		{ return atom_ref<NEWTYPE>(m_ptr); }
template<class TYPE> inline
TYPE & atom_ptr<TYPE>::operator *() const				{ return *m_ptr; }
template<class TYPE> inline
TYPE * atom_ptr<TYPE>::operator ->() const				{ return m_ptr; }
template<class TYPE> inline
TYPE * atom_ptr<TYPE>::ptr() const						{ return m_ptr; }
template<class TYPE> inline
TYPE * atom_ptr<TYPE>::detach() const					{ return m_ptr; m_ptr = NULL; }

template<class TYPE> inline
const atom_ptr<TYPE> & atom_ptr<TYPE>::operator =(TYPE* p)
{
	if (p) p->IncStrong(this);
	if (m_ptr) m_ptr->DecStrong(this);
	m_ptr = p;
	return *this;
}

template<class TYPE> inline
const atom_ptr<TYPE> & atom_ptr<TYPE>::operator =(const atom_ptr<TYPE>& p)
{
	return ((*this) = p.m_ptr);
}

/* ----------------- atom_ref Implementation ------------------*/

template<class TYPE> inline
atom_ref<TYPE>::atom_ref()								: m_ptr(NULL)	{ }
template<class TYPE> inline
atom_ref<TYPE>::atom_ref(const atom_ptr<TYPE>& p)		: m_ptr(p.m_ptr){ if (m_ptr) m_ptr->IncWeak(this); }
template<class TYPE> inline
atom_ref<TYPE>::atom_ref(const atom_ref<TYPE>& p)		: m_ptr(p.m_ptr){ if (m_ptr) m_ptr->IncWeak(this); }
template<class TYPE> inline
atom_ref<TYPE>::atom_ref(TYPE* p)						: m_ptr(p)		{ if (p) p->IncWeak(this); }
template<class TYPE> inline
atom_ref<TYPE>::~atom_ref()								{ if (m_ptr) m_ptr->DecWeak(this); }

template<class TYPE> template<class NEWTYPE> inline
atom_ref<TYPE>::operator atom_ref<NEWTYPE>() const		{ return atom_ref<NEWTYPE>(m_ptr); }
template<class TYPE> inline
TYPE * atom_ref<TYPE>::unsafe_ptr_access() const		{ return m_ptr; }

template<class TYPE> inline
const atom_ref<TYPE> & atom_ref<TYPE>::operator =(TYPE *p)
{
	if (p) p->IncWeak(this);
	if (m_ptr) m_ptr->DecWeak(this);
	m_ptr = p;
	return *this;
}

template<class TYPE> inline
const atom_ref<TYPE> & atom_ref<TYPE>::operator =(const atom_ptr<TYPE> &p)
{
	return ((*this) = p.m_ptr);
}

template<class TYPE> inline
const atom_ref<TYPE> & atom_ref<TYPE>::operator =(const atom_ref<TYPE> &p)
{
	return ((*this) = p.m_ptr);
}

template<class TYPE> inline
const atom_ptr<TYPE> atom_ref<TYPE>::promote() const
{
	return atom_ptr<TYPE>(m_ptr, true);
}

template<class TYPE> inline
int32 atom_ref<TYPE>::inc_weak(const void* id) const
{
	return m_ptr ? m_ptr->IncWeak(id) : 0;
}

template<class TYPE> inline
int32 atom_ref<TYPE>::dec_weak(const void* id) const
{
	return m_ptr ? m_ptr->DecWeak(id) : 0;
}

} }	// namespace B::Support2

#endif /* _SUPPORT2_ATOM_H */
