/***************************************************************************
//
//	File:			wm2/IWindow.h
//
//	Description:	The window interface
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _WM2_IWINDOW_H_
#define _WM2_IWINDOW_H_

#include <support2/Binder.h>
#include <wm2/IWindowModel.h>

namespace B {
namespace Wm2 {

using namespace B::Support2;
using namespace B::Interface2;

/**************************************************************************************/

/*!
	IWindow interface. This is client side of a Window.
	This interface allows you to receive events about a
	window (eg: Close requested, etc...).
	\sa BWindow
*/

class IWindow : public IInterface
{
public:

	B_DECLARE_META_INTERFACE(Window);
	
	/*! Requests from users to an IWindow
		These are typically called by the Window Manager
	*/
	//{@
	//! the window is asked to close itself (eg: close button pressed)
	virtual	void				CloseRequested() = 0;

	//! the window is asked to minimize itself (eg: minimize button has been pressed)
	virtual	void				MinimizeRequested() = 0;
	
	//! the window is asked to maximize itself (unminimize)
	virtual	void				MaximizeRequested() = 0;
	
	//! the window is asked to get full screen
	virtual	void				FullRequested() = 0;
	
	//! the window is asked to jump to the front
	virtual	void				FrontRequested() = 0;

	//! the window is asked to get the focus
	virtual	void				FocusRequested() = 0;

	//! the window is asked to be moved or resized
	virtual	void				MoveSizeRequested(uint32 anchors) = 0;
	//@}

	//! called by the window manager to give access to the window model
	virtual status_t			SetWindowModel(const IWindowModel::ref&) = 0;

	/*! Used to retrieve the window's model
		This is needed to perform actual actions
		on a window. Usually called by the IWindow
		implementation, but not necessarily.
	*/
	virtual IWindowModel::ptr	WindowModel() const = 0;
};


/**************************************************************************************/

//! Implement the local Window binder interface.
class LWindow : public LInterface<IWindow>
{
public:
	virtual	status_t HandleEffect(const BValue &in, const BValue &inBindings, const BValue &outBindings, BValue *out);
};

/**************************************************************************************/

} } // namespace B::Wm2

#endif	/* _WM2_IWINDOW_H_ */

