/*******************************************************************************
/
/	File:			render2/Gradient.h
/
/   Description:    BGradient describes a linear color gradient
/
/	Copyright: 		Copyright 2001, Be Incorporated, All Rights Reserved
/
*******************************************************************************/

#ifndef	_RENDER2_GRADIENT_H
#define	_RENDER2_GRADIENT_H

#include <support2/SupportDefs.h>
#include <render2/RenderDefs.h>
#include <render2/Point.h>
#include <render2/Color.h>
#include <render2/2dTransform.h>
#include <support2/Flattenable.h>

namespace B {
namespace Render2 {

using namespace Support2;

/*----------------------------------------------------------------*/
/*----- BGradient class ------------------------------------------*/

class BGradient : public BFlattenable
{
public:

	enum element {
			Rdx = 0,	Gdx = 1,	Bdx = 2,	Adx = 3,
			Rdy = 4,	Gdy = 5,	Bdy = 6,	Ady = 7,
			R = 8,		G = 9,		B = 10,		A = 11
	};

					BGradient();
					BGradient(const BGradient &grad);
					BGradient(const BPoint &p0, const BColor &c0, const BPoint &p1, const BColor &c1);
					BGradient(const BPoint &p0, const BColor &c0, float theta, const BColor &dc);
					BGradient(const BValue& value, status_t* result = NULL);
					BGradient(const value_ref& value, status_t* result = NULL);

	const	float *	Matrix() const;

			void	Set(const BPoint &p0, const BColor &c0, const BPoint &p1, const BColor &c1);
			void	Set(const BPoint &p0, const BColor &c0, float theta, const BColor &dc);

			void	SetRed(const BPoint &p0, float start, const BPoint &p1, float end);
			void	SetGreen(const BPoint &p0, float start, const BPoint &p1, float end);
			void	SetBlue(const BPoint &p0, float start, const BPoint &p1, float end);
			void	SetAlpha(const BPoint &p0, float start, const BPoint &p1, float end);

			void	Transform(const B2dTransform &xform);
			void	Transform(const BColorTransform &xform);

		void		PrintToStream(ITextOutput::arg io, uint32 flags=0) const;
static	status_t	printer(ITextOutput::arg io, const value_ref& val, uint32 flags);

	// BFlattenable API
	virtual	bool		IsFixedSize() const;
	virtual	type_code	TypeCode() const;
	virtual	ssize_t		FlattenedSize() const;
	virtual	status_t	Flatten(void *buffer, ssize_t size) const;
	virtual	status_t	Unflatten(type_code c, const void *buf, ssize_t size);

private:
			void	init();
			void	set_component(color_component component, const BPoint &p0, float start, const BPoint &p1, float end);
			float	m_matrix[12];
			uint32	m_flags;
};

/*----- Type and STL utilities --------------------------------------*/

const ITextOutput::sptr&	operator<<(const ITextOutput::sptr& io, const BGradient& g);

/*-------------------------------------------------------------------*/

inline void 
BGradient::SetRed(const BPoint &p0, float start, const BPoint &p1, float end)
{
	set_component(B_RED,p0,start,p1,end);
}

inline void 
BGradient::SetGreen(const BPoint &p0, float start, const BPoint &p1, float end)
{
	set_component(B_GREEN,p0,start,p1,end);
}

inline void 
BGradient::SetBlue(const BPoint &p0, float start, const BPoint &p1, float end)
{
	set_component(B_BLUE,p0,start,p1,end);
}

inline void 
BGradient::SetAlpha(const BPoint &p0, float start, const BPoint &p1, float end)
{
	set_component(B_ALPHA,p0,start,p1,end);
}

inline const float * BGradient::Matrix() const
{
	return m_matrix;
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

} } // namespace B::Render2

#endif /* _RENDER2_POINT_H */
