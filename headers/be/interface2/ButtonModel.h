#ifndef _INTERFACE2_BUTTONMODEL_H_
#define _INTERFACE2_BUTTONMODEL_H_

#include <interface2/IButtonModel.h>
#include <interface2/Model.h>
#include <support2/Binder.h>
#include <support2/Locker.h>
#include <support2/String.h>

namespace B {
namespace Interface2 {

using namespace Support2;

// These are the keys for the BButtonModel binder protocol.

class BButtonModel : public BModel, public LButtonModel
{
	public:
		B_STANDARD_ATOM_TYPEDEFS(BButtonModel)
		
								BButtonModel(IContext::ptr context);
		
		virtual	button_behavior	Behavior();
		virtual	const BString	Label();
		
		virtual	bool			IsEnabled();
		virtual	bool			IsPressed();
		virtual	bool			IsRollover();
		virtual	bool			IsSelected();
		
		virtual	void			SetBehavior(button_behavior behavior);
		virtual	void			SetEnabled(bool enabled);
		virtual	void			SetLabel(const char *label);
		virtual	void			SetPressed(bool pressed);
		virtual	void			SetRollover(bool rollmeover);
		virtual	void			SetSelected(bool selectwho);
	
		virtual	IView::ptr		View(	model_view_type type = B_DEFAULT_MODEL_VIEW,
										const BValue &options = BValue::undefined,
										const BValue &attr = BValue::undefined);
		
				void			PrintToStream(ITextOutput::arg io, uint32 flags = 0) const;
		
		// Glue BModel and LButtonModel together.
		virtual	BValue			Inspect(const BValue &which, uint32 flags = 0);
		virtual	IContext::ptr	Context() const { return BModel::Context(); }
	
#warning "REMOVE ME"
//START HERE
		virtual void			DispatchEvent(const BMessage &msg, const atom_ptr<IView>& view = NULL) { (void) msg; (void) view; };
//END HERE

	protected:
		virtual					~BButtonModel();
		
		// Glue BModel and LButtonModel together.
		virtual	status_t		Acquired(const void* id);
		virtual	status_t		Released(const void* id);
	
	private:
								BButtonModel(const BButtonModel& other);
		
		BLocker					m_lock;
		BString 				m_label;					
		button_behavior			m_behavior			: 1;
		bool					m_enabled			: 1;
		bool					m_pressed			: 1;
		bool					m_selected			: 1;
		bool					m_youRolledMeOver	: 1;
		
};

ITextOutput::arg	operator<<(ITextOutput::arg io, const BButtonModel& value);

} } // namespace B::Interface2

#endif /* _INTERFACE2_BUTTONMODEL_H_ */
