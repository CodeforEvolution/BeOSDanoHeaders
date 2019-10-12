#ifndef C_MEDIA_SEEKABLE_INTERFACE_H

#define C_MEDIA_SEEKABLE_INTERFACE_H

#include <support2/IInterface.h>

namespace B {
namespace Media2 {

using B::Support2::atom_ptr;
using B::Support2::atom_ref;
using B::Support2::BValue;
using B::Support2::IBinder;

class IMediaSeekable : public ::B::Support2::IInterface
{
	public:
		enum media_seek_type
		{
			B_MEDIA_SEEK_FRAME,
			B_MEDIA_SEEK_TIME
		};
		
		enum media_seek_proximity
		{
			B_MEDIA_SEEK_CLOSEST_ANY,
			B_MEDIA_SEEK_CLOSEST_FORWARD,
			B_MEDIA_SEEK_CLOSEST_BACKWARD
		};
	
	public:
		B_DECLARE_META_INTERFACE(MediaSeekable)

		/*! EstimateFrameCount tries to "estimate" the number of frames
		contained in the underlying media, it should give a rough estimate
		rather than spend a lot of time scanning the stream */
		
		virtual status_t EstimateFrameCount (uint64 *out_count) = 0;
		
		//! The same basic guidelines hold for EstimateDuration
		
		virtual status_t EstimateDuration (bigtime_t *out_duration) = 0;

		/*! GetFrameCount returns an accurate number of frames if possible
		return an error if you cannot ensure that the returned frame count is
		perfectly fine. */
		
		virtual status_t GetFrameCount (uint64 *out_count) = 0;
		
		//! The same hold true for GetDuration
		
		virtual status_t GetDuration (bigtime_t *out_duration) = 0;

		/*! Seek tries to get close to the desired frame/time (depending on the type)
		it returns an estimate of the actual position it seeked to in
		*inout_time and *inout_frame */
		
		virtual status_t Seek (uint64 *inout_frame,
								bigtime_t *inout_time,
								media_seek_type type,
								media_seek_proximity flags = B_MEDIA_SEEK_CLOSEST_ANY) = 0;
};

} } // B::Media2

#endif
