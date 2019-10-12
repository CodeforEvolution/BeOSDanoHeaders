/***************************************************************************
//
//	File:			render2/Color.h
//
//	Description:	Definitions of RGB color types.
//					BColor -- a color in floating point values.
//					BColor32 -- a color in integer values.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef	_RENDER2_COLOR_H
#define	_RENDER2_COLOR_H

#include <support2/SupportDefs.h>

#ifdef __cplusplus
#include <support2/ITextStream.h>
#include <support2/TypeFuncs.h>
#include <support2/Value.h>

namespace B {
namespace Render2 {
using namespace Support2;
#endif

struct BColor32;

struct BColor
{
	float	red;
	float	green;
	float	blue;
	float	alpha;
		
#ifdef __cplusplus
						BColor();
						BColor(float r, float g, float b, float a=1.0f);
						BColor(float gray, float alpha=1.0f);
						BColor(const BColor &o);
						BColor(const BColor32 &o);
						BColor(const BValue &o, status_t *result = NULL);
						BColor(const value_ref &o, status_t *result = NULL);
			
			BColor		operator+(const BColor& o) const;
			BColor		operator-(const BColor& o) const;
			bool		operator==(const BColor& o) const;
			bool		operator!=(const BColor& o) const;
		
			BValue		AsValue() const;
	inline				operator BValue() const		{ return AsValue(); }
			
			void		PrintToStream(ITextOutput::arg io, uint32 flags=0) const;
	static status_t		printer(ITextOutput::arg io, const value_ref& val, uint32 flags);

	//! Some usefull predefined colors
	static const		BColor transparent;
	static const		BColor black;
	static const		BColor white;

private:
			void		Construct(const value_ref &ref, status_t *result);
#endif
};

#ifdef __cplusplus
B_IMPLEMENT_SIMPLE_TYPE_FUNCS(BColor);

ITextOutput::arg operator<<(ITextOutput::arg io, const BColor& c);
#endif

//------------------------------------------------------------------------------

struct BColor32 {
			uint8		red;
			uint8		green;
			uint8		blue;
			uint8		alpha;

#ifdef __cplusplus
						BColor32();
						BColor32(uint8 r, uint8 g, uint8 b, uint8 a=255);
						BColor32(uint8 gray, uint8 a=255);
						BColor32(const BColor32 &o);
						BColor32(const BColor &o);
						BColor32(const BValue &o, status_t *result = NULL);
						BColor32(const value_ref &o, status_t *result = NULL);
		
			bool		operator==(const BColor32& o) const;
			bool		operator!=(const BColor32& o) const;
		
			BValue		AsValue() const;
	inline				operator BValue() const		{ return AsValue(); }
	
			void		PrintToStream(ITextOutput::arg io, uint32 flags=0) const;
	static status_t		printer(ITextOutput::arg io, const value_ref& val, uint32 flags);

	//! Some usefull predefined colors
	static const		BColor32 transparent;
	static const		BColor32 black;
	static const		BColor32 white;
	static const		BColor32 michelangelo_favorite_color;
	static const		BColor32 dsandler_favorite_sky_color;
	static const		BColor32 dsandler_favorite_ink_color;
	static const		BColor32 dsandler_favorite_shoes_color;
	static const		BColor32 dave_brown_favorite_color;

private:
			void		Construct(const value_ref &ref, status_t *result);
#endif
};

#ifdef __cplusplus
B_IMPLEMENT_SIMPLE_TYPE_FUNCS(BColor32);

ITextOutput::arg operator<<(ITextOutput::arg io, const BColor32& c);

/*-------------------------------------------------------------*/
/*---- No user serviceable parts after this -------------------*/

inline BColor::BColor()
{
}

inline BColor::BColor(const BColor &o)
	:	red(o.red), green(o.green), blue(o.blue), alpha(o.alpha)
{
}

inline BColor::BColor(const BColor32 &o)
	:	red(o.red/255.0f), green(o.green/255.0f),
		blue(o.blue/255.0f), alpha(o.alpha/255.0f)
{
}

inline BColor::BColor(float r, float g, float b, float a)
	:	red(r), green(g), blue(b), alpha(a)
{
}

inline BColor::BColor(float g, float a)
	:	red(g), green(g), blue(g), alpha(a)
{
}

inline BColor 
BColor::operator+(const BColor &o) const
{
	return BColor(red+o.red,green+o.green,blue+o.blue,alpha+o.alpha);
}

inline BColor 
BColor::operator-(const BColor &o) const
{
	return BColor(red-o.red,green-o.green,blue-o.blue,alpha-o.alpha);
}

inline bool BColor::operator==(const BColor& o) const
{
	return
		(red == o.red) &&
		(green == o.green) &&
		(blue == o.blue) &&
		(alpha == o.alpha) ;
}

inline bool BColor::operator!=(const BColor& o) const
{
	return
		(red != o.red) ||
		(green != o.green) ||
		(blue != o.blue) ||
		(alpha != o.alpha) ;
}

//------------------------------------------------------------------------------

inline BColor32::BColor32()
{
}

inline BColor32::BColor32(const BColor32 &o)
	:	red(o.red), green(o.green), blue(o.blue), alpha(o.alpha)
{
}

inline BColor32::BColor32(const BColor &o)
	:	red((uint8)(o.red*255+.5)), green((uint8)(o.green*255+.5)),
		blue((uint8)(o.blue*255+.5)), alpha((uint8)(o.alpha*255+.5))
{
}

inline BColor32::BColor32(uint8 r, uint8 g, uint8 b, uint8 a)
	:	red(r), green(g), blue(b), alpha(a)
{
}

inline BColor32::BColor32(uint8 g, uint8 a)
	:	red(g), green(g), blue(g), alpha(a)
{
}

inline bool BColor32::operator==(const BColor32& o) const
{
	return (*((uint32*)this)) == (*((uint32*)&o));
}

inline bool BColor32::operator!=(const BColor32& o) const
{
	return (*((uint32*)this)) != (*((uint32*)&o));
}

} } // namespace B::Render2
#endif

#endif /* _RENDER2_COLOR_H */
