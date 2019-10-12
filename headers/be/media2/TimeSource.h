#ifndef C_TIMESOURCE_H

#define C_TIMESOURCE_H

#include "ITimeSource.h"

#include <support2/Binder.h>

namespace B {
namespace Media2 {

using namespace Support2;

class LTimeSource : public LInterface<ITimeSource>
{
		static	const struct effect_action_def kActions[];

		static	BValue		hook_Now(IInterface::sptr_ref i);
		static	BValue		hook_SnoozeUntil(IInterface::sptr_ref i, const BValue & v);

	public:
		LTimeSource (B::Support2::IContext::sptr_ref context);	

		virtual	status_t	HandleEffect(const BValue & in,
										 const BValue & inBindings,
										 const BValue & outBindings,
										 BValue * out);
};

} } // B::Media2

#endif
