/***************************************************************************
//
//	File:			interface2/ViewParent.h
//
//	Description:	A default implementation for IViewParent + BView
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _INTERFACE2_VIEWPARENT_H_
#define _INTERFACE2_VIEWPARENT_H_

#include <interface2/IViewParent.h>
#include <interface2/View.h>

namespace B {
namespace Interface2 {

/**************************************************************************************/

class BViewParent : public LViewParent, public BView
{
public:
						 BViewParent(const BValue& attr = BValue::undefined);
						 BViewParent(const BViewParent &copyFrom);
	virtual 			~BViewParent();

	/*! Call \c MarkTraversalPath() with \e constrain and \e layout flags.
		This will eventually trigger an update as MarkTraversalPath() walks
		up in the hierarchy and encounters a \c ViewLayoutRoot object.
	*/
	virtual	status_t	InvalidateChildConstraints(const atom_ptr<IView> &child);

	/*! \c BViewParent propagate \c InvalidateChild() on its own parent if any.
		It will eventually trigger a call to IView::Display() as it reaches the root surface.
	*/
	virtual	status_t	InvalidateChild(	const atom_ptr<IView> &child,
											const BUpdate &update);

	//! Set the parent and triggers a layout.
	virtual	status_t	SetParent(const atom_ptr<IViewParent> &parent);

	virtual	void		MarkTraversalPath(int32 type);

protected:
			int32		SetTraversalFlags(int32 which_bits);
			int32		GetTraversalFlags(int32 which_bits);

private:

			int32		m_traversalFlags;
};

} } // namespace B::Interface2

#endif	/* _INTERFACE2_VIEWPARENT_H_ */
