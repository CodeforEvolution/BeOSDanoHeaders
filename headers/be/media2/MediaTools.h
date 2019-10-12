#ifndef _MEDIA2_TOOLS_H_

#define _MEDIA2_TOOLS_H_

#include <support2/IContext.h>
#include <media2/IMediaEndpoint.h>
#include <media2/IMediaNode.h>
#include <media2/IMediaExtractor.h>

namespace B {
namespace Media2 {

status_t GetExtractor (B::Support2::IContext::sptr_ref context,
						B::Support2::IByteInput::sptr_ref stream,
						B::Media2::IMediaExtractor::sptr *extractor);
						
status_t GetDecoderChain (B::Support2::IContext::sptr_ref context,
							IMediaOutput::sptr_ref starting_at,
							IMediaNode::sptr *collective);

status_t FindMediaComponentByConstraint (B::Support2::IContext::sptr_ref context,
											const BMediaConstraint *input,
											const BMediaConstraint *output,
											B::Support2::BValue &results);

} } // B::Media2

#endif
