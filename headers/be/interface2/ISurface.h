/***************************************************************************
//
//	File:			interface2/ISurface.h
//
//	Description:	ISurface interface.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef	_INTERFACE2_ISURFACE_H_
#define	_INTERFACE2_ISURFACE_H_

#include <support2/IInterface.h>
#include <support2/StaticValue.h>
#include <interface2/InterfaceDefs.h>
#include <interface2/View.h>
#include <interface2/IViewParent.h>

namespace B {
namespace Interface2 {

/**************************************************************************************/

class ISurface : public IInterface
{
public:
	B_DECLARE_META_INTERFACE(Surface);

	//! Set \p hostView as the root view of the surface
	virtual	status_t			SetHost(const IView::ptr& hostView) = 0;

	//! Returns the parent of the host view (the surface, that is)
	virtual	IViewParent::ptr	ViewParent() = 0;

	//! Takes a screen shot of the surface
	virtual status_t			ScreenShot(const IByteOutput::sptr& stream) = 0;

	static	const static_small_value	key_SetHost;
	static	const static_large_value	key_ScreenShot;
};

/**************************************************************************************/

class LSurface : public LInterface<ISurface>
{
public:
	virtual	status_t	Told(BValue &in);
	virtual	status_t	Asked(const BValue &outBindings, BValue &out);
};

/**************************************************************************************/

} }	// namespace B::Interface2

#endif 	/* _INTERFACE2_ISURFACE_H_ */
