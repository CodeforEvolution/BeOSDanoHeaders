/***************************************************************************
//
//	File:			support2/Root.h
//
//	Description:	Top level interface to a component.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _SUPPORT2_ROOT_H_
#define _SUPPORT2_ROOT_H_

#include <support2/Atom.h>
#include <support2/String.h>
#include <support2/IBinder.h>
#include <support2/IContext.h>

namespace B {
namespace Support2 {

/*--------------------------------------------------------*/
/*----- BRoot --------------------------------------------*/

class BRoot : virtual public BAtom
{
public:
			B_STANDARD_ATOM_TYPEDEFS(BRoot);
	
							BRoot();
	virtual					~BRoot();
	
	virtual IBinder::sptr	InstantiateComponent(const BString& name,
												 const IContext::sptr& context) = 0;

private:
							BRoot(const BRoot&);
};

} } // namespace B::Support2

/*! Implement this function to return a subclass of BRoot from your add-on
*/
extern "C" B::Support2::BRoot::ptr root(void);

#endif	/* _SUPPORT2_ROOT_H_ */
