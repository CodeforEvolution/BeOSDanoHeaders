#ifndef _MEDIA2_MEDIAPUSHSOURCE_INTERFACE_H_
#define _MEDIA2_MEDIAPUSHSOURCE_INTERFACE_H_

#include <media2/IMediaPushSource.h>
#include <support2/Binder.h>

namespace B {
namespace Media2 {

using namespace Support2;

class LMediaPushSource : public LInterface<IMediaPushSource>
{
		static	const struct effect_action_def kActions[];

		static	BValue		hook_Start(IInterface::sptr_ref i, const BValue & v);
		static	BValue		hook_Stop(IInterface::sptr_ref i);
		static	BValue		hook_PushBuffer(IInterface::sptr_ref i);
	public:
		virtual	status_t	HandleEffect(const BValue & in,
										 const BValue & inBindings,
										 const BValue & outBindings,
										 BValue * out);
};

class BMediaPushSource : public LMediaPushSource
{
public:
	B_STANDARD_ATOM_TYPEDEFS(BMediaPushSource)
	
						BMediaPushSource();
	virtual status_t	Start(bool usethisthread = false);
	virtual status_t	Stop();
	virtual status_t	PushBuffer() = 0;


private:
	thread_id			looper;
	static int32		startloop(void *arg);
	int32				RunLoop();
	volatile bool		fRunning;
};

}; }; // namespace B::Media2
#endif //_MEDIA2_MEDIAPUSHSOURCE_INTERFACE_H_
