/***************************************************************************
//
//	File:			render2/IVisual.h
//
//	Description:	Abstract visual interface.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _RENDER2_VISUAL_H_
#define _RENDER2_VISUAL_H_

#include <support2/Binder.h>
#include <support2/StaticValue.h>
#include <render2/Rect.h>

namespace B {
namespace Render2 {

using namespace Support2;

/**************************************************************************************/

class IRender;
class BUpdate;

class IVisual : public IInterface
{
public:

	B_DECLARE_META_INTERFACE(Visual);

	virtual	void					Display(const atom_ptr<IRender>& into) = 0;
	virtual	void					Draw(const atom_ptr<IRender>& into) = 0;
	virtual void					Invalidate(const BUpdate& update) = 0;
	virtual BRect					Frame() const = 0;
	
	static	value_csml				key_Display;
	static	value_csml				key_Draw;
	static	value_clrg				key_Invalidate;
	static	value_csml				key_Frame;
};

/**************************************************************************************/

class LVisual : public LInterface<IVisual>
{
protected:
	virtual	status_t	HandleEffect(	const BValue &in,
										const BValue &inBindings,
										const BValue &outBindings,
										BValue *out);
};

/**************************************************************************************/

} } // namespace B::Render2

#endif	// _RENDER2_VISUAL_H_
