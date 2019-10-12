/***************************************************************************
//
//	File:			media2/MediaProducerContext.h
//
//	Description:	Represents a set of endpoints that operate on buffers
//					from a given source.  The root of the graph is the
//					producer output.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _MEDIA2_MEDIAPRODUCERCONTEXT_H_
#define _MEDIA2_MEDIAPRODUCERCONTEXT_H_

#include <support2/Binder.h>
#include <media2/MediaEndpoint.h>

namespace B {
namespace Media2 {

using namespace ::B::Support2;

class BMediaProducerContext : public BBinder
{
public:
	B_STANDARD_ATOM_TYPEDEFS(BMediaProducerContext);

									BMediaProducerContext(BMediaOutput::sptr_ref producer);
	virtual							~BMediaProducerContext();
	
	virtual	status_t				Acquired(const void * id);
	virtual	status_t				Released(const void * id);

	virtual	status_t				Told(BValue & in);

			ssize_t					ListEndpoints(
										BMediaEndpointVector * outEndpoints,
										int32 type = B_ANY_ENDPOINT_TYPE,
										int32 state = B_ANY_ENDPOINT_STATE);

			/*!	create a buffer source: first delegate to endpoints (starting as far
				downstream as possible) then call MakeBufferSource() if none of
				the endpoints returned a buffer source. */
	virtual	status_t				AllocateBuffers(
										const BMediaConstraint & constraint);
			status_t				AllocateBuffers();
			IBufferSource::sptr		BufferSource() const;
			status_t				FreeBuffers();
			
			/*!	add another producer output, linked to the root of the graph. */
			status_t				LinkProducer(BMediaOutput::sptr_ref producer);
			
			//!	release all endpoints
			void					MakeEmpty();

protected:
	virtual	IBufferSource::sptr		MakeBufferSource(
										size_t bufferCount,
										size_t bufferSize,
										uint32 lock);

private:
	class	impl;
			impl *		i;

	class	allocate_state;			

			status_t	_ConnectRecurse(IMediaOutput::sptr_ref output, IMediaInput::sptr_ref input);
			status_t	Disconnect(IMediaOutput::sptr_ref output, IMediaInput::sptr_ref input);
			status_t	RemoveEndpoint(IMediaEndpoint::sptr_ref endpoint, IMediaNode::sptr_ref node);
			
	static	void		AcquireEndpointHook(IMediaEndpoint::sptr_ref endpoint, void * context);
	static	void		ReleaseEndpointHook(IMediaEndpoint::sptr_ref endpoint, void * context);

			status_t	_AcquireEndpoint(IMediaEndpoint::sptr_ref endpoint);
			status_t	_ReleaseEndpoint(IMediaEndpoint::sptr_ref endpoint);

	static	status_t	AllocateVisitForward(IMediaEndpoint::sptr_ref endpoint, void * state);
	static	status_t	FreeVisitForward(IMediaEndpoint::sptr_ref endpoint, void *);
	
			status_t	_FreeBuffers();
};

} } // namespace B::Media2
#endif //_MEDIA2_MEDIAPRODUCERCONTEXT_H_
