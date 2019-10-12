/***************************************************************************
//
//	File:			media2/MediaEndpointGraph.h
//
//	Description:	Represents a connected set of endpoints as an
//					undirected graph.  Provides access to those
//					endpoints in various orders.
//					NOT A VIRTUAL CLASS; NOT THREAD-SAFE.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _MEDIA2_MEDIAENDPOINTGRAPH_H_
#define _MEDIA2_MEDIAENDPOINTGRAPH_H_

#include <media2/IMediaEndpoint.h>

namespace B {

namespace Media2 {

using namespace B::Support2;

class BMediaEndpointGraph
{
public:
	enum visit_order_t
	{
		UNORDERED, // will list all endpoints, even those unreachable from the root
		BREADTH_FIRST,
		DEPTH_FIRST
	};

	typedef	void (*acquire_hook)(IMediaEndpoint::sptr_ref endpoint, void * context);
	typedef	void (*release_hook)(IMediaEndpoint::sptr_ref endpoint, void * context);
	
	typedef status_t (*visit_hook)	(IMediaEndpoint::sptr_ref endpoint, void * context);

						BMediaEndpointGraph();
						BMediaEndpointGraph(const BMediaEndpointGraph & clone);
						~BMediaEndpointGraph();
								
	BMediaEndpointGraph & operator=(const BMediaEndpointGraph & clone);

	// returns the root endpoint, or 0 if the graph is empty or has no root.
	IMediaEndpoint::sptr	Root() const;

	// sets the root endpoint of the graph, which is required for BREADTH_FIRST
	// and DEPTH_FIRST iteration via the Visit() method.
	// the root endpoint will not be implicitly removed from the graph, so
	// an primary reference to the root endpoint will be held until you SetRoot(0)
	// or destroy the graph.
	status_t			SetRoot(IMediaEndpoint::sptr_ref endpoint);

	// describe an output->input connection
	// may result in calls to the acquire hook
	status_t			AddConnection(IMediaOutput::sptr_ref output, IMediaInput::sptr_ref input);
	
	// may result in calls to the release hook
	status_t			RemoveConnection(IMediaOutput::sptr_ref output, IMediaInput::sptr_ref input);

	// describe a node-internal connection (the endpoints must belong to the same node.)
	// may result in calls to the acquire hook
	status_t			AddLink(IMediaEndpoint::sptr_ref e1, IMediaEndpoint::sptr_ref e2);

	// may result in calls to the release hook
	status_t			RemoveLink(IMediaEndpoint::sptr_ref e1, IMediaEndpoint::sptr_ref e2);

	// provide a hook to be called when an input or output is added to the graph.
	// the BMediaEndpointGraph will be locked when your hook is called.
	status_t			SetAcquireHook(acquire_hook hook,
									   void * context);

	// provide a hook to be called when an input or output is removed from the graph.
	// the BMediaEndpointGraph will be locked when your hook is called.
	status_t			SetReleaseHook(release_hook hook,
									   void * context);

	// call the given hook for each endpoint reachable from the root.
	// the BMediaEndpointGraph will be locked when your hook is called.
	// if your hook returns an error, the traversal will halt immediately
	// and the error code will be returned.  if the traversal is successful,
	// returns the number of endpoints visited.
	ssize_t				Visit(visit_hook hook,
							  void * context,
							  visit_order_t order = BREADTH_FIRST);

	status_t			ListEndpoints(BMediaEndpointVector * outEndpoints,
									  int32 type = B_ANY_ENDPOINT_TYPE,
									  int32 state = B_ANY_ENDPOINT_STATE,
									  visit_order_t order = BREADTH_FIRST);
	
	// remove the given endpoints (and any edges that touch it)
	status_t			RemoveEndpoint(IMediaEndpoint::sptr_ref endpoint);
	
	// remove endpoints that are unreachable from the root
	status_t			PruneUnreachable();
	
	// tabula rasa
	void				MakeEmpty();

private:
	class	impl;
			impl *		i;
			
	class	list_endpoints_state;
			
	static	status_t	do_list_endpoints(IMediaEndpoint::sptr_ref endpoint, void * context);
	
			ssize_t		visit_unordered(visit_hook hook, void * context);
			ssize_t		visit_breadth_first(visit_hook hook, void * context);
			ssize_t		visit_depth_first(visit_hook hook, void * context);
			
			impl *		clone() const;
};

} } // B::Media2
#endif //_MEDIA2_MEDIAENDPOINTGRAPH_H_
