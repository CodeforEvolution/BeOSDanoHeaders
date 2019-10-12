
#ifndef _INTERFACE2_IVIEW_H_
#define _INTERFACE2_IVIEW_H_

#include <support2/Binder.h>
#include <support2/Message.h>
#include <support2/StaticValue.h>
#include <render2/Rect.h>
#include <render2/Region.h>
#include <render2/IRender.h>
#include <render2/2dTransform.h>
#include <interface2/LayoutConstraints.h>

namespace B {
namespace Render2 {
	class BUpdate;
}}

namespace B {
namespace Interface2 {

class IViewParent;

/**************************************************************************************/

class IView : public IVisual
{
	public:

		B_DECLARE_META_INTERFACE(View)

		virtual	status_t				SetParent(const atom_ptr<IViewParent> &parent) = 0;
		virtual	bool					SetTransform(const B2dTransform& transform) = 0;
		virtual bool					SetFrame(const BRect& bounds) = 0;
		virtual	bool					SetExternalConstraints(const BLayoutConstraints &c) = 0;
		virtual	void					Hide() = 0;
		virtual	void					Show() = 0;

		virtual	void					PreTraversal() = 0;
		virtual	IView::sptr				PostTraversal(BUpdate &outDirty) = 0;

		virtual	BLayoutConstraints		Constraints() const = 0;
		virtual	atom_ptr<IViewParent>	Parent() const = 0;
		virtual	BRegion					Shape() const = 0;
		virtual	B2dTransform			Transform() const = 0;

		virtual	bool					IsHidden() const = 0;

		virtual	void					DispatchEvent(	const BMessage &msg,
														const BPoint& where,
														event_dispatch_result *result = NULL) = 0;
		
		static	value_clrg				key_SetParent;
		static	value_clrg				key_SetTransform;
		static	value_clrg				key_SetFrame;
		static	value_clrg				key_SetExternalConstraints;
		static	value_csml				key_Hide;
		static	value_csml				key_Show;
		static	value_clrg				key_PreTraversal;
		static	value_clrg				key_PostTraversal;
		static	value_clrg				key_Constraints;
		static	value_csml				key_Parent;
		static	value_csml				key_Shape;
		static	value_clrg				key_Transform;
		static	value_clrg				key_IsHidden;
		static	value_clrg				key_DispatchEvent;
};

/**************************************************************************************/

class LView : public LInterface<IView>
{
public:
	virtual	BValue		Inspect(const BValue &which, uint32 flags = 0);

protected:
						LView();
						LView(IContext::sptr_ref context);
						LView(const LView& other);
	virtual				~LView();
	virtual	status_t	HandleEffect(	const BValue &in,
										const BValue &inBindings,
										const BValue &outBindings,
										BValue *out);
};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_IVIEW_H_ */
