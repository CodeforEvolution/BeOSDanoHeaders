#ifndef _MEDIA2_MEDIANODE_INTERFACE_
#define _MEDIA2_MEDIANODE_INTERFACE_

#include <support2/IInterface.h>
#include <support2/IBinder.h>
#include <media2/MediaDefs.h>
#include <media2/MediaConstraint.h>

namespace B {
namespace Media2 {

using namespace Support2;

class IMediaNode : public IInterface
{
public:
	B_DECLARE_META_INTERFACE(MediaNode)

	virtual	BString					Name() const = 0;
	virtual	atom_ptr<IMediaCollective>	Parent() const = 0;

	/*!	Adds endpoints that match the given criteria to "outEndpoints"
		returns the number of new items in "outEndpoints" or an error */
	virtual	ssize_t					ListEndpoints(
										BMediaEndpointVector * outEndpoints,
										int32 type = B_ANY_ENDPOINT_TYPE,
										int32 state = B_ANY_ENDPOINT_STATE) const = 0;

	/*!	Lists endpoints of this node that are internally linked to the given
		endpoint (which must also belong to this node.)  An internal link
		between an input and an output indicates that the input and output
		will share a buffer source (ie. filter buffers in-place.) */
	virtual	ssize_t					ListLinkedEndpoints(
										const atom_ptr<IMediaEndpoint> & fromEndpoint,
										BMediaEndpointVector * outEndpoints,
										int32 state = B_ANY_ENDPOINT_STATE) const = 0;
	
	/*! Returns the first endpoint with the given name (and, optionally, a given
		endpoint type and connection state.)  If name is an empty string, then
		only the type and state will be matched. */
	virtual	status_t				FindEndpoint(
										const BString & name,
										atom_ptr<IMediaEndpoint> * outEndpoint,
										int32 type = B_ANY_ENDPOINT_TYPE,
										int32 state = B_ANY_ENDPOINT_STATE) const = 0;
	
	/*!	find an output by name */
			status_t				FindOutput(
										const BString & name,
										atom_ptr<IMediaEndpoint> * outEndpoint = 0,
										atom_ptr<IMediaOutput> * outOutput = 0,
										int32 state = B_ANY_ENDPOINT_STATE) const;

	/*!	find an input by name */
			status_t				FindInput(
										const BString & name,
										atom_ptr<IMediaEndpoint> * outEndpoint = 0,
										atom_ptr<IMediaInput> * outInput = 0,
										int32 state = B_ANY_ENDPOINT_STATE) const;
	
	/*!	find the first free output */
			status_t				FindFreeOutput(
										atom_ptr<IMediaEndpoint> * outEndpoint = 0,
										atom_ptr<IMediaOutput> * outOutput = 0);

	/*!	find the first free input */
			status_t				FindFreeInput(
										atom_ptr<IMediaEndpoint> * outEndpoint = 0,
										atom_ptr<IMediaInput> * outInput = 0);


	/*! [re-]allocates buffers via every BMediaProducerContext that
		intersects this node. */
	virtual	status_t				AllocateBuffers(
										const BMediaConstraint & constraint) = 0;
			status_t				AllocateBuffers();

private:
	friend class BMediaCollective;
	friend class LMediaNode;

	virtual	status_t				SetParent(const atom_ptr<IMediaCollective> & parent) = 0;
};

}; }; // namespace B::Media2
#endif //_MEDIA2_MEDIANODE_INTERFACE_
