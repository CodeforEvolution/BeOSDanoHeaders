#ifndef C_MEDIA_EXTRACTOR_INTERFACE_H

#define C_MEDIA_EXTRACTOR_INTERFACE_H

#include <support2/IByteStream.h>

namespace B {
namespace Media2 {

using namespace Support2;

class IMediaExtractor : public IInterface
{
	public:
		B_DECLARE_META_INTERFACE(MediaExtractor)

		/*! Sniff is called to roughly identify the stream and your capability
		of dealing with its contents. Avoid seeking the stream, avoid lengthy
		operations. The quality setting should reflect how optimistic you are
		to know this format (don't return 1.0f based on checking 4 bytes for example)
		\param quality returns an estimate of your probability to handle this stream
		in the range (0.0f-1.0f)
		\returns B_OK if successful or an appropriate error-code.
		Don't return an error if you cannot handle the stream but return a quality
		of 0.0f instead. errors are intended for exceptional situations such as
		I/O errors */
				
		virtual status_t Sniff (IByteInput::sptr_ref stream,
								float *quality) = 0;

		/*! AttachedToStream is called to give you the chance of setting up
		internal data structures, publish outputs (\sa BMediaOutput,BMediaEndpoint)
		and set up the corresponding constraints. This is never called if you returned
		an error or a "quality" of 0.0f from Sniff.
		\param stream contains an atom_ptr to the \sa IByteInput representing the stream
		\returns B_OK if successful or an appropriate error-code.
		You may return an error here if you cannot handle the stream even if
		you advertised a non-zero "quality" in Sniff */
		
		virtual status_t AttachedToStream (IByteInput::sptr_ref stream) = 0;
		
		/*! DetachedFromStream is called to let you free internal data-structures */
		
		virtual void DetachedFromStream() = 0;
};

} } // B::Media2

#endif
