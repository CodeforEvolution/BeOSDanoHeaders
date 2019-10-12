/***************************************************************************
//
//	File:			media2/IBufferSource.h
//
//	Description:	Interface that provides access to an abstract buffer source.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _MEDIA2_BUFFERSOURCE_INTERFACE_
#define _MEDIA2_BUFFERSOURCE_INTERFACE_

#include <media2/MediaDefs.h>
#include <media2/Buffer.h>
#include <support2/IInterface.h>
#include <support2/Binder.h>

namespace B {
namespace Media2 {

using namespace Support2;

class IBufferSource : public IInterface
{
	public:
		B_DECLARE_META_INTERFACE(BufferSource);
	
		//!	return buffer ID or an error code
		virtual	int32				AcquireBuffer(
										BBuffer * outBuffer = 0,
										int32 id = BBuffer::ANY_BUFFER,
										bigtime_t timeout = B_INFINITE_TIMEOUT) = 0;
	
		//!	describe the buffers that may be returned by AcquireBuffer(). 
		virtual ssize_t				ListBuffers(
										BVector<BBuffer> * outBuffers) const = 0;
	
		//!	describe the shared-memory area used to manage buffer allocation and reclamation.
		virtual	void				GetListArea(
										area_id * outArea,
										size_t * outOffset) const = 0;
};

class LBufferSource : public LInterface<IBufferSource>
{
		static	const struct effect_action_def kActions[];
		static	BValue		hook_AcquireBuffer(IInterface::sptr_ref i, const BValue & v);
		static	BValue		hook_ListBuffers(IInterface::sptr_ref i);
		static	BValue		hook_GetListArea(IInterface::sptr_ref i);
	public:
		B_STANDARD_ATOM_TYPEDEFS(LBufferSource);

		virtual	status_t			HandleEffect(const BValue & in,
										 const BValue & inBindings,
										 const BValue & outBindings,
										 BValue * out);

		virtual	status_t			Transact(uint32 code,
										 BParcel & data,
										 BParcel * reply = 0,
										 uint32 flags = 0);
};

} } // namespace B::Media2
#endif //_MEDIA2_BUFFERSOURCE_INTERFACE_
