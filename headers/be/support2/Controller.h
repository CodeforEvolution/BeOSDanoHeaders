
#ifndef _SUPPORT2_CONTROLLER_H_
#define _SUPPORT2_CONTROLLER_H_

#include <support2/Binder.h>

namespace B {
namespace Support2 {

/**************************************************************************************/

class BController : public BBinder
{
	public:
		B_STANDARD_ATOM_TYPEDEFS(BController);

								BController(IContext::sptr_ref ctxt, const BValue &data = BValue::undefined);
		virtual					~BController();

				BValue			Value() const;

				const BValue	ValueBinding() const;
				const BValue	EventBinding() const;

		virtual	status_t		HandleValue(const BValue &value);
		virtual	status_t		HandleEvent(const BValue &event);

	protected:

				void			SetValue(const BValue &value);

	private:

		virtual	status_t		Told(BValue &in);

				BValue			m_value;
};

/**************************************************************************************/

} } // namespace B::Support2

#endif	/* _SUPPORT2_CONTROLLER_H_ */
