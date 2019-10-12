/***************************************************************************
//
//	File:			interface2/Control.h
//
//	Description:	A widget that allows viewing and modification of a datum
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _INTERFACE2_CONTROL_H_
#define _INTERFACE2_CONTROL_H_

#include <interface2/Widget.h>

namespace B {
namespace Interface2 {

using namespace Support2;

/**************************************************************************************/

/*
	A control is a type of view that takes an input value of arbitrary complexity and
	displays it to the user for review or modification.  It has one
	input binding and one output binding.  The input is the value that is actually
	displayed, and the output is the result of any changes made by the user.
*/

/**************************************************************************************/

class BControl : public BWidget
{
	public:

		B_STANDARD_ATOM_TYPEDEFS(BControl)
		
								BControl();
								BControl(const BValue &value);
								BControl(const BControl &copyFrom);

		virtual	BView *			Copy() { return new BControl(*this); }	

		virtual	BValue			Value() const;
			
	protected:

		virtual					~BControl();

		virtual	void			Invoked(const BValue &newValue);
		virtual	void			ValueChanged(const BValue &newValue);

		virtual	status_t		Told(BValue &in);
		virtual	status_t		Asked(const BValue &outBindings, BValue &out);
	
	private:

				BValue			m_value;
};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_CONTROL_H_ */
