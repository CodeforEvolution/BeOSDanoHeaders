#ifndef C_MEDIA_EXTRACTOR_H

#define C_MEDIA_EXTRACTOR_H

#include "IMediaExtractor.h"

#include <media2/MediaNode.h>
#include <media2/MediaPushSource.h>
#include <support2/Binder.h>

namespace B {
namespace Media2 {

using namespace Support2;

class LMediaExtractor : public LInterface<IMediaExtractor>
{
		static	const struct effect_action_def kActions[];

		static	BValue		hook_Sniff(IInterface::sptr_ref i, const BValue & v);
		static	BValue		hook_AttachedToStream(IInterface::sptr_ref i, const BValue & v);
		static	BValue		hook_DetachedFromStream(IInterface::sptr_ref i);

	public:
		virtual	status_t	HandleEffect(const BValue & in,
										 const BValue & inBindings,
										 const BValue & outBindings,
										 BValue * out);
};

class BMediaExtractor : public BMediaNode,
						public BMediaPushSource,
						public LMediaExtractor
{
	public:
		B_STANDARD_ATOM_TYPEDEFS(BMediaExtractor)
		
		BMediaExtractor (B::Support2::IContext::sptr_ref context, const char *name);
		virtual	BValue	Inspect(const BValue &which, uint32 flags = 0);
};

} } // B::Media2

#endif
