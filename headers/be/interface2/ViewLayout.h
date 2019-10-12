/***************************************************************************
//
//	File:			interface2/ViewLayout.h
//
//	Description:	general-purpose view-layout engine
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _INTERFACE2_VIEWLAYOUT_H_
#define _INTERFACE2_VIEWLAYOUT_H_

#include <interface2/InterfaceDefs.h>
#include <interface2/ViewList.h>
#include <interface2/ViewParent.h>
#include <interface2/View.h>
#include <support2/Locker.h>

namespace B {
namespace Render2 { class BUpdate; }
namespace Interface2 {

/**************************************************************************************/

/*!
	BViewLayout is the base class (pure virtual) used to
	implement a layout engine. Two pure virtuals must be
	implemented:
		\li void Layout() = 0;
		\li BLayoutConstraints InternalConstraints() const = 0;
	\sa BView LViewParent BViewParent BViewList
*/


class BViewLayout : public BViewParent, public BViewList
{
	public:
										BViewLayout(const BValue &attr = BValue::undefined);
										BViewLayout(const BViewLayout &copyFrom);
		virtual							~BViewLayout();

		/*! Implement Layout() to apply the new layout to the hierarchy
			typically Layout() resolves the constraints and calls
			IView::SetLayoutBounds on all its children.
			\sa IView::SetLayoutBounds
		*/
		virtual void					Layout() = 0;

		/*! Implement InternalConstraints() to return the constraints based on the
			children's external constraints. Usually (but not always) the internal
			constraints overrule the children's external constraints.
		*/
		virtual	BLayoutConstraints		InternalConstraints() const = 0;

		//! Use this method to add a view to the hierarchy
		virtual	status_t				AddView(const IView::ptr &child, const BValue &attr = BValue::undefined);

		//! Use this method to remove a view from the hierarchy
		virtual	status_t				RemoveView(const IView::ptr &child);


		//! These methods are described in IView. \sa IView.
		virtual	void					Draw(const IRender::ptr &into);

		virtual bool					SetFrame(const BRect& bounds);
		virtual	bool					SetExternalConstraints(const BLayoutConstraints &c);

		virtual	void					PreTraversal();
		virtual	IView::ptr				PostTraversal(BUpdate &outDirty);

		virtual	BRegion					Shape() const;
		virtual	BLayoutConstraints		Constraints() const;

		virtual	void					DispatchEvent(	const BMessage &msg,
														const BPoint& where,
														event_dispatch_result *result);

	protected:
	
		virtual	status_t				Acquired(const void* id);
		virtual	status_t				Released(const void* id);
				void					CalculateConstraints();

	private:
	
				BLayoutConstraints		m_aggregateConstraints;
				BRegion					m_shape;
};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_VIEWLAYOUT_H_ */
