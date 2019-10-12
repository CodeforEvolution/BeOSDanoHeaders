/***************************************************************************
//
//	File:			interface2/ViewGroup.h
//
//	Description:	A basic layout engine, that just honor the children's constraints
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _INTERFACE2_VIEWGROUP_H_
#define _INTERFACE2_VIEWGROUP_H_

#include <interface2/ViewLayout.h>

namespace B {
namespace Render2 { class BUpdate; }
namespace Interface2 {

/**************************************************************************************/

/*!
	Basic implementation of BViewLayout. This layout engine always let
	its children layout themselves as they requested.
	\sa BViewLayout
*/

class BViewGroup : public BViewLayout
{
public:

									BViewGroup(const BValue &attr = BValue::undefined);
									BViewGroup(const BViewGroup &copyFrom);
	virtual							~BViewGroup();

	virtual	BView *					Copy();

	virtual	BLayoutConstraints		InternalConstraints() const;
	virtual	void					Layout();

private:

	mutable BLayoutConstraints		m_internal_cnst;
			uint32					m_squeeze:2;
};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_VIEWGROUP_H_ */
