#ifndef _INTERFACE2_BUTTONVIEW_H_
#define _INTERFACE2_BUTTONVIEW_H_

#include <interface2/ModelTarget.h>
#include <interface2/View.h>

namespace B {
namespace Interface2 {

using namespace Support2;

class BButtonView : public BView, public BModelTarget 
{
	public:
		B_STANDARD_ATOM_TYPEDEFS(BButtonView)
		
							BButtonView(IContext::arg context, IModel::arg model);
							BButtonView(const BButtonView &copyFrom);
		
		virtual	BView *		Copy() { return new BButtonView(*this); }	
		virtual void 		Draw(const IRender::ptr& into);
		virtual	void 		DispatchEvent(const BMessage &msg, const BPoint& where, event_dispatch_result *result);

		virtual	event_dispatch_result	KeyDown(const BMessage &msg, const char *bytes, int32 numBytes);
		virtual	event_dispatch_result	KeyUp(const BMessage &msg, const char *bytes, int32 numBytes);
		virtual event_dispatch_result	MouseDown(const BMessage &msg, const BPoint &where, int32 buttons, int32 modifiers);
		virtual event_dispatch_result	MouseMoved(const BMessage &msg, const BPoint &where, int32 buttons, int32 modifiers, int32 transit);
		virtual event_dispatch_result	MouseUp(const BMessage &msg, const BPoint &where, int32 buttons, int32 modifiers);
		virtual event_dispatch_result	WheelMoved(const BMessage &msg, coord xDelta, coord yDelta);

	
	protected:
		virtual				~BButtonView();
		
		virtual	status_t	Acquired(const void* id);
		virtual	status_t	Released(const void* id);

		virtual	BValue		ModelInterfaceDescriptors() const;
		virtual	status_t	HandleModelEffect(const BValue &in, const BValue &inBindings, const BValue &outBindings, BValue *out);

	private:

};

} } // namespace B::Interface2

#endif /* _INTERFACE2_BUTTONVIEW_H_ */
