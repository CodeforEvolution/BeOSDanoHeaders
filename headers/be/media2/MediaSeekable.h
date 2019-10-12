#ifndef C_MEDIA_SEEKABLE_H

#define C_MEDIA_SEEKABLE_H

#include "IMediaSeekable.h"

#include <support2/Binder.h>

namespace B {
namespace Media2 {

using namespace Support2;

class LMediaSeekable : public LInterface<IMediaSeekable>
{
		static	const struct effect_action_def kActions[];

		static	BValue		hook_GetFrameCount(IInterface::sptr_ref i, const BValue & v);
		static	BValue		hook_GetDuration(IInterface::sptr_ref i, const BValue & v);
		static	BValue		hook_Seek(IInterface::sptr_ref i, const BValue & v);
	public:
		virtual	status_t	HandleEffect(const BValue & in,
										 const BValue & inBindings,
										 const BValue & outBindings,
										 BValue * out);
};

} } // B::Media2

#endif
