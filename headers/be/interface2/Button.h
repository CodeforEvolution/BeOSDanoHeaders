/***************************************************************************
//
//	File:			interface2/Button.h
//
//	Description:	A button!
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _INTERFACE2_BUTTON_H_
#define _INTERFACE2_BUTTON_H_

#include <interface2/Widget.h>

namespace B {
namespace Interface2 {

using namespace Support2;

/**************************************************************************************/

class BButton : public BWidget
{
	public:

		B_STANDARD_ATOM_TYPEDEFS(BButton)
		
										BButton();
										BButton(const BValue &value);
										BButton(const BButton &copyFrom);

		virtual	BView *					Copy() { return new BButton(*this); }	

		virtual event_dispatch_result	MouseDown(const BMessage &msg, const BPoint &where, int32 buttons, int32 modifiers);
		virtual event_dispatch_result	MouseUp(const BMessage &msg, const BPoint &where, int32 buttons, int32 modifiers);

	protected:

		enum state {
			B_NORMAL = 0,
			B_ROLLED_OVER = 1,
			B_PRESSED = 2
		};

		virtual							~BButton();
		
				state					State();
				void					SetState(state newState);
		virtual void					Invoke();

	private:
	
				state					m_state;
};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_BUTTON_H_ */
