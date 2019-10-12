/***************************************************************************
//
//	File:			wm2/IWindowManager.h
//
//	Description:	The window manager interface 
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _WM2_IWINDOWMANAGER_H_
#define _WM2_IWINDOWMANAGER_H_

#include <interface2/IView.h>
#include <support2/Binder.h>
#include <support2/String.h>
#include <wm2/IWindowModel.h>

namespace B {
namespace Wm2 {

using namespace B::Support2;
using namespace B::Interface2;

/**************************************************************************************/

/*!
	IWindowManager interface.
*/

class IWindowManager : public IInterface
{
public:

	B_DECLARE_META_INTERFACE(WindowManager);

	/*! Interface to performs actions on a window manager.
		These are usually called from user code.
	*/
	//{@
	virtual void			AddWindow(const IView::ptr& view) = 0;
	//@}
};

/**************************************************************************************/

//! Implement the local Window binder interface.
class LWindowManager : public LInterface<IWindowManager>
{
public:
	virtual	status_t HandleEffect(const BValue &in, const BValue &inBindings, const BValue &outBindings, BValue *out);
};

/**************************************************************************************/

} } // namespace B::Wm2

#endif	/* _WM2_IWINDOWMANAGER_H_ */

