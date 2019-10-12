/***************************************************************************
//
//	File:			wm2/IWindowModel.h
//
//	Description:	The window model interface
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _WM2_IWINDOWMODEL_H_
#define _WM2_IWINDOWMODEL_H_

#include <interface2/IView.h>
#include <support2/Binder.h>
#include <support2/String.h>

namespace B {
namespace Wm2 {

using namespace B::Support2;
using namespace B::Interface2;

/**************************************************************************************/

/*!
	IWindowModel interface. This is the Window Manager side
	of a window. IWindowModel allows you to
		\li set the look and feel of a window
		\li perform actions on a window (close, move, etc...)
		\li query about the state of a window
	\note You never instantiate yourself this object, it is
	given to you through the IWindow interface.
	\sa IWindow
*/

//! These enums define the window feel. \sa SetFeel()
enum window_feel {
	B_NORMAL_WINDOW_FEEL			= 0,	//! A regular window
	B_MODAL_SUBSET_WINDOW_FEEL		= 1,	// think about it...
	B_MODAL_APP_WINDOW_FEEL			= 2,	// think about it...
	B_MODAL_ALL_WINDOW_FEEL			= 3,	// think about it...
	B_FLOATING_SUBSET_WINDOW_FEEL	= 4,	// think about it...
	B_FLOATING_APP_WINDOW_FEEL		= 5,	// think about it...
	B_FLOATING_ALL_WINDOW_FEEL		= 6		// think about it...
};

//! These enums define the window look. \sa SetLook()
enum window_look {
	B_DOCUMENT_WINDOW_LOOK			= 0,	//! Window for documents
	B_TITLED_WINDOW_LOOK			= 1,	//! Window with a title bar
	B_BORDERED_WINDOW_LOOK			= 2,	//! Window with a border only
	B_NO_BORDER_WINDOW_LOOK			= 3,	//! Window with no decor at all
	B_MODAL_WINDOW_LOOK				= 4,	//! Window for immediate user action
	B_FLOATING_WINDOW_LOOK			= 5		//! A floatting window look
};

//! These flags affect the window decor. \sa SetLook()
enum {
	B_LOOK_NOT_CLOSABLE				= 0x00000001,
	B_LOOK_NOT_ZOOMABLE				= 0x00000002,
	B_LOOK_NOT_MINIMIZABLE			= 0x00000004,
};

//! Define the handles to be shown by the window decor. \sa SetLook()
enum {
	B_HANDLE_NONE					= 0x00000000,
	B_HANDLE_LEFT					= 0x00000001,
	B_HANDLE_TOP					= 0x00000002,
	B_HANDLE_RIGHT					= 0x00000004,
	B_HANDLE_BOTTOM					= 0x00000008,
	B_HANDLE_LEFT_TOP				= 0x00000010,
	B_HANDLE_RIGHT_TOP				= 0x00000020,
	B_HANDLE_RIGHT_BOTTOM			= 0x00000040,
	B_HANDLE_LEFT_BOTTOM			= 0x00000080,
	B_HANDLE_ALL					= 0x000000FF
};

//! Define the anchors for a move or resize action. \sa MoveSize()
enum {
	B_ANCHOR_LEFT					= 0x00000001,
	B_ANCHOR_TOP					= 0x00000002,
	B_ANCHOR_RIGHT					= 0x00000004,
	B_ANCHOR_BOTTOM					= 0x00000008,
	B_ANCHOR_ALL					= 0x0000000F
};

//! Workspaces
enum {
	B_CURRENT_WORKSPACE	= 0,
	B_ALL_WORKSPACES	= 0xFFFFFFFF
};

/**************************************************************************************/

/*!
	IWindowModel interface
*/

class IWindowModel : public IInterface
{
public:

	B_DECLARE_META_INTERFACE(WindowModel);

	/*! Interface to performs actions on a window.
		These are usually called from an IWindow object.
	*/
	//{@
	virtual void			Close() = 0;
	virtual void			Minimize() = 0;
	virtual void			Maximize() = 0;
	virtual void			Full() = 0;
	virtual void			SendToFront() = 0;
	virtual void			SendToBack() = 0;
	virtual void			SendBehind(const IView::ptr& window) = 0;
	virtual void			Activate(bool focus = true) = 0;
	virtual void			MoveSize(uint32 anchors) = 0;
	virtual void			SetTitle(const BString& title) = 0;
	virtual void			SetWorkspaces(uint32 ws) = 0;
	virtual void			SetFeel(window_feel feel) = 0;
	virtual void			SetLook(window_look look, int32 flags, int32 handles) = 0;
	//@}

	/*! Interface to get the state of a window.
		These are usually called from an IWindow object.
	*/
	//{@
	virtual bool			IsMinimized() const = 0;
	virtual bool			IsFull() const = 0;
	virtual bool			IsFront() const = 0;
	virtual bool			IsActive() const = 0;
	virtual BString			Title() const = 0;
	virtual uint32			Workspaces() const = 0;
	virtual window_feel		Feel() const = 0;
	virtual window_look		Look() const = 0;
	//@}
};

/**************************************************************************************/

//! Implement the local Window binder interface.
class LWindowModel : public LInterface<IWindowModel>
{
public:
	virtual	status_t HandleEffect(const BValue &in, const BValue &inBindings, const BValue &outBindings, BValue *out);
};

/**************************************************************************************/

} } // namespace B::Wm2

#endif	/* _WM2_IWINDOWMODEL_H_ */

