/***************************************************************************
//
//	File:			media2/MediaNode.h
//
//	Description:	A BMediaNode aggregates one or more endpoints into
//					a functional group, with which one can implement a
//					buffer producer, filter, consumer, or any combination
//					thereof.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _MEDIA2_MEDIANODE_H_
#define _MEDIA2_MEDIANODE_H_

#include <support2/Binder.h>
#include <support2/String.h>

#include <media2/MediaDefs.h>
#include <media2/IMediaNode.h>
#include <media2/IMediaCollective.h>
#include <media2/MediaEndpoint.h>

namespace B {
namespace Media2 {

using namespace Support2;

class LMediaNode : public LInterface<IMediaNode>
{
	static const struct effect_action_def kActions[];
	
	static status_t hook_SetParent (IInterface::sptr_ref i, const BValue & value);
	static BValue hook_Name (IInterface::sptr_ref i);
	static BValue hook_Parent (IInterface::sptr_ref i);
	static BValue hook_ListEndpoints (IInterface::sptr_ref i, const BValue & value);
	static BValue hook_ListLinkedEndpoints (IInterface::sptr_ref i, const BValue & value);
	static BValue hook_FindEndpoint (IInterface::sptr_ref i, const BValue & value);
	static BValue hook_AllocateBuffers (IInterface::sptr_ref i, const BValue & value);
		
	public:
							LMediaNode(IContext::sptr_ref context);
							LMediaNode();

		virtual	status_t	HandleEffect(const BValue & in,
										 const BValue & inBindings,
										 const BValue & outBindings,
										 BValue * out);
};

class BMediaNode : public LMediaNode
{
public:
	B_STANDARD_ATOM_TYPEDEFS(BMediaNode);

									BMediaNode(
										IContext::sptr_ref context,
										const char * name = 0);

									BMediaNode(
										const char * name = 0);
	
	virtual	status_t				AddEndpoint(BMediaEndpoint::sptr_ref endpoint);
	virtual	status_t				RemoveEndpoint(BMediaEndpoint::sptr_ref endpoint);
			BMediaEndpoint::sptr		EndpointAt(size_t index) const;
			size_t					CountEndpoints() const;
			
			/*!	LinkEndpoints(input, output): indicates that 'input' may
				operate in place and retransmit its buffers via 'output'. */
	virtual	status_t				LinkEndpoints(
										BMediaInput::sptr_ref input,
										BMediaOutput::sptr_ref output);

			/*!	ShareBufferSource(with_output, at_output): indicates that the two
				outputs should always use the same buffer source.  Behavior is undefined
				if these outputs are not producers (ie. they've been linked to inputs
				via LinkEndpoints().) */
	virtual	status_t				ShareBufferSource(
										BMediaOutput::sptr_ref at_output,
										BMediaOutput::sptr_ref with_output);

			/*!	[re-]allocate buffers via every BMediaProducerContext that
				intersects this node. */
	virtual	status_t				AllocateBuffers(
										const BMediaConstraint & constraint);
			status_t				AllocateBuffers();

	// ** IMediaNode

	virtual	BString					Name() const;
	virtual	IMediaCollective::sptr	Parent() const;
	virtual	ssize_t					ListEndpoints(
										BMediaEndpointVector * outEndpoints,
										int32 type = B_ANY_ENDPOINT_TYPE,
										int32 state = B_ANY_ENDPOINT_STATE) const;
	virtual	ssize_t					ListLinkedEndpoints(
										IMediaEndpoint::sptr_ref fromEndpoint,
										BMediaEndpointVector * outEndpoints,
										int32 state = B_ANY_ENDPOINT_STATE) const;
	virtual	status_t				FindEndpoint(
										const BString & name,
										IMediaEndpoint::sptr * outEndpoint,
										int32 type = B_ANY_ENDPOINT_TYPE,
										int32 state = B_ANY_ENDPOINT_STATE) const;
													
protected:
	virtual							~BMediaNode();
	
	// ** BUFFER-PROCESSING HOOKS

	//!	base impl: no-op, return empty constraint
	virtual	BMediaConstraint		BufferSourceConstraint(
										BMediaEndpoint::const_sptr_ref for_endpoint) const;

	//!	base impl: no-op, return 0
	virtual	IBufferSource::sptr		MakeBufferSource(
										BMediaEndpoint::sptr_ref endpoint,
										size_t bufferCount,
										size_t bufferSize,
										uint32 lock);

	//!	base impl: error
	virtual	status_t				HandleBuffer(
										BMediaInput::sptr_ref receiver,
										BBuffer * buffer);

	/*!	called when a buffer source has been allocated for use by the given
		endpoint.  if your node publishes a linked endpoints, then this
		method may be called multiple times for the same source. */
	virtual	void					AttachBufferSource(
										BMediaEndpoint::sptr_ref endpoint,
										IBufferSource::sptr_ref source);

	/*!	called when the given buffer source is no longer to be used.  after
		this point buffers may not be acquired from the source, though
		previously-acquired buffers can still be passed across local or
		remote connections.  if your node caches buffers, then it is
		responsible for either releasing the buffers immediately or maintaining
		a primary reference to the buffer source.  otherwise the source could
		be deallocated while buffers still reference its memory area, which
		would be bad. */
	virtual	void					DetachBufferSource(
										BMediaEndpoint::sptr_ref endpoint,
										IBufferSource::sptr_ref source);

	// ** NOTIFICATION

	/*!	a connection between the given endpoints will complete
		immediately after this call returns. */
	virtual	void					AboutToConnect(
										BMediaEndpoint::sptr_ref localEndpoint,
										IMediaEndpoint::sptr_ref remoteEndpoint,
										const BMediaFormat & format);

	//!	a connection between the given endpoints has been completed.
	virtual	void					Connected(
										BMediaEndpoint::sptr_ref localEndpoint,
										IMediaEndpoint::sptr_ref remoteEndpoint,
										const BMediaFormat & format);

	//!	a reservation or connection between the given endpoints has been broken.
	virtual	void					Disconnected(
										BMediaEndpoint::sptr_ref localEndpoint,
										IMediaEndpoint::sptr_ref remoteEndpoint);

	/*! a buffer source has been set on the given endpoint.  when buffer
		allocation completes for a given producer subgraph, this method
		will be called for each endpoint. */
	virtual	void					BufferSourceChanged(
										BMediaEndpoint::sptr_ref endpoint,
										IBufferSource::sptr_ref source);

	// ** EXTENDED CONNECTION HOOKS
	//    You can safely ignore these.
	
	/*!	base impl: no-op, return B_OK
		at this point, constraint has been validated against localOutput's default format (if any.)
		return B_OK to continue the connection process. */
	virtual	status_t				ReserveOutputConnection(
										BMediaOutput::sptr_ref output,
										IMediaInput::sptr_ref remoteInput,
										BMediaConstraint * constraint,
										media_endpoint_state requestedState);

	/*!	base impl: no-op, return B_OK
		the given remote output wishes to form a connection; the given constraints are valid from
		its point of view and have been resolved against the input's default format (if any.)
		you may point ioInput at a new or pre-existing input if you want the original input to
		act as an always-free placeholder.  if you do that, ReserveInputConnection() will be called
		again with the input you specify.
		return B_OK to continue the connection process. */
	virtual	status_t				ReserveInputConnection(
										IMediaOutput::sptr_ref remoteOutput,
										BMediaInput::sptr * ioInput,
										BMediaConstraint * constraint,
										media_endpoint_state requestedState);

	/*!	base impl: no-op, return B_OK
		a format has been negotiated for the given connection.  if you need to allocate
		shared system or hardware resources, now is a good time to do so; if you return
		an error the connection will be cancelled.
		return B_OK to continue the connection process. */
	virtual	status_t				AcceptInputConnection(
										IMediaOutput::sptr_ref remoteOutput,
										BMediaInput::sptr_ref input,
										const BMediaFormat & format);

	/*!	base impl: no-op
		at this point the remote input and its node have accepted the given format.
		if you need to allocate shared system or hardware resources, now is a good time to do so;
		if you return an error the connection will be cancelled.
		return B_OK to continue the connection process. */
	virtual	status_t				AcceptOutputConnection(
										BMediaOutput::sptr_ref output,
										IMediaInput::sptr_ref remoteInput,
										const BMediaFormat & format);

	/*!	base impl: no-op
		the given reservation or connection failed. */
	virtual	void					AbortInputConnection(
										IMediaOutput::sptr_ref remoteOutput,
										BMediaInput::sptr_ref input,
										media_endpoint_state deniedState);

	/*!	base impl: no-op
		the given reservation or connection failed. */
	virtual	void					AbortOutputConnection(
										BMediaOutput::sptr_ref output,
										IMediaInput::sptr_ref remoteInput,
										media_endpoint_state deniedState);

	// message hooks
	
	/*!	base impl: propagate message along all connected endpoints in
		the direction specified */
	virtual status_t				PropagateMessage(
										const BMessage & message,
										IMediaEndpoint::sptr_ref from,
										media_endpoint_type direction,
										BMediaEndpointVector * outVisited);

	// ** BAtom

	virtual	status_t				Acquired(const void* id);
	virtual	status_t				Released(const void* id);

private:
	friend	class BMediaEndpoint;
	friend	class BMediaOutput;
	friend	class BMediaInput;

			class endpoint_link;
			class impl;
			impl *					i;
					
	virtual	status_t				SetParent(IMediaCollective::sptr_ref parent);

			int32					_IndexOfEndpoint(BMediaEndpoint::sptr_ref endpoint);
};

}; }; // namespace B::Media2
#endif //_MEDIA2_MEDIANODE_H_
