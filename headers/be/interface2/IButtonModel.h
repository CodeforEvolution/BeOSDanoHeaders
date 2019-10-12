
#ifndef _INTERFACE2_IBUTTONMODEL_H_
#define _INTERFACE2_IBUTTONMODEL_H_

#include <support2/Binder.h>

namespace B {
namespace Interface2 {
using namespace Support2;

/**************************************************************************************/

enum button_behavior {
	B_ONE_STATE_BUTTON = 0,
	B_TWO_STATE_BUTTON = 1
};
		
class IButtonModel : public IInterface
{
	public:
		B_DECLARE_META_INTERFACE(ButtonModel);

		virtual	button_behavior	Behavior() = 0;
		virtual	const BString	Label() = 0;
		
		virtual	bool			IsEnabled() = 0;
		virtual	bool			IsPressed() = 0;
		// XXX rollover state should be moved to view.
		virtual	bool			IsRollover() = 0;
		virtual	bool			IsSelected() = 0;
		
		virtual	void			SetBehavior(button_behavior behavior) = 0;
		virtual	void			SetEnabled(bool enabled) = 0;
		virtual	void			SetLabel(const char *label) = 0;
		virtual	void			SetPressed(bool pressed) = 0;
		virtual	void			SetRollover(bool rollmeover) = 0;
		virtual	void			SetSelected(bool selectwho) = 0;
	
		static	const BValue 	key_Behavior;
		static	const BValue 	key_Enabled;
		static	const BValue 	key_Label;
		static	const BValue 	key_Pressed;
		static	const BValue 	key_Rollover;
		static	const BValue 	key_Selected;
};

/**************************************************************************************/

class LButtonModel : public LInterface<IButtonModel>
{
	protected:
		inline							LButtonModel(const IContext::ptr& context) : LInterface<IButtonModel>(context) { }
		inline							LButtonModel(const LButtonModel& other) : LInterface<IButtonModel>(other) { }
		virtual							~LButtonModel() { }
		
		virtual	status_t				HandleEffect(	const BValue &in,
														const BValue &inBindings,
														const BValue &outBindings,
														BValue *out);
};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_IVIEW_H_ */
