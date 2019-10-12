/***************************************************************************
//
//	File:			interface2/LabeledButton.h
//
//	Description:	A button!
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _INTERFACE2_LABELEDBUTTON_H_
#define _INTERFACE2_LABELEDBUTTON_H_

#include <support2/String.h>
#include <render2/Font.h>
#include <interface2/Button.h>

namespace B {
namespace Interface2 {

using namespace Support2;

/**************************************************************************************/

class BLabeledButton : public BButton
{
	public:

		B_STANDARD_ATOM_TYPEDEFS(BLabeledButton)
		
										BLabeledButton();
										BLabeledButton(const BValue &value);
										BLabeledButton(const BString &label, const BFont &font);
										BLabeledButton(const BLabeledButton &copyFrom);

		virtual	BView *					Copy() { return new BLabeledButton(*this); }	

		virtual	BLayoutConstraints		Constraints() const;
		virtual	BRegion					Shape() const;
		virtual void 					Draw(const IRender::ptr& into);

	protected:

		virtual							~BLabeledButton();

		virtual	void					SetFont(const BFont &font);
		virtual	void					SetLabel(const BString &label);
};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_BUTTON_H_ */
