#ifndef _MEDIA2_MEDIAENDPOINT_INTERFACE_
#define _MEDIA2_MEDIAENDPOINT_INTERFACE_

#include <support2/IInterface.h>
#include <support2/IBinder.h>
#include <support2/Message.h>

#include <media2/MediaDefs.h>
#include <media2/IBufferSource.h>

namespace B {
namespace Media2 {

using namespace Support2;

class BBuffer;
class BMediaConstraint;
class BMediaPreference;
class BMediaFormat;

class IMediaEndpoint : public IInterface
{
public:
	B_DECLARE_META_INTERFACE(MediaEndpoint)

	//! linkables
	static	const BValue	attached_to_node;	// arg_endpoint->this, arg_node->node
	static	const BValue	detached_from_node;	// arg_endpoint->this
	static	const BValue	allocate_buffers;	// arg_endpoint->this, arg_constraint->constraint (as value)
	static	const BValue	connected;			// arg_output->endpoint, arg_input->endpoint
	static	const BValue	disconnected;		// arg_output->endpoint, arg_input->endpoint
	
	static	const BValue	arg_endpoint;
	static	const BValue	arg_output;
	static	const BValue	arg_input;
	static	const BValue	arg_node;
	static	const BValue	arg_constraint;

	virtual	BString					Name() const = 0;
	
	//! owning node (guaranteed to be in the same team)
	virtual	atom_ptr<IMediaNode>	Node() const = 0;
	
	//! input or output?
	virtual media_endpoint_type		EndpointType() const = 0;
	
	//! free, reserved, or connected?
	virtual	media_endpoint_state	EndpointState() const = 0;

	/*! if reserved or connected, return the endpoint on
		the other end of the connection */
	virtual	atom_ptr<IMediaEndpoint>	Partner() const = 0;

	/*! format-negotiation rules for this endpoint
		(valid at any time, used during reservation/connection) */
	virtual	BMediaConstraint		Constraint() const = 0;
	
	/*!	preferred format values for this endpoint
		(valid at any time, used during connection) */
	virtual	BMediaPreference		Preference() const = 0;
	
	/*!	resolved format (invalid unless the endpoint is connected) */
	virtual	BMediaFormat			Format() const = 0;
	
	/*!	propagate a message along this arc until the whole chain
		is visited or some endpoint along the chain returns an error */
	virtual	status_t				PropagateMessage(
										const BMessage & message,
										media_endpoint_type direction,
										BMediaEndpointVector * visited) = 0;

	/*!	triggers buffer allocation for every BMediaProducerContext that intersects
		this endpoint. */
	virtual	status_t				AllocateBuffers(
										const BMediaConstraint & constraint) = 0;
			status_t				AllocateBuffers();
	
	virtual	BMediaConstraint		BufferSourceConstraint() const = 0;

protected:

	/*!	this will be called during the buffer allocation process.
		feel free to return 0 if you don't need to allocate buffers yourself.
		the 'lock' value should be passed to create_area (if you can't honor
		it, return 0.) */
	virtual	IBufferSource::sptr		MakeBufferSource(
										size_t bufferCount,
										size_t bufferSize,
										uint32 lock) = 0;
	
	/*! when the buffer allocation process succeeds, AttachBufferSource() will be
		called on every participating endpoint. */
	virtual	void					AttachBufferSource(
										IBufferSource::sptr_ref source) = 0;
	
	/*!	when a buffer source is freed, DetachBufferSource() will be called on
		every applicable endpoint, starting at the producer and proceding downstream. */
	virtual	void					DetachBufferSource(
										IBufferSource::sptr_ref source) = 0;

	friend	class BMediaProducerContext;
	friend	class BMediaEndpoint;
	friend	class LMediaEndpoint;

	virtual void					CommitDependantTransaction() = 0;
	virtual void					CancelDependantTransaction() = 0;
};

/**************************************************************************************/

class IMediaOutput : public IInterface
{
public:
	B_DECLARE_META_INTERFACE(MediaOutput)

	/*!	reserves a connection to the given input (for which another input may be
		substituted at the other node's discretion.)  this call will fail if this
		output or the given input is already reserved or connected.
		** you don't need to call Reserve() before Connect().  this call is
		useful if you need to atomically make multiple connections connections,
		or if you need more than one stage of format negotiation. */

	virtual	status_t	Reserve(atom_ptr<IMediaInput> * ioInput,
								const BMediaConstraint & constraint) = 0;

			status_t	Reserve(atom_ptr<IMediaInput> * ioInput);

	/*!	completes a connection to the given input (for which another input may be
		substituted, unless you've already reserved a connection to this one.)  this
		call will fail if this output or the given input are otherwise reserved, or
		are already connected. */
	virtual	status_t	Connect(atom_ptr<IMediaInput> * ioInput,
								BMediaFormat * outFormat,
								const BMediaConstraint & constraint) = 0;

			status_t	Connect(atom_ptr<IMediaInput> * ioInput,
								BMediaFormat * outFormat);

	//! break the current connection.
	virtual	status_t	Disconnect() = 0;

protected:
	friend class BMediaEndpoint;
	friend class BMediaInput;
	friend class LMediaOutput;

	virtual	status_t	AcceptFormat(const atom_ptr<IMediaInput> & input, const BMediaFormat & format) = 0;

	virtual	status_t	DependantConnect(const BMediaConstraint & constraint) = 0;
};

/**************************************************************************************/

class IMediaInput : public IInterface
{
public:
	B_DECLARE_META_INTERFACE(MediaInput)

protected:
	friend class BMediaOutput;
	friend class LMediaInput;

	virtual	status_t	AcceptReserve(
							const atom_ptr<IMediaEndpoint> & sourceEndpoint,
							const atom_ptr<IMediaOutput> & sourceOutput,
							const BMediaConstraint & constraint,
							atom_ptr<IMediaEndpoint> * outReservedEndpoint,
							atom_ptr<IMediaInput> * outReservedInput) = 0;

	virtual	status_t	AcceptConnect(
							const atom_ptr<IMediaEndpoint> & sourceEndpoint,
							const atom_ptr<IMediaOutput> & sourceOutput,
							const BMediaConstraint & constraint,
							const BMediaPreference & sourcePreference,
							atom_ptr<IMediaEndpoint> * outConnectedEndpoint,
							atom_ptr<IMediaInput> * outConnectedInput,
							BMediaFormat * outFormat) = 0;

	virtual	status_t	AcceptDependantConnect(
							const BMediaConstraint & constraint,
							const BMediaPreference & sourcePreference,
							BMediaFormat * outFormat) = 0;

	virtual	status_t	AcceptDisconnect() = 0;	
};

}; }; // namespace B::Media2
#endif //_MEDIA2_MEDIAENDPOINT_INTERFACE_
