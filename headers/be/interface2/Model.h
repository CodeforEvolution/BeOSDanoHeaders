/***************************************************************************
//
//	File:			interface2/Model.h
//
//	Description:	Model
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _INTERFACE2_MODEL_H_
#define _INTERFACE2_MODEL_H_

#include <interface2/InterfaceDefs.h>
#include <interface2/IModel.h>
#include <interface2/View.h>
#include <support2/IContext.h>
#include <support2/Locker.h>
#include <support2/OrderedVector.h>

namespace B {
namespace Interface2 {

using namespace Support2;

/**************************************************************************************/

class BModel : public LModel
{
public:
	B_STANDARD_ATOM_TYPEDEFS(BModel)
	
							BModel(	const IContext::ptr& context,
									const BValue &attr = BValue::undefined);
#warning "REMOVE ME"
//START HERE
	virtual void			DispatchEvent(const BMessage &msg, const IView::ptr& view = NULL);
	virtual void			WheelMoved(const BMessage &msg, coord xDelta, coord yDelta);
	virtual	void			KeyDown(const BMessage &msg, const char *bytes, int32 numBytes);
	virtual	void			KeyUp(const BMessage &msg, const char *bytes, int32 numBytes);
//END HERE
		
protected:
	virtual					~BModel();

private:
#warning "REMOVE ME"
//START HERE
	void do_key_up(const BMessage &, const IView::ptr&);
	void do_key_dn(const BMessage &, const IView::ptr&);
//END HERE
};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_MODEL_H_ */
