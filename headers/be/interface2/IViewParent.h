/***************************************************************************
//
//	File:			interface2/IViewParent.h
//
//	Description:	An interface used by views that can have children
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _INTERFACE2_IVIEWPARENT_H_
#define _INTERFACE2_IVIEWPARENT_H_

#include <support2/Binder.h>
#include <render2/RenderDefs.h>
#include <render2/2dTransform.h>
#include <support2/IInterface.h>
#include <interface2/InterfaceDefs.h>

namespace B {
namespace Render2 {
	class BUpdate;
}}

namespace B {
namespace Interface2 {

class IView;
class BLayoutConstraints;

/**************************************************************************************/

class IViewParent : public IInterface
{
public:

	B_DECLARE_META_INTERFACE(ViewParent)

			//! Bitmask for MarkTraversalPath()
			enum {
				pretraversal = 	0x00000001,	//! Need PreTraversal()
				constrain = 	0x00000002,	//! Need the constraints to be resolved
				layout = 		0x00000004,	//! Need layout
				posttraversal =	0x00000008	//! Need PostTraversal()
			};

	/*! \c InvalidateChildConstraints() is called by the framework to notifiy the parent
		that a \param child has new external constraints.
		\note \c InvalidateChildConstraints() doesn't call \c IView::SetExternalConstraints()
		\sa IView::SetExternalConstraints
	*/
	virtual	status_t	InvalidateChildConstraints(	const atom_ptr<IView> &child) = 0;

	/*! Invalidate the specified \param child with the \param update given as argument.
		\sa \c IView::Display() \c IView::Draw()
	*/
	virtual	status_t	InvalidateChild(const atom_ptr<IView> &child,
										const BUpdate &update) = 0;

	//! Mark the views in the hierarchy that need a layout
	virtual	void		MarkTraversalPath(int32 type) = 0;
};

/**************************************************************************************/

class LViewParent : public LInterface<IViewParent>
{
protected:
						LViewParent();
						LViewParent(const LViewParent& other);
	virtual				~LViewParent();
	virtual	status_t	Told(BValue &in);
};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_IVIEWPARENT_H_ */
