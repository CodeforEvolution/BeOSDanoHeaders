/***************************************************************************
//
//	File:			interface2/IModel.h
//
//	Description:	IModel interface
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _INTERFACE2_IMODEL_H_
#define _INTERFACE2_IMODEL_H_

#include <support2/Binder.h>
#include <support2/Value.h>
#include <support2/Message.h>
#include <interface2/IView.h>

namespace B {
namespace Interface2 {
using namespace Support2;

/**************************************************************************************/

//!	These are the standard kinds of model views that can be requested.
enum model_view_type {
	B_DEFAULT_MODEL_VIEW,	//!< Request the basic editable view for this model.
	B_PRINT_MODEL_VIEW,		//!< Request a view for printing the model.
	B_STATIC_MODEL_VIEW		//!< Request a static (no editing) view for the model.
};

class IModel : public IInterface
{
public:

	B_DECLARE_META_INTERFACE(Model);

	//!	Return a new view on this model.
	virtual	IView::ptr				View(	model_view_type type = B_DEFAULT_MODEL_VIEW,
											const BValue &options = BValue::undefined,
											const BValue &attr = BValue::undefined) = 0;
	
	//!	Handle an event in the model.  Should probably go away.
	virtual void					DispatchEvent(const BMessage &msg, const atom_ptr<IView>& view = NULL) = 0;
};

/**************************************************************************************/

class LModel : public LInterface<IModel>
{
protected:
									LModel(const IContext::ptr& context);
	virtual							~LModel();
	
	virtual	status_t				HandleEffect(	const BValue &in,
													const BValue &inBindings,
													const BValue &outBindings,
													BValue *out);
};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_IMODEL_H_ */
