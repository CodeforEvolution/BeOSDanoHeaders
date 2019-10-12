/***************************************************************************
//
//	File:			media2/Buffer.h
//
//	Description:	Represents a shared chunk of memory.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _MEDIA2_BUFFER_H_
#define _MEDIA2_BUFFER_H_

#include <support2/Atom.h>
#include <support2/Value.h>

#include <media2/MediaDefs.h>
#include <media2/MediaFormat.h>

#include <OS.h>

namespace B {

namespace Private {
	struct media_buffer_source;
	class AreaCloneCache;
	class BufferSourceCache;
}

namespace Media2 {

class BBufferGroup;

using namespace Support2;

class BBuffer
{
public:
			enum special_id
			{
				ANY_BUFFER		= -2,
				INVALID_BUFFER	= -1
			};

	//!	instantiates an invalid object
									BBuffer();
	//!	reference local data
									BBuffer(void * data, size_t size);
	//!	reference local data, avoids an area lookup
									BBuffer(void * data, area_id local_area, size_t offset, size_t size);
									BBuffer(const BValue & source);
									BBuffer(const BBuffer & clone);
			BBuffer &				operator=(const BBuffer & clone);
									~BBuffer();

	//!	reference local data
			status_t				SetTo(void * data, size_t size);
	//!	reference local data, avoids an area lookup
			status_t				SetTo(void * data, area_id local_area, size_t offset, size_t size);
			status_t				InitCheck() const;

			int32					ID() const;
			area_id					Area() const;
			size_t					AreaOffset() const;
			size_t					Size() const;
			void *					Data() const;

	//! these three do not take the current range into account
			void *					Base() const;
			size_t					DataOffset() const;
			size_t					TotalSize() const;
			
			media_buffer_source_id	Source() const;
			size_t					SourceOffset() const;

			const BValue &			Info() const;
			BValue &				EditInfo();
			void					SetInfo(const BValue & info);
			
			BValue					AsValue() const;
	inline							operator BValue() const { return AsValue(); }
	
	//!	sets object to an invalid state
			void					Clear();

	/*!	constrains the range of data this buffer references.  multiple calls will
		have a cumulative effect. */
			status_t				SetRange(size_t offset, size_t size);
			void					ClearRange();
			
	/*!	note that these don't operate like the BAtom reference-counting methods.
		when a buffer's reference count drops to zero, ownership of the memory
		region it represents transfers back to the source -- the actual BBuffer
		instance won't be deleted, and it is in fact safe to construct a BBuffer
		on the stack.
		AcquireBuffer() and ReleaseBuffer() will only succeed if the buffer has
		been acquired from a buffer source (calls to AcquireBuffer() after the final
		ReleaseBuffer() will fail.) */
			status_t				AcquireBuffer();
			status_t				ReleaseBuffer();

	/*!	IBufferSource implementations call this to assign each BBuffer
		instance a unique ID.  AttachToSource() will fail if the BBuffer has
		already been assigned to a buffer source. */
			status_t				AttachToSource(media_buffer_id id,
												   media_buffer_source_id source,
												   size_t source_offset);

private:
			enum flag_t {
				RELEASED			= 1,
				RANGE_SPECIFIED		= 2
			};

			status_t				SetTo(
										void * data, area_id remote, size_t offset, size_t size,
										media_buffer_id id, media_buffer_source_id source, size_t source_offset,
										void * source_data, const BValue & info, ::B::Private::AreaCloneCache * cache);

			void					Init();
			void					Init(void * data, size_t size);
			void					Init(void * data, area_id area, size_t offset, size_t size);
			void					Init(void * data, area_id remote, size_t offset, size_t size,
										media_buffer_id id, media_buffer_source_id source, size_t source_offset,
										void * source_data, const BValue & info, ::B::Private::AreaCloneCache * cache);

			status_t				Import(const BValue & value);

	friend	class BMediaOutput;
	friend	class ::B::Private::BufferSourceCache;

			status_t				mInitStatus;
			media_buffer_id			mID;

			area_id					mArea;
			size_t					mOffset;
			size_t					mSize;
	mutable	void *					mData;
			size_t					mSubOffset;
			size_t					mSubSize;

			area_id					mListArea;
			size_t					mListOffset;
			B::Private::media_buffer_source *	mList;
			int32					mFlags;
			
			BValue					mInfo;
			B::Private::AreaCloneCache *		mCache;
};

inline size_t 
BBuffer::Size() const
{
	return mSubSize;
}

inline void *
BBuffer::Data() const
{
	return (int8*)mData + mSubOffset;
}

inline void *
BBuffer::Base() const
{
	return mData;
}

inline size_t 
BBuffer::DataOffset() const
{
	return mSubOffset;
}

inline size_t 
BBuffer::TotalSize() const
{
	return mSize;
}

}; }; // namespace B::Media2
#endif //_MEDIA2_BUFFER_H_
