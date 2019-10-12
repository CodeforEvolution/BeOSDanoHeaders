/***************************************************************************
//
//	File:			interface2/RadioButton.h
//
//	Description:	A radio button!
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _INTERFACE2_RADIO_BUTTON_H_
#define _INTERFACE2_RADIO_BUTTON_H_

#include <interface2/Control.h>

namespace B {
namespace Interface2 {

using namespace B::Support2;

/**************************************************************************************/

class BRadioButton : public BControl
{
	public:

		B_STANDARD_ATOM_TYPEDEFS(BRadioButton)
		
										BRadioButton();
										BRadioButton(const BValue &value);
										BRadioButton(const BRadioButton &copyFrom);

		virtual	BView *					Copy() { return new BRadioButton(*this); }	
		virtual	BLayoutConstraints		Constraints() const;

		virtual	BRegion					Shape() const;

		virtual	void					Invoked(const BValue &newValue);
		virtual	void					ValueChanged(const BValue &newValue);

		virtual	void					Draw(const IRender::ptr &into);
		virtual event_dispatch_result	MouseDown(const BMessage &msg, const BPoint &where, int32 buttons, int32 modifiers);
		virtual event_dispatch_result	MouseUp(const BMessage &msg, const BPoint &where, int32 buttons, int32 modifiers);

	protected:

		virtual							~BRadioButton();
		
	private:
	
				bool					m_pressed:1;
};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_RADIO_BUTTON_H_ */
