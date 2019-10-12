/***************************************************************************
//
//	File:			render2/Update.h
//
//	Description:	An object to describe an update in terms of pixels to
//					move and pixels to invalidate
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _RENDER2_UPDATE_RESOLVER_H_
#define _RENDER2_UPDATE_RESOLVER_H_

#include <render2/Point.h>
#include <render2/Rect.h>
#include <render2/Region.h>
#include <render2/2dTransform.h>
#include <support2/Flattenable.h>
#include <support2/Vector.h>

namespace B {
namespace Render2 {

using namespace B::Support2;


/*!
	BUpdate describes an update as a list of regions to be moved plus
	one \e dirty region (to be updated).
	BUpdates can be composed togather, either in space or in time.
	When BUpdates are composed, the composition tries to maximize the
	number of regions to be moved and minimizes the dirty region.

	\note BUpdate compositions can't handle complex
	2D transformations; if a region of the screen is rotated,
	scaled or skewed, the resulting composition is a plain dirty region.

	\sa IViewParent::InvalidateChild()
*/

class BUpdate : public BFlattenable
{
public:	

	//! Hint flags used in BUpdate constructor
	enum {
		B_OPAQUE			= 0x00000000,		//!< Opaque surface
		B_TRANSLUCENT		= 0x00000001,		//!< Translucent surface
		B_UPDATE_ON_RESIZE	= 0x00000004,		//!< Update when surface resized
		B_ALWAYS_UPDATE		= 0x00000008		//!< This surface will always be updated
	};

	//! Description of a region that needs to be moved
	struct record {
		record() { };
		record(const BRegion& r, const BPoint& p)
			: offset(p), region(r) { };
		BPoint		offset;
		BRegion		region;
	};

	//! The empty update does nothing, but is usefull to compose with.
	static const BUpdate empty;


	////////////////////////////////////////////////////////
	// Constructors

		BUpdate();
		BUpdate(const BValue&);
		BUpdate(const value_ref& value);

		/*! Use this constructor if the region moved or changed shape.
			\param flags:		Hints about the region type. This will affect how
								the update is described.
			\param t0:			2D Transformation of the region before the update.
			\param t1:			2D Transformation of the region after the update.
			\praam r0:			Region's shape \e before the update. It is stated in
								\e t0 coordinate space.
			\praam r1:			Region's shape \e after the update. It is stated in
								\e t1 coordinate space.
		*/
	
		BUpdate(const uint32 flags,
				const B2dTransform& t0, const BRegion& r0,
				const B2dTransform& t1, const BRegion& r1);

		/*! Use this constructor if the region just needs to be updated
			\param flags:		Hints about the region type. This will affect how
								the update is described.
			\param t0:			2D Transformation of the region before the update.
			\praam shape:		Region's shape \e before the update. It is stated in
								\e t0 coordinate space.
			\praam invalidate:	Region to invalidate stated in \e t0 coordinate space.
		*/

		BUpdate(const uint32 flags,
				const B2dTransform& t0, const BRegion& shape,
				const BRegion& invalidate = BRegion::empty);


	BValue AsValue() const;
	
	//! Clears the BUpdate.
	void MakeEmpty();
	
	//! Apply a 2D transformation to this BUpdate. You normaly don't use this call.
	void Transform(const B2dTransform& t);

	//! Return \e true if this BUpdate is equivalent to BUpdate::empty
	const bool IsEmpty() const;

	//! Returns the number of region to be moved in this update describtion
	const int32 CountRegions() const { return m_movedRegions.CountItems(); }
	
	//! Returns an array to the region to be moved
	const BVector<record>& MovedRegions() const { return m_movedRegions; }
	
	//! Returns the dirty region
	const BRegion& DirtyRegion() const { return m_dirtyRegion; }

	////////////////////////////////////////////////////////
	// Compose update descriptions

	/*! Composes two BUpdate togather spatially.
		Both updates must represent regions of the screen
		whose \e t0 2D transformations are stated in the
		same coordinate space. In other words, they must
		be siblings.
	*/
	//{@
	BUpdate& ComposeInFront(const BUpdate& back) {
		const BUpdate& front = *this;
		*this = compose(front, back);
		return *this;
	}

	BUpdate& ComposeBehind(const BUpdate& front) {
		const BUpdate& back = *this;
		*this = compose(front, back);
		return *this;
	}
	//@}
	
	//! Compose a parent with a desription of all its children
	BUpdate& ComposeWithChildren(const BUpdate& children);

	/*! Composes two BUpdate togather in time.
		Both updates must represent regions of the screen
		whose \e t0 2D transformations are stated in the
		same coordinate space. In other words, they must
		be siblings.
	*/
	//{@
	BUpdate& ComposeAfter(const BUpdate& first) {
		const BUpdate& next = *this;
		*this = compose_in_time(next, first);
		return *this;
	}

	BUpdate& ComposeBefore(const BUpdate& next) {
		const BUpdate& first = *this;
		*this = compose_in_time(next, first);
		return *this;
	}
	//@}
	

	////////////////////////////////////////////////////////
	// Implement BFlattenable API.

	//! BUpdate implement BFlattenable API. \sa BFlattenable
	//{@
	virtual	bool		IsFixedSize() const;
	virtual	type_code	TypeCode() const;
	virtual	ssize_t		FlattenedSize() const;
	virtual	status_t	Flatten(void *buffer, ssize_t size) const;
	virtual	bool		AllowsTypeCode(type_code code) const;
	virtual	status_t	Unflatten(type_code c, const void *buf, ssize_t size);
	//@}

private:
	static BUpdate compose(const BUpdate& front, const BUpdate& back);
	static BUpdate compose_in_time(const BUpdate& next, const BUpdate& first);
	record& operator [] (int32 index) {	return m_movedRegions.EditItemAt(index); }
	void add_record(const record& r);
	struct reg_sizes_t {
		ssize_t clean;
		ssize_t trans;
		ssize_t dirty;
		ssize_t nshap;
	};
	BVector<record>		m_movedRegions;	// array of region to move
	BRegion				m_cleanRegion;	// region covered by all visited views
	BRegion				m_translucent;	// translucent region of the surface
	BRegion				m_dirtyRegion;	// region to send an update for
	BRegion				m_newRegion;	// final shape of the view
	B2dTransform		m_srcTransform;
	B2dTransform		m_dstTransform;
};

} } // namespace B::Render2

#endif 	/* _RENDER2_UPDATE_RESOLVER_H_ */

