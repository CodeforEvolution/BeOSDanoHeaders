/***************************************************************************
//
//	File:			interface2/Widget.h
//
//	Description:	A view that represents a UI widget
//					This includes any form element, active or not
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _INTERFACE2_WIDGET_H_
#define _INTERFACE2_WIDGET_H_

#include <support2/String.h>
#include <render2/Font.h>
#include <interface2/View.h>

namespace B {
namespace Interface2 {

using namespace Support2;

/**************************************************************************************/

class BWidget : public BView
{
	public:

		B_STANDARD_ATOM_TYPEDEFS(BWidget)
		
								BWidget();
								BWidget(const BValue &value);
								BWidget(const BWidget &copyFrom);

				BFont			Font() const;
				BString			Label() const;
			
	protected:

		virtual	BView *			Copy() { return new BWidget(*this); }	

		virtual	void			SetFont(const BFont &font);
		virtual	void			SetLabel(const BString &label);

		virtual					~BWidget();
		virtual	status_t		Told(BValue &in);
		virtual	status_t		Asked(const BValue &outBindings, BValue &out);
	
	private:

				BFont			m_font;
				BString			m_label;
};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_CONTROL_H_ */
