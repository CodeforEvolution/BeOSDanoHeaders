/***************************************************************************
//
//	File:			media2/MediaEndpoint.h
//
//	Description:	Endpoints manage connections between nodes, buffer
//					acquisition, and buffer passing.
//
//					BMediaEndpoint: common endpoint functionality
//					BMediaOutput: buffer-producing endpoint
//					BMediaInput: buffer-consuming endpoint
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _MEDIA2_MEDIAENDPOINT_H_
#define _MEDIA2_MEDIAENDPOINT_H_

#include <support2/Binder.h>
#include <support2/Locker.h>
#include <support2/String.h>

#include <media2/BufferGroup.h>
#include <media2/IMediaEndpoint.h>
#include <media2/IMediaNode.h>
#include <media2/MediaConstraint.h>
#include <media2/MediaPreference.h>

namespace B {
namespace Private {
	class BufferSourceCache;
}
namespace Media2 {

using namespace Support2;

class BMediaNode;
class BMediaEndpoint;

class BMediaProducerContext;

class LMediaEndpoint : public LInterface<IMediaEndpoint>
{
	static const struct effect_action_def kActions[];

	static BValue hook_Name (IInterface::sptr_ref i);
	static BValue hook_Node (IInterface::sptr_ref i);
	static BValue hook_EndpointType (IInterface::sptr_ref i);
	static BValue hook_EndpointState (IInterface::sptr_ref i);
	static BValue hook_Partner (IInterface::sptr_ref i);
	static BValue hook_Constraint (IInterface::sptr_ref i);
	static BValue hook_Preference (IInterface::sptr_ref i);
	static BValue hook_Format (IInterface::sptr_ref i);
	static BValue hook_AcquireBuffer (IInterface::sptr_ref i, const BValue &);
	static BValue hook_PropagateMessage (IInterface::sptr_ref i, const BValue &v);
	static BValue hook_AllocateBuffers (IInterface::sptr_ref i, const BValue &);
	static BValue hook_BufferSourceConstraint (IInterface::sptr_ref i, const BValue &v);
	static BValue hook_MakeBufferSource (IInterface::sptr_ref i, const BValue &v);
	static BValue hook_AttachBufferSource (IInterface::sptr_ref i, const BValue &v);
	static BValue hook_DetachBufferSource (IInterface::sptr_ref i, const BValue &v);
	static BValue hook_CommitDependantTransaction (IInterface::sptr_ref i, const BValue &);
	static BValue hook_CancelDependantTransaction (IInterface::sptr_ref i, const BValue &);

	public:
							LMediaEndpoint(IContext::sptr_ref context);
							LMediaEndpoint();
		virtual	status_t	HandleEffect(const BValue & in,
										 const BValue & inBindings,
										 const BValue & outBindings,
										 BValue * out);
};

class BMediaEndpoint : public LMediaEndpoint
{
public:
	B_STANDARD_ATOM_TYPEDEFS(BMediaEndpoint)

	// ** local API

	virtual	status_t				SetName(const char * name);
	virtual	status_t				SetNode(const atom_ptr<BMediaNode> & node);
	
	// you can call SetConstraint() at any time, but it will only have a visible
	// effect if/once the endpoint is disconnected.  when an endpoint is reserved
	// or connected,  IMediaEndpoint::Constraint() will reflect that state.
	virtual	status_t				SetConstraint(const BMediaConstraint & constraint);
	
	virtual	status_t				SetPreference(const BMediaPreference & preference);
	
	// call AddDependantConstraint() from BMediaNode::AcceptXXXConnection()
	// or BMediaNode::ReserveXXXConnection() to register a dependancy
	// between this endpoint (which must currently be being connected) and 'target',
	// with the given constraint.  the call WILL FAIL if this node is not currently
	// being connected, if a dependant constraint on 'target' has already been
	// added for this endpoint, or if 'target' does not belong to the same node as
	// this endpoint.  the effects of this call vary depending on the current state
	// of 'target':
	
	// B_FREE_ENDPOINT: the constraint is And()'d with the target's current constraint
	// (the one reported by Constraint()).  when this endpoint is disconnected, the
	// constraint is removed.  yes, you can stack multiple dependant constraints
	// on a given target and disconnect the endpoints in any order.  if a Simplify() of the
	// combined constraint fails, an appropriate error is returned and the endpoint
	// is left unmolested.
	
	// B_RESERVED_ENDPOINT: triggers a connection to the reserved partner endpoint,
	// using the given constraint. returns B_OK if the connection succeeded, or an
	// appropriate error otherwise.
	
	// B_CONNECTED_ENDPOINT: returns the result of a Simplify() of the given constraint
	// And()'d against the connection format.  doesn't result in any state change.
	//
	// any changes in endpoint state resulting from this call will take effect if
	// and only if the original connection to/from this endpoint is successful.

	virtual	status_t				AddDependantConstraint(
										const atom_ptr<BMediaEndpoint> & target,
										const BMediaConstraint & constraint);
	
	// ** IMediaEndpoint
	
	virtual	BString					Name() const;
	virtual	IMediaNode::sptr			Node() const;
	virtual media_endpoint_type		EndpointType() const;
	virtual	media_endpoint_state	EndpointState() const;
	virtual	IMediaEndpoint::sptr		Partner() const;
	virtual	BMediaConstraint		Constraint() const;
	virtual	BMediaPreference		Preference() const;
	virtual	BMediaFormat			Format() const;

	virtual	status_t				AllocateBuffers(
										const BMediaConstraint & constraint);
			status_t				AllocateBuffers();

	// default impl: hand off to BMediaNode::BufferSourceConstraint()
	virtual	BMediaConstraint		BufferSourceConstraint() const;
	
protected:

	// ** IMediaEndpoint
	
	// default impl: hand off to BMediaNode::MakeBufferSource()
	virtual	IBufferSource::sptr		MakeBufferSource(
										size_t minBufferCount,
										size_t minBufferCapacity,
										uint32 lock);

	virtual	void					AttachBufferSource(
										IBufferSource::sptr_ref source);

	virtual	void					DetachBufferSource(
										IBufferSource::sptr_ref source);

	// ** hooks
	
	// default impl: no-op
	virtual	void					AboutToConnect(
										IMediaEndpoint::sptr_ref remoteEndpoint,
										const BMediaFormat & format);
	
	// default impl: no-op
	virtual	void					Connected(
										IMediaEndpoint::sptr_ref remoteEndpoint,
										const BMediaFormat & format);

	// default impl: no-op
	virtual	void					Disconnected(
										IMediaEndpoint::sptr_ref remoteEndpoint);


	// implementation

	virtual							~BMediaEndpoint();
	
	virtual	void					AttachedToNode(const atom_ptr<BMediaNode> & node);
	virtual	void					DetachedFromNode(const atom_ptr<BMediaNode> & node);

	virtual	status_t				Acquired(const void* id);
	virtual	status_t				Released(const void* id);

	virtual void					CommitDependantTransaction();
	virtual void					CancelDependantTransaction();

private:
	friend	class BMediaNode;
	friend	class BMediaInput;
	friend	class BMediaOutput;

									BMediaEndpoint(IContext::sptr_ref context,
												   media_endpoint_type type);
									BMediaEndpoint(media_endpoint_type type);

			status_t				BeginConnect(
										media_endpoint_state requestedState,
										BMediaConstraint * outConstraint = 0);
			void					SetReserved(
										IMediaEndpoint::sptr_ref partner,
										const BMediaConstraint & reservation);
			void					SetConnected(
										IMediaEndpoint::sptr_ref partner,
										const BMediaFormat & format,
										bool asDependant = false);
			void					SetDisconnected();
			void					EndConnect(
										bool succeeded = false);
			
			status_t				BeginDependantTransaction(
										const atom_ptr<BMediaEndpoint> & source,
										const BMediaConstraint & constraint);
			status_t				BeginDependantConnect(
										BMediaConstraint * outConstraint = 0);
			void					EndDependantConnect();
			atom_ptr<BMediaNode> 	LocalNode() const;
			
			::B::Private::BufferSourceCache &	BufferSourceCache() const;
						
			BMediaConstraint		_MergeExternalConstraints(const BMediaConstraint & internal);
			BMediaConstraint		_Constraint() const;

	class	impl;
			impl * i;
	friend class B::Media2::BMediaEndpoint::impl;
};

/**************************************************************************************/

class LMediaOutput : public LInterface<IMediaOutput>
{
	B_STANDARD_ATOM_TYPEDEFS(LMediaOutput);

		static	const struct effect_action_def kActions[];

		static	BValue		hook_Reserve(IInterface::sptr_ref i, const BValue & v);
		static	BValue		hook_Connect(IInterface::sptr_ref i, const BValue & v);
		static	BValue		hook_Disconnect(IInterface::sptr_ref i, const BValue &);
		static	BValue		hook_AcceptFormat(IInterface::sptr_ref i, const BValue &v);
		static	BValue		hook_DependantConnect(IInterface::sptr_ref i, const BValue &v);

	public:
		virtual	status_t	HandleEffect(const BValue & in,
										 const BValue & inBindings,
										 const BValue & outBindings,
										 BValue * out);
};

class BMediaOutput : public BMediaEndpoint, public LMediaOutput
{
public:
	B_STANDARD_ATOM_TYPEDEFS(BMediaOutput)

							BMediaOutput(IContext::sptr_ref context, const char * name = 0);
							BMediaOutput(const char * name = 0);

	/*!	fetch a buffer to be passed downstream.  this method is only guaranteed to work
		for an output at the head of a processing chain for a given buffer source -- it
		must not be internally linked to an input. */
	virtual	int32			AcquireBuffer(
								BBuffer * outBuffer = 0,
								int32 id = BBuffer::ANY_BUFFER,
								bigtime_t timeout = B_INFINITE_TIMEOUT);

	virtual	status_t		SendBuffer(BBuffer * buffer);

	// ** IMediaOutput

	virtual	status_t		Reserve(IMediaInput::sptr * ioInput, const BMediaConstraint & constraint);
			status_t		Reserve(IMediaInput::sptr * ioInput);
	virtual	status_t		Connect(IMediaInput::sptr * ioInput, BMediaFormat * outFormat,
									const BMediaConstraint & constraint);
			status_t		Connect(IMediaInput::sptr * ioInput, BMediaFormat * outFormat);
	virtual	status_t		Disconnect();
	
	// ** IMediaEndpoint

	virtual	void			AttachBufferSource(
								IBufferSource::sptr_ref source);

	virtual	void			DetachBufferSource(
								IBufferSource::sptr_ref source);

	/*!	propagate a message along this arc until the whole chain
		is visited or some endpoint along the chain returns an error. */
	virtual	status_t		PropagateMessage(
								const BMessage & message,
								media_endpoint_type direction,
								BMediaEndpointVector * visited);

	virtual	BValue			Inspect(const BValue &which, uint32 flags = 0);

protected:
	virtual					~BMediaOutput();
	
	virtual	status_t		Acquired(const void* id);
	virtual	status_t		Released(const void* id);

	virtual	status_t		AcceptFormat(IMediaInput::sptr_ref input, const BMediaFormat & format);

	virtual	status_t		DependantConnect(const BMediaConstraint & constraint);

	virtual void			AbortDependantConnect();

private:
	class	impl;
			impl * i;

			status_t send_local(BBuffer * buffer);
			status_t send_remote(BBuffer * buffer);

			status_t acquire_local(BBuffer * out_buffer, int32 requested_id, bigtime_t timeout);
			status_t acquire_remote(BBuffer * out_buffer, int32 requested_id, bigtime_t timeout);

	friend	class BMediaNode;

			status_t share_producer_context(BMediaOutput::sptr_ref with_output);
};

/**************************************************************************************/

class LMediaInput : public LInterface<IMediaInput>
{
		static	const struct effect_action_def kActions[];

		static	BValue		hook_HandleBuffer(IInterface::sptr_ref i, const BValue & v);
		static	BValue		hook_AcceptReserve(IInterface::sptr_ref i, const BValue & v);
		static	BValue		hook_AcceptConnect(IInterface::sptr_ref i, const BValue & v);
		static	BValue		hook_AcceptDependantConnect(IInterface::sptr_ref i, const BValue & v);
		static	BValue		hook_AcceptDisconnect(IInterface::sptr_ref i);

				BMediaInput *	mInput;

	public:
		B_STANDARD_ATOM_TYPEDEFS(LMediaInput);

							LMediaInput(BMediaInput * input);
		virtual	status_t	HandleEffect(const BValue & in,
										 const BValue & inBindings,
										 const BValue & outBindings,
										 BValue * out);
		virtual	status_t	Transact(uint32 code, BParcel & data,
									 BParcel * reply = 0, uint32 flags = 0);

		virtual	status_t	HandleBuffer(BBuffer * buffer) = 0;
};

class BMediaInput : public BMediaEndpoint, public LMediaInput
{
public:
	B_STANDARD_ATOM_TYPEDEFS(BMediaInput)

								BMediaInput(IContext::sptr_ref context, const char * name = 0);
								BMediaInput(const char * name = 0);
	
	// ** IMediaInput

	/*! consume or filter the given buffer.  if you don't retransmit the buffer,
		be sure to call ReleaseBuffer() on it.
		default implementation: hand off to BMediaNode::HandleBuffer(). */
	virtual	status_t			HandleBuffer(BBuffer * buffer);

	// propagate a message along this arc until the whole chain
	// is visited or some endpoint along the chain returns an error
	virtual	status_t			PropagateMessage(
									const BMessage & message,
									media_endpoint_type direction,
									BMediaEndpointVector * visited);

	virtual	BValue				Inspect(const BValue &which, uint32 flags = 0);

protected:
	virtual						~BMediaInput();

	virtual	status_t			Acquired(const void* id);
	virtual	status_t			Released(const void* id);

	virtual	status_t			AcceptReserve(
									IMediaEndpoint::sptr_ref sourceEndpoint,
									IMediaOutput::sptr_ref sourceOutput,
									const BMediaConstraint & constraint,
									IMediaEndpoint::sptr * outReservedEndpoint,
									IMediaInput::sptr * outReservedInput);

	virtual	status_t			AcceptConnect(
									IMediaEndpoint::sptr_ref sourceEndpoint,
									IMediaOutput::sptr_ref sourceOutput,
									const BMediaConstraint & constraint,
									const BMediaPreference & sourcePreference,
									IMediaEndpoint::sptr * outConnectedEndpoint,
									IMediaInput::sptr * outConnectedInput,
									BMediaFormat * outFormat);

	virtual	status_t			AcceptDependantConnect(
									const BMediaConstraint & constraint,
									const BMediaPreference & sourcePreference,
									BMediaFormat * outFormat);

	virtual	status_t			AcceptDisconnect();

private:
	friend	class LMediaInput;
	
			status_t			DeliverBuffer(int32 id, area_id area, size_t offset,
											  size_t info_size, const void * info);
			status_t			DeliverBuffer(int32 id, area_id area, size_t offset,
											  size_t range_offset, size_t range_size,
											  size_t info_size, const void * info);
};

}; }; // namespace B::Media2
#endif //_MEDIA2_MEDIAENDPOINT_H_
