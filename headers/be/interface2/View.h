
#ifndef _INTERFACE2_VIEW_H_
#define _INTERFACE2_VIEW_H_

#include <interface2/ParentedView.h>
#include <support2/Locker.h>

namespace B {
namespace Interface2 {

/**************************************************************************************/

class BView : public BParentedView
{
	public:

		/*!
			Effective arguments for the BView(BValue) constructor (keys in
			\a attr whose values we pay attention to):
			
			"bounds"	:	BRect
			
			"left",		:	string, e.g.: "5px"    --  5 pixels
			"right",		              "7.1cm"  --  7.1 centimeters
			"top",			              "24pt"   --  24 points
			"bottom"		              "25%"    --  25% of parent's size
							              "1/3nm"  --  0.33333333 of parent's size
							 Setting "right" places your right edge the given
							 distance from the right edge of your parent.  That is,
							 right="4px" means your right edge is four pixels in from
							 your parent's right edge.  "bottom" works similarly.
							 "left" and "top" set your distances from your parent's
							 left and top edges, respectively.
							 
							Numeric type (int, float, etc.): Assumed to be in pixels.
							 Note that the value passes through a coord, so you may
							 lose precision at that point.
							 
			"width",	:	string: "[all]", "[min] [max]", or "[min] [pref] [max]",
			"height"		 where [all], [min], [max], and [pref] are specified as
							 above.
							
							Numeric type: Assumed to be in pixels.  Sets all of min,
							 max, and pref to the specified value.
							 
			"prefw",	:	string: Interpreted as for left/right/top/bottom.
			"prefh"			Numeric type: Assumed to be in pixels.

			Values from later in this list override values from earlier.  E.g.,
			if you specify both "bounds" and "left", the value from "left" will
			override bounds.left.
		*/

										BView(const BValue &attr = BValue::undefined);
										BView(const BView &copyFrom);
		virtual							~BView();

		virtual	BView *					Copy() = 0;

		virtual	bool					SetTransform(const B2dTransform& transform);
		virtual bool					SetFrame(const BRect& bounds);
		virtual	bool					SetExternalConstraints(const BLayoutConstraints &c);
		virtual	void					Hide();
		virtual	void					Show();

		virtual	void					PreTraversal();
		virtual	IView::ptr				PostTraversal(BUpdate &outDirty);

		virtual	void					Display(const IRender::ptr& into);
		virtual	void					Draw(const IRender::ptr& into);
		virtual	void					DispatchEvent(	const BMessage &msg,
														const BPoint& where,
														event_dispatch_result *result = NULL);

		virtual	event_dispatch_result 	KeyDown(const BMessage &msg, const char *bytes, int32 numBytes);
		virtual	event_dispatch_result	KeyUp(const BMessage &msg, const char *bytes, int32 numBytes);
		virtual event_dispatch_result	MouseDown(const BMessage &msg, const BPoint &where, int32 buttons, int32 modifiers);
		virtual event_dispatch_result	MouseMoved(const BMessage &msg, const BPoint &where, int32 buttons, int32 modifiers, int32 transit);
		virtual event_dispatch_result	MouseUp(const BMessage &msg, const BPoint &where, int32 buttons, int32 modifiers);
		virtual event_dispatch_result	WheelMoved(const BMessage &msg, coord xDelta, coord yDelta);

		virtual	BLayoutConstraints		Constraints() const;
		virtual	BRegion					Shape() const;
		virtual BRect					Frame() const;
		virtual	B2dTransform			Transform() const;
		virtual	bool					IsHidden() const;

	protected:

				BRect					Bounds() const;

		virtual	BView *					OpenLayoutTransaction();
		virtual	status_t				Acquired(const void* id);
		virtual	status_t				Released(const void* id);

				enum {
					lfVisible	= 0x00000001,
				};
				
				BLayoutConstraints		ExternalConstraints() const;

				uint32					Flags() const;
				uint32					SetFlags(uint32 flags, bool on);
				
		lock_status_t					LockTransaction() const;
				void					UnlockTransaction() const;
				const BView *			Transaction() const;
				BView *					Transaction();

	private:
	
				BLayoutConstraints*		edit_external_constraints();
				
				void					init_edge(const BValue &attr, const char *name,
											void (BLayoutConstraints::*set_func)(dimth set_to));
				void					init_size(const BValue &attr, const char *name,
											orientation orient);
				void					init_pref_size(const BValue &attr, const char *name,
											void (BLayoutConstraints::*set_func)(dimth set_to, bool set_all));
											
				void					construct(const BValue& attr);

				BRect					m_frame;
				BLayoutConstraints*		m_externalConstraints;
				B2dTransform			m_transform;
				atom_ptr<BView>			m_openTransaction;
				uint32					m_flags;
		mutable	BNestedLocker			m_transactionLock;
};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_VIEW_H_ */
