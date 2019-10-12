#ifndef _MEDIA2_MEDIAPUSHSOURCE_INTERFACE_
#define _MEDIA2_MEDIAPUSHSOURCE_INTERFACE_

#include <support2/IInterface.h>
#include <support2/IBinder.h>
#include <media2/MediaDefs.h>

namespace B {
namespace Media2 {

using namespace Support2;

class IMediaPushSource : public IInterface
{
public:
	B_DECLARE_META_INTERFACE(MediaPushSource);

	/*!	repeatedly call PushBuffer().
		/param usethisthread if true, takes over the current thread; otherwise
		and by default a new thread is spawned. */
	virtual status_t Start(bool usethisthread = false) = 0;
	//!	shut down the PushBuffer() loop.
	virtual status_t Stop() = 0;
	//!	acquire, fill, and send one BBuffer to the downstream input.
	virtual status_t PushBuffer() = 0;
};

}; }; // namespace B::Media2
#endif //_MEDIA2_MEDIAPUSHSOURCE_INTERFACE_
