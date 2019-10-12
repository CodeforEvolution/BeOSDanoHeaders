/***************************************************************************
//
//	File:			wm2/Window.h
//
//	Description:	The window definition
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _WM2_WINDOW_H_
#define _WM2_WINDOW_H_

#include <support2/String.h>
#include <wm2/IWindow.h>

namespace B {
namespace Wm2 {

using namespace B::Support2;
using namespace B::Interface2;

/**************************************************************************************/

/*!
	BWindow, an IWindow implementation.
*/

class BWindow : public LWindow
{
public:

	/*! These flags affect how xxxRequested() behaves
		In any case they can be overulled programatically.
	*/ 
	enum {
		B_NOT_MOVABLE				= 0x00000001,		//! This window is not movable
		B_NOT_RESIZABLE				= 0x00000002,		//! This window is not resizable
		B_NOT_H_RESIZABLE			= 0x00000004,		//! This window is not resizable horizontally
		B_NOT_V_RESIZABLE			= 0x00000008,		//! This window is not resizable vertically
		B_NOT_CLOSABLE				= 0x00000010,		//! This window is not closable
		B_NOT_ZOOMABLE				= 0x00000020,		//! This window is not zoomable
		B_NOT_MINIMIZABLE			= 0x00000040,		//! This window is not minimizable
		B_AVOID_FRONT				= 0x00010000,		//! This window cannot be the frontmost (unless it is the only one)
		B_AVOID_FOCUS				= 0x00020000,		//! This window cannot get the focus
		B_WILL_ACCEPT_FIRST_CLICK	= 0x00040000,		// not sure about these
		B_NO_WORKSPACE_ACTIVATION	= 0x00080000,		// not sure about these
		B_NOT_ANCHORED_ON_ACTIVATE	= 0x00100000,		// not sure about these
		B_FLAGS_NOT_VALID			= 0xFFFFFFFF,		//! should never happen
	};


	//! Construct your window
				BWindow(			const BString& title,
									window_look look	= B_TITLED_WINDOW_LOOK,
									window_feel feel	= B_NORMAL_WINDOW_FEEL,
									uint32 flags		= 0,
									uint32 workspace	= B_CURRENT_WORKSPACE);

				BWindow(const BWindow& copyFrom);

	virtual		~BWindow();
	status_t	Acquired(const void *id);
	status_t	Released(const void *id);

	//! Set/Get the behaviour of xxxRequested()
	void				SetFlags(uint32 flags);
	uint32				Flags() const;


	/*! Events from the window manager or users of
		that window.
		Override to change the default behaviour.
	*/ 
	//{@
	virtual	void	CloseRequested();					//! Closes the window according to Flags()
	virtual	void	MinimizeRequested();				//! Minimizes the window according to Flags()
	virtual	void	MaximizeRequested();				//! Always Maximize the window
	virtual	void	FullRequested();					//! Zooms the window according to Flags()
	virtual	void	FrontRequested();					//! Move the window to front according to Flags()
	virtual	void	FocusRequested();					//! Gives focus to the window according to Flags()
	virtual	void	MoveSizeRequested(uint32 anchors);	//! Resize or move the window according to Flags()
	//@}

	//! Should not be overriden, unless you know what you're doing.
	virtual IWindowModel::ptr	WindowModel() const;

	//! Convenience calls to retrieve the state of a window
	bool				IsMinimized() const;
	bool				IsFull() const;
	bool				IsFront() const;
	bool				IsActive() const;
	BString				Title() const;
	uint32				Workspaces() const;
	window_feel			Feel() const;
	window_look			Look() const;

protected:
	/*! Convenience calls on the WindowManager
		These are meant to be called from implementation
		typically (but not necessarily) from their xxxRequested()
		counterpart.
	*/ 
	//{@
	void		Close();
	void		Minimize();
	void		Maximize();
	void		Full();
	void		SendToFront();
	void		SendToBack();
	void		SendBehind(const IView::ptr& window);
	void		Activate(bool focus=true);
	void		MoveSize(uint32 anchors);
	void		SetTitle(const BString& title);
	void		SetWorkspaces(uint32 ws);
	void		SetFeel(window_feel look);
	void		SetLook(window_look feel);
	//@}

	/*! For implementation. Do not override in the regular case
		or call directly.
		If overriden, be sure to call this version AFTER your
		implementation and do not hold any non-nested lock.
		The implementation might call WindowModel().
	*/
	virtual status_t	SetWindowModel(const IWindowModel::ref&);

private:
	enum window_action {
		W_CLOSE,
		W_MINIMIZE, W_MAXIMIZE,
		W_FULL,
		W_FRONT, W_BACK
	};
	void action(window_action a);
	static void get_look_flags(uint32 flags, int32 *lookFlags, int32 *lookHandles);

	IWindowModel::ref				m_model;
	uint32							m_flags;
	mutable BNestedLocker			m_lock;
	mutable BString					m_title;
	mutable uint32					m_workspaces;
	mutable window_look				m_look;
	mutable window_feel				m_feel;
};

/**************************************************************************************/

} } // namespace B::Wm2

#endif	/* _WM2_WINDOW_H_ */
