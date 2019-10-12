/***************************************************************************
//
//	File:			render2/ColorTransform.h
//
//	Description:	Class representating a general colorspace transform
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _RENDER2_COLORTRANSFORM_H
#define _RENDER2_COLORTRANSFORM_H

#include <render2/RenderDefs.h>
#include <render2/Color.h>
#include <support2/Flattenable.h>

namespace B {
namespace Render2 {

using namespace Support2;

/*******************************************************************/

/*! This class represents a transform through RGBA space.  Because of
	the definition of the alpha component within the rendering model,
	alpha transformation options are severly limited.  More extensive
	alpha transformations may be allowed later as part of the grouping
	facility.
*/

enum {
	B_COLOR_TRANSFORM_IDENTITY	= 0x00,
	B_COLOR_TRANSFORM_ALL		= 0xFF
};

class BColorTransform : public BFlattenable
{
public:

	enum element {
			//! General 4x5 color transform
			RR = 0,		RG = 3,		RB = 6,		/* 0, */	RT = 9,
			GR = 1,		GG = 4,		GB = 7,		/* 0, */	GT = 10,
			BR = 2,		BG = 5,		BB = 8,		/* 0, */	BT = 11,
			/* 0,		0,			0, */		AA = 12		/* 0 */
	};

								BColorTransform();
								BColorTransform(const BColorTransform &xform);
								BColorTransform(const float *matrix, uint8 operations);
								BColorTransform(const BValue& value, status_t* result = NULL);
								BColorTransform(const value_ref& value, status_t* result = NULL);

			void 				SetTo(const BColorTransform& from);

			BColorTransform&	operator=(const BColorTransform& from);

			BValue				AsValue() const;
inline							operator BValue() const	{ return AsValue(); }

			const float *		Matrix() const;

			bool				IsIdentity() const { return m_flags == B_COLOR_TRANSFORM_IDENTITY; }

	//! Return the identity transformation
	static	const BColorTransform&	MakeIdentity();

	//! Color saturation (0 to 1), 0 transforms to grey, 1 is identity
	static	BColorTransform			MakeSaturate(float saturation);
	
	//! Modulation by a color
	static	BColorTransform			MakeModulate(const BColor &color);

	//! Make one or several components constant
	static	BColorTransform			MakeConstant(color_component which, float value);
	static	BColorTransform			MakeConstant(const BColor &color, uint32 colorMask=B_ALL_MASK);

			BColorTransform &		Concatenate(const BColorTransform &color);
			void					Concatenate(const BColorTransform &color, BColorTransform* dest) const;
			BColorTransform &		PreConcatenate(const BColorTransform &color);
			void					PreConcatenate(const BColorTransform &color, BColorTransform* dest) const;

			BColorTransform			operator *(const BColorTransform &color) const;
			BColorTransform &		operator *=(const BColorTransform &color);

			BColor					Transform(const BColor &color) const;

		void						PrintToStream(ITextOutput::arg io, uint32 flags=0) const;
static	status_t					printer(ITextOutput::arg io, const value_ref& val, uint32 flags);

	//! BFlattenable API
	virtual	bool					IsFixedSize() const;
	virtual	type_code				TypeCode() const;
	virtual	ssize_t					FlattenedSize() const;
	virtual	status_t				Flatten(void *buffer, ssize_t size) const;
	virtual	status_t				Unflatten(type_code c, const void *buf, ssize_t size);

private:
									BColorTransform(bool);
	static	void					multiply(float *r, const float *m1, const float *m2);
	static	float					m_identity_matrix[13];

	//!< NOTE: This variable is made public by the inlined MakeIdentity().
	static	const BColorTransform	m_identity;

			float					m_matrix[13];
			uint32					m_flags;
};

/*----- Type and STL utilities --------------------------------------*/

ITextOutput::arg	operator<<(ITextOutput::arg io, const BColorTransform& matrix);

/*-------------------------------------------------------------------*/
/*----- inline definitions ------------------------------------------*/

inline BColorTransform& BColorTransform::operator=(const BColorTransform& from)
{
	SetTo(from);
	return *this;
}

inline const BColorTransform& BColorTransform::MakeIdentity()
{
	return m_identity;
}

inline const float *BColorTransform::Matrix() const {
	return m_matrix;
}

} }

#endif
