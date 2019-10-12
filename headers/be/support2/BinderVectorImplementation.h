
/*	This file implements an IBinderVector interface specialization. */

#include <support2/Binder.h>
#include <support2_p/BinderKeys.h>

#define COMBINE_TOKENS(a,b) a##b

using namespace B::Private; // For IBinderVector<> binder keys.

/**************************************************************************************/

#if IMPLEMENT_INTERFACE

const BValue IBinderVector<CHILD_TYPE>::descriptor(BValue::TypeInfo(THIS_TYPE));

#endif /* IMPLEMENT_INTERFACE */

/**************************************************************************************/

#if IMPLEMENT_REMOTE

template<class CHILD_TYPE>
status_t RBinderVector<CHILD_TYPE>::AddChild(const atom_ptr<CHILD_TYPE> &child, const BValue &attr)
{
	return
		(Remote()->Invoke(
			BValue()
				.JoinItem(BValue::Int32(0), child->AsBinder())
				.JoinItem(BValue::Int32(1), attr),
			g_keyAddChild))
		.AsStatus();
}

template<class CHILD_TYPE>
status_t RBinderVector<CHILD_TYPE>::ReplaceAttributes(int32 index, const BValue &attr)
{
	return
		(Remote()->Invoke(
			BValue()
				.JoinItem(BValue::Int32(0), BValue::Int32(index))
				.JoinItem(BValue::Int32(1), attr),
			g_keyReplaceAttributes))
		.AsStatus();
}

template<class CHILD_TYPE>
status_t RBinderVector<CHILD_TYPE>::RemoveChild(const atom_ptr<CHILD_TYPE> &child)
{
	return (Remote()->Invoke(BValue(BValue::Int32(0), child->AsBinder()), g_keyRemoveChild)).AsStatus();
}

template<class CHILD_TYPE>
atom_ptr<CHILD_TYPE> RBinderVector<CHILD_TYPE>::ChildAt(int32 index, BValue *out_attr) const
{
	BValue result = Remote()->Invoke(BValue(BValue::Int32(0), BValue::Int32(index)), g_keyChildAt);

	BValue child_value = result[g_keyChildAt_child];
	if (!(child_value.IsDefined())) return NULL;
	atom_ptr<CHILD_TYPE> child = CHILD_TYPE::AsInterface(child_value);
	if (child == NULL) return NULL;

	if (out_attr) *out_attr = result[g_keyChildAt_attr];
	return child;
}

template<class CHILD_TYPE>
atom_ptr<CHILD_TYPE> RBinderVector<CHILD_TYPE>::ChildFor(const char *name, BValue *out_attr) const
{
	BValue result = Remote()->Invoke(BValue(BValue::Int32(0), BValue::String(name)), g_keyChildFor);

	BValue child_value = result[g_keyChildAt_child];
	if (!(child_value.IsDefined())) return NULL;
	atom_ptr<CHILD_TYPE> child = CHILD_TYPE::AsInterface(child_value);
	if (child == NULL) return NULL;

	if (out_attr) *out_attr = result[g_keyChildAt_attr];
	return child;
}

template<class CHILD_TYPE>
int32 RBinderVector<CHILD_TYPE>::IndexOf(const atom_ptr<CHILD_TYPE> &child) const
{
	return (Remote()->Invoke(BValue(BValue::Int32(0), child->AsBinder()), g_keyIndexOf)).AsInteger();
}

template<class CHILD_TYPE>
BString RBinderVector<CHILD_TYPE>::NameOf(const atom_ptr<CHILD_TYPE> &child) const
{
	return (Remote()->Invoke(BValue(BValue::Int32(0), child->AsBinder()), g_keyNameOf)).AsString();
}

template<class CHILD_TYPE>
int32 RBinderVector<CHILD_TYPE>::Count() const
{
	return (Remote()->Get(BValue(g_keyCount))).AsInteger();
}

#endif /* IMPLEMENT_REMOTE */

/**************************************************************************************/

#if IMPLEMENT_CONCRETE

template<class CHILD_TYPE>
static BValue
binder_vector_hook_AddChild(const IInterface::ptr& This, const BValue& args)
{
	status_t error;
	
	BValue param = args[BValue::Int32(0)];
	if (!(param.IsDefined())) return BValue::Int32(B_BINDER_MISSING_ARG);
	IBinder::ptr child_binder = param.AsBinder(&error);
	if (error != B_OK) return BValue::Int32(B_BINDER_BAD_TYPE);
	atom_ptr<CHILD_TYPE> child = CHILD_TYPE::AsInterface(child_binder);
	if (child == NULL) return BValue::Int32(B_BINDER_BAD_TYPE);
	
	BValue attr = args[BValue::Int32(1)];
	if (!(attr.IsDefined())) return BValue::Int32(B_BINDER_MISSING_ARG);

	return BValue::Int32(static_cast<IBinderVector<CHILD_TYPE>*>(This.ptr())->AddChild(child, attr));
}

template<class CHILD_TYPE>
static BValue
binder_vector_hook_ReplaceAttributes(const IInterface::ptr& This, const BValue& args)
{
	status_t error;
	
	BValue param = args[BValue::Int32(0)];
	if (!(param.IsDefined())) return BValue::Int32(B_BINDER_MISSING_ARG);
	int32 index = param.AsInt32(&error);
	if (error != B_OK) return BValue::Int32(B_BINDER_BAD_TYPE);
	
	BValue attr = args[BValue::Int32(1)];
	if (!(attr.IsDefined())) return BValue::Int32(B_BINDER_MISSING_ARG);

	return BValue::Int32(static_cast<IBinderVector<CHILD_TYPE>*>(This.ptr())->ReplaceAttributes(index, attr));
}

template<class CHILD_TYPE>
static BValue
binder_vector_hook_RemoveChild(const IInterface::ptr& This, const BValue& args)
{
	status_t error;
	
	BValue param = args[BValue::Int32(0)];
	if (!(param.IsDefined())) return BValue::Int32(B_BINDER_MISSING_ARG);
	IBinder::ptr child_binder = param.AsBinder(&error);
	if (error != B_OK) return BValue::Int32(B_BINDER_BAD_TYPE);
	atom_ptr<CHILD_TYPE> child = CHILD_TYPE::AsInterface(child_binder);
	if (child == NULL) return BValue::Int32(B_BINDER_BAD_TYPE);
	
	return BValue::Int32(static_cast<IBinderVector<CHILD_TYPE>*>(This.ptr())->RemoveChild(child));
}

template<class CHILD_TYPE>
static BValue
binder_vector_hook_ChildAt(const IInterface::ptr& This, const BValue& args)
{
	status_t error;
	
	BValue param = args[BValue::Int32(0)];
	if (!(param.IsDefined())) return BValue::Int32(B_BINDER_MISSING_ARG);
	int i = param.AsInt32(&error);
	if (error != B_OK) return BValue::Int32(B_BINDER_BAD_TYPE);
	
	BValue attr;
	BValue child = BValue((static_cast<IBinderVector<CHILD_TYPE>*>(This.ptr())->ChildAt(i, &attr))->AsBinder());

	return
		BValue()
			.JoinItem(g_keyChildAt_child, child)
			.JoinItem(g_keyChildAt_attr, attr);
}

template<class CHILD_TYPE>
static BValue
binder_vector_hook_ChildFor(const IInterface::ptr& This, const BValue& args)
{
	status_t error;
	
	BValue param = args[BValue::Int32(0)];
	if (!(param.IsDefined())) return BValue::Int32(B_BINDER_MISSING_ARG);
	BString name = param.AsString(&error);
	if (error != B_OK) return BValue::Int32(B_BINDER_BAD_TYPE);

	BValue attr;
	BValue child = BValue((static_cast<IBinderVector<CHILD_TYPE>*>(This.ptr())->ChildFor(name, &attr))->AsBinder());

	return
		BValue()
			.JoinItem(g_keyChildAt_child, child)
			.JoinItem(g_keyChildAt_attr, attr);
}

template<class CHILD_TYPE>
static BValue
binder_vector_hook_IndexOf(const IInterface::ptr& This, const BValue& args)
{
	status_t error;
	
	BValue param = args[BValue::Int32(0)];
	if (!(param.IsDefined())) return BValue::Int32(B_BINDER_MISSING_ARG);
	IBinder::ptr child_binder = param.AsBinder(&error);
	if (error != B_OK) return BValue::Int32(B_BINDER_BAD_TYPE);
	atom_ptr<CHILD_TYPE> child = CHILD_TYPE::AsInterface(child_binder);
	if (child == NULL) return BValue::Int32(B_BINDER_BAD_TYPE);
	
	return BValue::Int32(static_cast<IBinderVector<CHILD_TYPE>*>(This.ptr())->IndexOf(child));
}

template<class CHILD_TYPE>
static BValue
binder_vector_hook_NameOf(const IInterface::ptr& This, const BValue& args)
{
	status_t error;
	
	BValue param = args[BValue::Int32(0)];
	if (!(param.IsDefined())) return BValue::Int32(B_BINDER_MISSING_ARG);
	IBinder::ptr child_binder = param.AsBinder(&error);
	if (error != B_OK) return BValue::Int32(B_BINDER_BAD_TYPE);
	atom_ptr<CHILD_TYPE> child = CHILD_TYPE::AsInterface(child_binder);
	if (child == NULL) return BValue::Int32(B_BINDER_BAD_TYPE);
	
	return BValue::String(static_cast<IBinderVector<CHILD_TYPE>*>(This.ptr())->NameOf(child));
}

template<class CHILD_TYPE>
static BValue
binder_vector_hook_Count(const IInterface::ptr& This)
{
	return BValue::Int32(static_cast<IBinderVector<CHILD_TYPE>*>(This.ptr())->Count());
}


template<class CHILD_TYPE>
status_t
LBinderVector<CHILD_TYPE>::HandleEffect(const BValue &in,
										const BValue &inBindings,
										const BValue &outBindings,
										BValue *out)
{
	static const struct effect_action_def binder_vector_actions[] = {
		{	sizeof(effect_action_def), &g_keyAddChild, NULL, NULL, NULL, binder_vector_hook_AddChild<CHILD_TYPE> },
		{	sizeof(effect_action_def), &g_keyReplaceAttributes, NULL, NULL, NULL, binder_vector_hook_ReplaceAttributes<CHILD_TYPE> },
		{	sizeof(effect_action_def), &g_keyRemoveChild, NULL, NULL, NULL, binder_vector_hook_RemoveChild<CHILD_TYPE> },
		{	sizeof(effect_action_def), &g_keyChildAt, NULL, NULL, NULL, binder_vector_hook_ChildAt<CHILD_TYPE> },
		{	sizeof(effect_action_def), &g_keyChildFor, NULL, NULL, NULL, binder_vector_hook_ChildFor<CHILD_TYPE> },
		{	sizeof(effect_action_def), &g_keyIndexOf, NULL, NULL, NULL, binder_vector_hook_IndexOf<CHILD_TYPE> },
		{	sizeof(effect_action_def), &g_keyNameOf, NULL, NULL, NULL, binder_vector_hook_NameOf<CHILD_TYPE> },
		{	sizeof(effect_action_def), &g_keyCount, NULL, NULL, binder_vector_hook_Count<CHILD_TYPE>, NULL },
	};

	return execute_effect(	IInterface::ptr(this),
							in, inBindings, outBindings, out,
							binder_vector_actions, sizeof(binder_vector_actions)/sizeof(binder_vector_actions[0]));
}

#endif /* IMPLEMENT_CONCRETE */

/**************************************************************************************/

#if IMPLEMENT_BASE

template<class CHILD_TYPE>
status_t BBinderVector<CHILD_TYPE>::_AddChild(const atom_ptr<CHILD_TYPE> &child, const BValue &attr)
{
	m_listMap.AddItem(child.ptr(), m_list.AddItem(list_entry(attr, child)));
	return B_OK;
}

template<class CHILD_TYPE>
status_t BBinderVector<CHILD_TYPE>::_RemoveChild(const atom_ptr<CHILD_TYPE> &child)
{
	const ssize_t i = m_listMap.IndexOf(child.ptr());
	if (i >= 0) {
		m_list.RemoveItemsAt(m_listMap.ValueAt(i));
		m_listMap.RemoveItemsAt(i);
		return B_OK;
	}
	
	return ENOENT;
}

template<class CHILD_TYPE>
status_t BBinderVector<CHILD_TYPE>::_ReplaceChild(int32 i, const atom_ptr<CHILD_TYPE> &child)
{
	atom_ptr<CHILD_TYPE> old_child = _ChildAt(i, NULL);
	if (old_child == NULL)
		return B_BAD_VALUE;
	m_listMap.RemoveItemFor(old_child.ptr());
	m_list.EditItemAt(i).child = child;
	m_listMap.AddItem(child.ptr(), i);
	return B_OK;
}

template<class CHILD_TYPE>
status_t BBinderVector<CHILD_TYPE>::_ReplaceAttributes(int32 i, const BValue &attr)
{
	if (i < 0 || i >= m_list.CountItems()) {
		return B_BAD_VALUE;
	}
	m_list.EditItemAt(i).attr = attr;
	return B_OK;
}

template<class CHILD_TYPE>
atom_ptr<CHILD_TYPE> BBinderVector<CHILD_TYPE>::_ChildAt(int32 index, BValue *out_attr) const
{
	if ((index < 0) || (index >= static_cast<int32>(m_list.CountItems())))
		return NULL;
	if (out_attr) *out_attr = m_list[index].attr;
	return m_list[index].child;
}

template<class CHILD_TYPE>
atom_ptr<CHILD_TYPE> BBinderVector<CHILD_TYPE>::_ChildFor(const char *name, BValue *out_attr) const
{
	for (size_t i=0; i<m_list.CountItems(); i++) {
		if (m_list[i].attr[B_FIELD_NAME].AsString() == name) {
			if (out_attr) *out_attr = m_list[i].attr;
			return m_list[i].child;
		}
	}

	return NULL;
}

template<class CHILD_TYPE>
int32 BBinderVector<CHILD_TYPE>::_IndexOf(const atom_ptr<CHILD_TYPE> &child) const
{
	const ssize_t index = m_listMap.IndexOf(child.ptr());
	return index >= B_OK ? index : -1;
}

template<class CHILD_TYPE>
BString BBinderVector<CHILD_TYPE>::_NameOf(const atom_ptr<CHILD_TYPE> &child) const
{
	const ssize_t index = m_listMap.IndexOf(child.ptr());
	return index >= B_OK ? m_list[index].attr[B_FIELD_NAME].AsString() : BString();
}

template<class CHILD_TYPE>
int32 BBinderVector<CHILD_TYPE>::_Count() const
{
	return m_list.CountItems();
}

/**************************************************************************************/

template<class CHILD_TYPE>
BBinderVector<CHILD_TYPE>::BBinderVector()
	:	m_listLock("BBinderVector access")
{
}

template<class CHILD_TYPE>
BBinderVector<CHILD_TYPE>::BBinderVector(const BBinderVector<CHILD_TYPE>& o)
	:	LBinderVector<CHILD_TYPE>(o),
		m_list(o.m_list), m_listMap(o.m_listMap)
{
}

template<class CHILD_TYPE>
BBinderVector<CHILD_TYPE>::~BBinderVector()
{
}

template<class CHILD_TYPE>
status_t BBinderVector<CHILD_TYPE>::Acquired(const void* id)
{
	return LBinderVector<CHILD_TYPE>::Acquired(id);
}

template<class CHILD_TYPE>
status_t BBinderVector<CHILD_TYPE>::Released(const void* id)
{
	m_list.MakeEmpty();
	m_listMap.MakeEmpty();
	return LBinderVector<CHILD_TYPE>::Released(id);
}

template<class CHILD_TYPE>
status_t BBinderVector<CHILD_TYPE>::AddChild(const atom_ptr<CHILD_TYPE> &child, const BValue &attr)
{
	BAutolock _auto(m_listLock.Lock());
	return _AddChild(child,attr);
}

template<class CHILD_TYPE>
status_t BBinderVector<CHILD_TYPE>::ReplaceAttributes(int32 index, const BValue &attr)
{
	BAutolock _auto(m_listLock.Lock());
	return _ReplaceAttributes(index, attr);
}

template<class CHILD_TYPE>
status_t BBinderVector<CHILD_TYPE>::RemoveChild(const atom_ptr<CHILD_TYPE> &child)
{
	BAutolock _auto(m_listLock.Lock());
	return _RemoveChild(child);
}

template<class CHILD_TYPE>
atom_ptr<CHILD_TYPE> BBinderVector<CHILD_TYPE>::ChildAt(int32 index, BValue *out_attr) const
{
	BAutolock _auto(m_listLock.Lock());
	return _ChildAt(index, out_attr);
}

template<class CHILD_TYPE>
atom_ptr<CHILD_TYPE> BBinderVector<CHILD_TYPE>::ChildFor(const char *name, BValue *out_attr) const
{
	BAutolock _auto(m_listLock.Lock());
	return _ChildFor(name, out_attr);
}

template<class CHILD_TYPE>
int32 BBinderVector<CHILD_TYPE>::IndexOf(const atom_ptr<CHILD_TYPE> &child) const
{
	BAutolock _auto(m_listLock.Lock());
	return _IndexOf(child);
}

template<class CHILD_TYPE>
BString BBinderVector<CHILD_TYPE>::NameOf(const atom_ptr<CHILD_TYPE> &child) const
{
	BAutolock _auto(m_listLock.Lock());
	return _NameOf(child);
}

template<class CHILD_TYPE>
int32 BBinderVector<CHILD_TYPE>::Count() const
{
	return _Count();
}

template<class CHILD_TYPE>
void BBinderVector<CHILD_TYPE>::_instantiate_all_the_list()
{
	_ReplaceChild(0, _ChildAt(1, NULL));
}

static BBinderVector<CHILD_TYPE>* instantiate_template()
{
	return new BBinderVector<CHILD_TYPE>();
}

static BBinderVector<CHILD_TYPE>* instantiate_template2(BBinderVector<CHILD_TYPE>& o)
{
	o._instantiate_all_the_list();
	return new BBinderVector<CHILD_TYPE>(o);
}

/**************************************************************************************/
#endif /* IMPLEMENT_BASE */

