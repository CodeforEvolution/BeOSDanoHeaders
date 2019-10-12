
#ifndef _SUPPORT2_BINDERVECTOR_H_
#define _SUPPORT2_BINDERVECTOR_H_

#include <support2/Vector.h>
#include <support2/KeyedVector.h>
#include <support2/Binder.h>
#include <support2/Locker.h>
#include <support2/Autolock.h>
#include <support2/IInterface.h>

namespace B {
namespace Support2 {

//! When calling AddChild(), attr[B_FIELD_NAME] gives the name to be used for the child.
extern const BValue B_FIELD_NAME;

/**************************************************************************************/

template <class CHILD_TYPE>
class IBinderVector : public IInterface
{
	public:
		static	const BValue					descriptor;
		static	atom_ptr< IBinderVector<CHILD_TYPE> >	AsInterface(const IBinder::ptr &o);

		virtual	status_t						AddChild(const atom_ptr<CHILD_TYPE> &child, const BValue &attr = BValue::undefined) = 0;
		virtual	status_t						ReplaceAttributes(int32 i, const BValue &attr) = 0;
		virtual	status_t						RemoveChild(const atom_ptr<CHILD_TYPE> &child) = 0;
		virtual	atom_ptr<CHILD_TYPE>			ChildAt(int32 index, BValue *out_attr = NULL) const = 0;
		virtual	atom_ptr<CHILD_TYPE>			ChildFor(const char *name, BValue *out_attr = NULL) const = 0;
		virtual	int32							IndexOf(const atom_ptr<CHILD_TYPE> &child) const = 0;
		virtual	BString							NameOf(const atom_ptr<CHILD_TYPE> &child) const = 0;
		virtual	int32							Count() const = 0;
};

/**************************************************************************************/

template <class CHILD_TYPE>
inline atom_ptr< IBinderVector<CHILD_TYPE> > 
IBinderVector<CHILD_TYPE>::AsInterface(const IBinder::ptr &o)
{
	return dynamic_cast<IBinderVector<CHILD_TYPE>*>(o->Get(descriptor).AsAtom().ptr());
}

/**************************************************************************************/

template <class CHILD_TYPE>
class LBinderVector : public LInterface< IBinderVector<CHILD_TYPE> >
{
	protected:
		inline									LBinderVector() { }
		inline									LBinderVector(const LBinderVector& other) : LInterface< IBinderVector<CHILD_TYPE> >(other) { }
		virtual									~LBinderVector() { }
		
		virtual	status_t						HandleEffect(	const BValue &in,
																const BValue &inBindings,
																const BValue &outBindings,
																BValue *out);
};

/**************************************************************************************/

template <class CHILD_TYPE>
class RBinderVector : public RInterface< IBinderVector<CHILD_TYPE> >
{
	public:
												RBinderVector(IBinder::arg remote) : RInterface< IBinderVector<CHILD_TYPE> >(remote) {};
													
		virtual	status_t						AddChild(const atom_ptr<CHILD_TYPE> &child, const BValue &attr = BValue::undefined);
		virtual	status_t						ReplaceAttributes(int32 i, const BValue &attr);
		virtual	status_t						RemoveChild(const atom_ptr<CHILD_TYPE> &child);
		virtual	atom_ptr<CHILD_TYPE>			ChildAt(int32 index, BValue *out_attr = NULL) const;
		virtual	atom_ptr<CHILD_TYPE>			ChildFor(const char *name, BValue *out_attr = NULL) const;
		virtual	int32							IndexOf(const atom_ptr<CHILD_TYPE> &child) const;
		virtual	BString							NameOf(const atom_ptr<CHILD_TYPE> &child) const;
		virtual	int32							Count() const;
};

/**************************************************************************************/

template <class CHILD_TYPE>
class BBinderVector : public LBinderVector<CHILD_TYPE>
{
	public:
												BBinderVector();
												BBinderVector(const BBinderVector<CHILD_TYPE>& o);
												
		virtual	status_t						AddChild(const atom_ptr<CHILD_TYPE> &child, const BValue &attr = BValue::undefined);
		virtual	status_t						RemoveChild(const atom_ptr<CHILD_TYPE> &child);
		virtual	atom_ptr<CHILD_TYPE>			ChildAt(int32 index, BValue *out_attr = NULL) const;
		virtual	atom_ptr<CHILD_TYPE>			ChildFor(const char *name, BValue *out_attr = NULL) const;
		virtual	status_t						ReplaceAttributes(int32 i, const BValue &attr);
		virtual	int32							IndexOf(const atom_ptr<CHILD_TYPE> &child) const;
		virtual	BString							NameOf(const atom_ptr<CHILD_TYPE> &child) const;
		virtual	int32							Count() const;

	protected:
		virtual									~BBinderVector();
		
				status_t						_AddChild(const atom_ptr<CHILD_TYPE> &child, const BValue &attr);
				status_t						_RemoveChild(const atom_ptr<CHILD_TYPE> &child);
				status_t						_ReplaceChild(int32 i, const atom_ptr<CHILD_TYPE> &child);
				status_t						_ReplaceAttributes(int32 i, const BValue &attr);
				atom_ptr<CHILD_TYPE>			_ChildAt(int32 index, BValue *out_attr = NULL) const;
				atom_ptr<CHILD_TYPE>			_ChildFor(const char *name, BValue *out_attr = NULL) const;
				int32							_IndexOf(const atom_ptr<CHILD_TYPE> &child) const;
				BString							_NameOf(const atom_ptr<CHILD_TYPE> &child) const;
				int32							_Count() const;

		virtual	status_t						Acquired(const void* id);
		virtual	status_t						Released(const void* id);

	private:


				typedef BKeyedVector<CHILD_TYPE*,int32> index_map;

				struct list_entry {
					list_entry() {};
					list_entry(const BValue &_attr,
					           const atom_ptr<CHILD_TYPE> &_child)
						:	attr(_attr),
							child(_child)
					{};

					BValue					attr;
					atom_ptr<CHILD_TYPE>	child;
				};
	
		mutable	BLocker					m_listLock;
				BVector<list_entry>		m_list;
				index_map				m_listMap;
	
	public:
		// gross hack
				void						_instantiate_all_the_list();
};

/**************************************************************************************/

} } // namespace B::Support2

#endif	/* _SUPPORT2_BINDERVECTOR_H_ */
