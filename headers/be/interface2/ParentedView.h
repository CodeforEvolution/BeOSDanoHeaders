/***************************************************************************
//
//	File:			interface2/ParentedView.h
//
//	Description:	BParentedView implements Parent() and SetParent().
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _INTERFACE2_PARENTEDVIEW_H_
#define _INTERFACE2_PARENTEDVIEW_H_

#include <interface2/InterfaceDefs.h>
#include <interface2/IView.h>
#include <interface2/IViewParent.h>
#include <support2/Locker.h>

namespace B {
namespace Interface2 {

/**************************************************************************************/

/*!
	BParentedView implements the Parent()/SetParent() handling that's common
	to BView and BViewLayoutRoot.  It doesn't do much, and we hope to get
	rid of it someday, but the SetParent() handling is nontrivial, so while
	it's in development, it's better to have it in one place.
	\sa BView BViewLayoutRoot
*/

class BParentedView : public LView
{
public:
									BParentedView(const BValue& attr = BValue::undefined);
									BParentedView(const BParentedView &copyFrom);
	virtual							~BParentedView();

	virtual	status_t				SetParent(IViewParent::sptr_ref parent);
	virtual	IViewParent::sptr		Parent() const;


			void					InvalidateConstraints();
	virtual void					Invalidate(const BUpdate& update);
			void					Invalidate();

protected:
	virtual	status_t				Acquired(const void* id);
	virtual	status_t				Released(const void* id);
	
				lock_status_t		LockParent() const;
				void				UnlockParent() const;
	virtual	atom_ptr<IViewParent>	_Parent() const;

private:
	mutable	BLocker					m_parentLock;
			atom_ref<IViewParent>	m_parent;
};

} } // namespace B::Interface2

#endif	/* _INTERFACE2_PARENTEDVIEW_H_ */
