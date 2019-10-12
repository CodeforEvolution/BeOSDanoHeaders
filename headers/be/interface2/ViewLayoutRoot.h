/***************************************************************************
//
//	File:			interface2/ViewLayoutRoot.h
//
//	Description:	BViewLayoutRoot handles asynchronous layouts.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _INTERFACE2_VIEWLAYOUTROOT_H_
#define _INTERFACE2_VIEWLAYOUTROOT_H_

#include <support2/Handler.h>
#include <interface2/InterfaceDefs.h>
#include <interface2/IViewParent.h>
#include <interface2/ParentedView.h>

namespace B {
namespace Interface2 {

/**************************************************************************************/

/*!
	BViewLayoutRoot often sits at the top of a view hierarchy
	and is mostly used to do asynchronous layouts across
	teams. Users normaly don't instantiate or subclass
	BViewLayoutRoot.
	\sa IView IViewParent BParentedView BViewLayout
*/

class BViewLayoutRoot : public BParentedView, public LViewParent, public BHandler
{
public:
							BViewLayoutRoot(const IView::ptr &child, const BValue& attr = BValue::undefined);
	virtual					~BViewLayoutRoot();

	/*!	Most of the IView interface is delegated directly to \c BViewLayoutRoot's
		single child.
	*/
	virtual	bool					SetTransform(const B2dTransform& transform);
	virtual bool					SetFrame(const BRect& bounds);
	virtual	bool					SetExternalConstraints(const BLayoutConstraints &c);
	virtual	void					Hide();
	virtual	void					Show();
	virtual	BLayoutConstraints		Constraints() const;
	virtual	BRegion					Shape() const;
	virtual	B2dTransform			Transform() const;
	virtual void					DispatchEvent(	const BMessage &msg,
													const BPoint& where,
													event_dispatch_result *result);
	virtual	bool					IsHidden() const;
	virtual	void					Display(const IRender::ptr& into);
	virtual void					Draw(const IRender::ptr& into);
	virtual void					Invalidate(const BUpdate& update);
	virtual BRect					Frame() const;

	/*! \c PreTraversal() and \c LayoutTransactionView() do nothing.
		\c BViewLayoutRoot pretraverses, lays out, and posttraverses its single
		child in its own thread when the child requests those actions (using
		the \c IViewParent interface that \c BViewLayoutRoot presents to its
		child). 
	*/
	virtual	void			PreTraversal();

	//! \c PostTraversal() always returns \c this.  BViewLayoutRoot never copies itself.
	virtual	IView::ptr		PostTraversal(BUpdate &outDirty);

	/*! \c MarkTraversalPath() sets atomic traversal flags and, if necessary,
		posts a message to itself to prompt a pretraverse/layout/posttraverse
		sequence on the child.
	*/
	virtual	void			MarkTraversalPath(int32 type);

	/*! \c ConstrainChild() redirects the call to the \c BViewLayoutRoot's
		parent, changing \a child to \c this.
	*/
	virtual	status_t		InvalidateChildConstraints(	const atom_ptr<IView> &child);

	/*! \c InvalidateChild() propagates the \c InvalidateChild() call to
		the \c BViewLayoutRoot's parent, if any. It will eventually trigger a
		call to \c IView::Display() when it reaches the root surface.
	*/
	virtual	status_t		InvalidateChild(const atom_ptr<IView> &child,
											const BUpdate &update);

	/*! \c HandleMessage() reads the traversal flags and, depending on which
		ones were set, calls \c PreTraversal(), \c LayoutTransactionView(),
		and \c PostTraversal() on its child. It will not process a \c bmsgGo
		message if \c m_blockPost is \c true (meaning we're in a
		\c Begin/EndModifyChild()). In that case it sets \c m_deferredPost and
		\c EndModifyChild() posts another bmsgGo message after we're done
		modifying the child.
	*/ 
	virtual	status_t		HandleMessage(const BMessage &message);

protected:

	virtual	status_t		Acquired(const void* id);
	virtual	status_t		Released(const void* id);
	/*! Using \c Child() prevents you from getting a corrupt child \c ptr,
		but you might get a stale one (i.e., one that is about to be discarded
		after its \c PostTraversal()). This is acceptable for \c Draw(), but not
		much else. If you want to be sure that your work with the child is
		synchronous with respect to the child's \c PostTraversal(), lock
		\c m_childLock while you work, and use \c m_child directly.
	*/
			IView::ptr		Child() const;

private:

			int32			SetTraversalFlags(int32 which_bits);
			int32			GetTraversalFlags(int32 which_bits);

			int32				m_traversalFlags;
			IView::ptr			m_child;
			BLayoutConstraints	m_constraints;
	mutable	BLocker				m_childLock;
			BRect				m_frame;
};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_VIEWLAYOUTROOT_H_ */
