/***************************************************************************
//
//	File:			render2/IPath.h
//
//	Description:	Path definition interface
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _RENDER2_PATH_H_
#define _RENDER2_PATH_H_

#include <math.h>
#include <support2/SupportDefs.h>
#include <support2/IInterface.h>
#include <render2/Point.h>

namespace B {
namespace Render2 {


using namespace Support2;

/**************************************************************************************/
//! A few usefull angles for Arc(), use them for the arclen whenever possible

extern const float B_PI_4;		//!<	   pi/4		 45 deg
extern const float B_PI_2;		//!<	   pi/2		 90 deg
extern const float B_3_PI_4;	//!<	 3*pi/4		135 deg
extern const float B_PI;		//!<	   pi		180 deg
extern const float B_5_PI_4;	//!<	 5*pi/4		225 deg
extern const float B_3_PI_2;	//!<	 3*pi/2		270 deg
extern const float B_7_PI_4;	//!<	 7*pi/4		315 deg
extern const float B_2_PI;		//!<	 2*pi		360 deg

/**************************************************************************************/

class IPath : public IInterface
{
public:

	B_DECLARE_META_INTERFACE(Path);

	/*!	MoveTo(), LineTo(), BezierTo(), ArcTo() and Close() have direct analogs in Postscript.
		Arc() explicitly defines an arc via centerpoint, radius and start and stop angle
			("connected" determines if the arc is started with a LineTo()(true) or MoveTo()(false))

		ArcTo() equivalent to postscript 'arct'.
		appends an arc of a circle to the current path, possibly preceded by a straight line segment.
		The arc is defined by a radius and two tangent lines. The tangent lines are those drawn from
		the current point here called p0, to p1, and from p1 to p2.
		The center of the arc is located within the inner angle between the tangent lines. It is the
		only point located at distance radius in a direction perpendicular to both lines.
		The arc begins at the first tangent point pt1 on the first tangent line, passes between its
		center and the point p1, and ends at the second tangent point pt2 on the second tangent line.
		
		If radius is set to -1, the radius will be automatically computed so that the pt1=p0 and pt2=p2,
		this is possible only if |p1,p0| == |p1,p2|, which ArcsTo() assumes to be true.
	*/

	virtual	void		MoveTo(const BPoint& pt) = 0;
	virtual	void		LinesTo(const BPoint* points, int32 lineCount=1) = 0;
	virtual	void		BeziersTo(const BPoint* points, int32 bezierCount=1) = 0;
	virtual	void		ArcsTo(const BPoint* points, const coord* radius, int32 arcCount=1) = 0;
	virtual	void		Close() = 0;


	//! Convenience functions. You don't need to implement those.
	virtual	void		Arc(const BPoint& center, const BPoint& radii, float startAngle=0, float arcLen=B_2_PI, bool connected=false);
	virtual	void		Line(const BPoint &p0, const BPoint &p1);
	virtual	void		Rect(const BRect &r);
	virtual	void		RoundRect(const BRect &r, const BPoint& radii);

	//! Convenience functions that are implemented on top of the virtual calls.
			void		MoveTo(coord x, coord y);

			void		LineTo(const BPoint& endpoint);
			void		LineTo(coord x, coord y);

			void		BezierTo(const BPoint& pt1, const BPoint& pt2, const BPoint& endpoint);
			void		BezierTo(coord x1, coord y1, coord x2, coord y2, coord x3, coord y3);

			void		ArcTo(const BPoint& p1, const BPoint& p2, coord radius);
			void		ArcTo(coord x1, coord y1, coord x2, coord y2, coord radius);
	
			//! Arc() is implemented with beziers
			void		Arc(const BPoint& center, coord radius, float startAngle=0, float arcLen=B_2_PI, bool connected=false);
			void		Arc(coord centerX, coord centerY, coord radius, float startAngle=0, float arcLen=B_2_PI, bool connected=false);
			void		Arc(coord centerX, coord centerY, coord radx, coord rady, float startAngle=0, float arcLen=B_2_PI, bool connected=false);
			void		Arc(const BRect& bounds, float startAngle=0, float arcLen=B_2_PI, bool connected=false);



protected:
	/*! You can use the following calls in your implementation
		of IPath as conveniences.
	*/
			void		ArcsTo2Beziers(	const BPoint& p0,			//! current pen position (p0)
										const BPoint& p1,			//! p1,p0 defines the first tangent
										const BPoint& p2,			//! p1,p2 defines the 2nd tangent
										coord radius);				//! radius
};

/**************************************************************************************/

} } // namespace B::Render2

#endif
