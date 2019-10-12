/*******************************************************************************
/
/	File:			render2/Point.h
/
/   Description:    BPoint represents a single x,y coordinate.
/
/	Copyright 1993-98, Be Incorporated, All Rights Reserved
/
*******************************************************************************/

#ifndef	_RENDER2_POINT_H
#define	_RENDER2_POINT_H

#include <support2/ITextStream.h>
#include <support2/SupportDefs.h>
#include <support2/TypeFuncs.h>
#include <support2/Value.h>

namespace B {
namespace Render2 {
using namespace Support2;

class BRect;

/*----------------------------------------------------------------*/
/*----- BPoint class --------------------------------------------*/

class BPoint {

public:
		coord x;
		coord y;

		BPoint();
		BPoint(coord X, coord Y);
		BPoint(const BPoint& pt);
		BPoint(const BValue& o, status_t* result = NULL);
		BPoint(const value_ref& o, status_t* result = NULL);
		BPoint(const BPoint& refpt, coord radius, coord angle);
		
		BPoint		&operator=(const BPoint &from);
		void		Set(coord X, coord Y);

		BValue		AsValue() const;
inline				operator BValue() const					{ return AsValue(); }
		
		void		ConstrainTo(const BRect& rect);

		coord		Angle() const; //Angle from BPoint(0,0);
		coord		Angle(const BPoint& refpoint) const; //Angle from refpoint 
		
		coord		Radius() const; //Distance from BPoint(0,0);
		coord		Radius(const BPoint& refpoint) const; //Distance from refpoint

		void		PrintToStream(ITextOutput::arg io, uint32 flags=0) const;
static status_t		printer(ITextOutput::arg io, const value_ref& val, uint32 flags);
		
		BPoint		operator-() const;
		BPoint		operator+(const BPoint&) const;
		BPoint		operator-(const BPoint&) const;
		BPoint&		operator+=(const BPoint&);
		BPoint&		operator-=(const BPoint&);
		BPoint		operator*(const coord f) const;
		BPoint		operator/(const coord f) const;
		BPoint&		operator*=(const coord f);
		BPoint&		operator/=(const coord f);

		int32		Compare(const BPoint& p) const;
		bool		operator!=(const BPoint&) const;
		bool		operator==(const BPoint&) const;
		bool		operator<(const BPoint&) const;
		bool		operator<=(const BPoint&) const;
		bool		operator>=(const BPoint&) const;
		bool		operator>(const BPoint&) const;

private:
		void		Construct(const value_ref& ref, status_t* result);
};

extern const BPoint B_ORIGIN;

/*----- Type and STL utilities --------------------------------------*/
B_IMPLEMENT_SIMPLE_TYPE_FUNCS(BPoint);
void				BDestroy(BPoint* base, size_t count);
int32				BCompare(const BPoint& p1, const BPoint& p2);

ITextOutput::arg	operator<<(ITextOutput::arg io, const BPoint& point);

/*----------------------------------------------------------------*/
/*----- inline definitions ---------------------------------------*/

inline BPoint::BPoint()
{
}

inline BPoint::BPoint(coord X, coord Y)	: x(X), y(Y)
{
}

inline BPoint::BPoint(const BPoint& pt) : x(pt.x), y(pt.y)
{
}

inline BPoint &BPoint::operator=(const BPoint& from)
{
	x = from.x;
	y = from.y;
	return *this;
}

inline void BPoint::Set(coord X, coord Y)
{
	x = X;
	y = Y;
}

inline
coord BPoint::Angle(const BPoint& refpoint) const
{
	return (*this - refpoint).Angle();
}

inline
coord BPoint::Radius(const BPoint& refpoint) const
{
	const BPoint d(x-refpoint.x, y-refpoint.y);
	return d.Radius();
}


inline bool
BPoint::operator<(const BPoint& other) const
{
	return Compare(other) < 0;
}

inline bool
BPoint::operator<=(const BPoint& other) const
{
	return Compare(other) <= 0;
}

inline bool
BPoint::operator>=(const BPoint& other) const
{
	return Compare(other) >= 0;
}

inline bool
BPoint::operator>(const BPoint& other) const
{
	return Compare(other) > 0;
}

inline void BDestroy(BPoint* base, size_t count)
{
	(void)base; (void)count;
}

inline int32 BCompare(const BPoint& p1, const BPoint& p2)
{
	return p1.Compare(p2);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

} } // namespace B::Render2

#endif /* _RENDER2_POINT_H */
