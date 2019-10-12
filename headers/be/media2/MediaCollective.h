/***************************************************************************
//
//	File:			media2/MediaCollective.h
//
//	Description:	Presents a group of BMediaNodes as a single node,
//					exposing selected endpoints.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _MEDIA2_MEDIACOLLECTIVE_H_
#define _MEDIA2_MEDIACOLLECTIVE_H_

#include <support2/String.h>

#include <media2/MediaNode.h>
#include <media2/MediaDefs.h>

#include <media2/IMediaCollective.h>

namespace B {
namespace Media2 {

using namespace ::B::Support2;

class LMediaCollective : public LInterface<IMediaCollective>
{
		static	const struct effect_action_def kActions[];
		static	BValue		hook_ListNodes(IInterface::sptr_ref i);
	protected:
							LMediaCollective(const IContext::sptr& context);
							LMediaCollective();
		virtual				~LMediaCollective();
		
		virtual	status_t	HandleEffect(const BValue & in,
										 const BValue & inBindings,
										 const BValue & outBindings,
										 BValue * out);
};

class BMediaCollective : public LMediaCollective, public LMediaNode
{
public:
	B_STANDARD_ATOM_TYPEDEFS(BMediaCollective)

									BMediaCollective(const IContext::sptr& context, const char * name = 0);
									BMediaCollective(const char * name = 0);

	virtual	BValue					Inspect(const BValue &which, uint32 flags = 0);
	inline	IContext::sptr			Context() const { return LMediaCollective::Context(); }

			void					SetName(const char * name);

	virtual	status_t				AddNode(IMediaNode::sptr_ref node);
	virtual	status_t				RemoveNode(IMediaNode::sptr_ref node);
			IMediaNode::sptr			NodeAt(int32 index) const;
			int32					IndexOf(IMediaNode::sptr_ref node);
			int32					CountNodes() const;

	// you can only expose endpoints belonging to current child nodes
	virtual	status_t				ShowEndpoint(IMediaEndpoint::sptr_ref endpoint);
	virtual	status_t				HideEndpoint(IMediaEndpoint::sptr_ref endpoint);
	virtual	bool					IsVisible(IMediaEndpoint::sptr_ref endpoint);

	// ** IMediaCollective

	virtual	ssize_t					ListNodes(BMediaNodeVector * outNodes) const;
	
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

	virtual status_t				AllocateBuffers(
										const BMediaConstraint & constraint);
	
protected:
	virtual							~BMediaCollective();

	virtual	status_t				Acquired(const void* id);
	virtual	status_t				Released(const void* id);

private:

	virtual	status_t				SetParent(IMediaCollective::sptr_ref parent);

			int32					_FindNode(IMediaNode::sptr_ref node);
			int32					_FindVisible(IMediaEndpoint::sptr_ref endpoint);

	mutable	BLocker					mLock;
			BString					mName;
			IMediaCollective::wptr	mParent;
			BMediaNodeVector		mNodes;
			BMediaEndpointVector	mVisible;
};

}; }; // namespace B::Media2
#endif //_MEDIA2_MEDIACOLLECTIVE_H_
