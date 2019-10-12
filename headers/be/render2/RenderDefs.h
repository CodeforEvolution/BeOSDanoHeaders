/*******************************************************************************
/
/	File:			GraphicsDefs.h
/
/   Description:    Color space definitions.
/
/	Copyright 1992-98, Be Incorporated, All Rights Reserved
/
*******************************************************************************/

#ifndef _RENDER2_RENDERDEFS_H
#define _RENDER2_RENDERDEFS_H

#include <OS.h>
#include <support2/SupportDefs.h>
#include <support2/Value.h>

namespace B {
namespace Render2 {

using namespace Support2;

class IRender;
class IVisual;
class BRegion;
class BUpdate;
class BFont;
class B2dTransform;
class BColorTransform;

/*----------------------------------------------------------------*/

enum color_component {
	B_RED = 0,
	B_GREEN = 1,
	B_BLUE = 2,
	B_ALPHA = 3
};

enum color_component_mask {
	B_RED_MASK		= 0x01,
	B_GREEN_MASK	= 0x02,
	B_BLUE_MASK		= 0x04,
	B_ALPHA_MASK	= 0x08,
	B_ALL_MASK		= 0x0F
};

enum winding_rule {
	B_WINDING_EVENODD = 1,
	B_WINDING_NONZERO,
	B_WINDING_POSITIVE,
	B_WINDING_NEGATIVE,
	B_WINDING_ABS_GEQ_TWO
};

enum texttopath_flags {
	B_JUSTIFY_LEFT		= 0x00000001,
	B_JUSTIFY_RIGHT		= 0x00000002,
	B_JUSTIFY_FULL		= 0x00000003,
	B_JUSTIFY_CENTER	= 0x00000004,

	B_VALIGN_BASELINE	= 0x00000010,
	B_VALIGN_TOP		= 0x00000020,
	B_VALIGN_BOTTOM		= 0x00000030,
	B_VALIGN_CENTER		= 0x00000040
};

enum join_mode {
	B_MITER_JOIN = 0,
	B_ROUND_JOIN,
	B_BEVEL_JOIN
};

enum cap_mode {
	B_BUTT_CAP = 0,
	B_ROUND_CAP,
	B_SQUARE_CAP,
	B_ARROW_CAP
};

enum group_flags {
	B_MODULATE			= 0x00000001,	//!< create a modulation group
	B_INVERSE			= 0x00000002,	//!< inverse colors
	B_INVERSE_MODULATE	= 0x00000003,	//!< modulate to inverse (eg: inverse clipping)
	B_FORCE_MODULATION	= 0x20000000,	//!< mostly for debugging only
	B_SYSTEM_RESERVED0	= 0x40000000,	//!< don't use this flag
	B_SYSTEM_RESERVED1	= 0x80000000	//!< don't use this flag
};

//! Render flags.
enum render_flags {
	B_DISABLE_ANTIALIASING		= 0x00000000,
	B_NORMAL_ANTIALIASING		= 0x00000001,
	B_TV_ANTIALIASING			= 0x00000002,
	B_ANTIALIASING_MASK			= 0x00000007,
	
	B_DISABLE_HINTING			= 0x00000000,
	B_ENABLE_HINTING			= 0x00000008,
	B_HINTING_MASK				= 0x00000008,
	
	B_DISABLE_TEXT_ANTIALIASING	= 0x00000000,
	B_NORMAL_TEXT_ANTIALIASING	= 0x00010000,
	B_TV_TEXT_ANTIALIASING		= 0x00020000,
	B_TEXT_ANTIALIASING_MASK	= 0x00070000,
	
	B_DISABLE_TEXT_HINTING		= 0x00000000,
	B_ENABLE_TEXT_HINTING		= 0x00080000,
	B_TEXT_HINTING_MASK			= 0x00080000,
	
	B_DISABLE_GLOBAL_OVERLAY	= 0x00000000,
	B_ENABLE_GLOBAL_OVERLAY		= 0x00100000,
	B_GLOBAL_OVERLAY_MASK		= 0x00100000
};

/*----------------------------------------------------------------*/

#define B_UTF8_ELLIPSIS		"\xE2\x80\xA6"
#define B_UTF8_OPEN_QUOTE	"\xE2\x80\x9C"
#define B_UTF8_CLOSE_QUOTE	"\xE2\x80\x9D"
#define B_UTF8_COPYRIGHT	"\xC2\xA9"
#define B_UTF8_REGISTERED	"\xC2\xAE"
#define B_UTF8_TRADEMARK	"\xE2\x84\xA2"
#define B_UTF8_SMILING_FACE	"\xE2\x98\xBB"
#define B_UTF8_HIROSHI		"\xE5\xBC\x98"

/*----------------------------------------------------------------*/

#define 	B_CSTR_LEN		(-1)

struct escapements {
	escapements() { }
	escapements(uint32 n, uint32 s) : nonspace(n), space(s) { }
	bool operator == (const escapements& e) const { return ((nonspace==e.nonspace) && (space==e.space)); }
	coord nonspace;
	coord space;
};

extern const escapements B_NO_ESCAPEMENT;

/*----------------------------------------------------------------*/

struct pixel_format {
	pixel_format() { }
	pixel_format(uint32 l, uint32 c) : layout(l), components(c) { }
	bool operator == (const pixel_format& other) {
		return ((layout == other.layout) && (components == other.components));
	}
	bool operator != (const pixel_format& other) { return !operator == (other); }
	uint32	layout;
	uint32	components;
};


class BPixelData;


/*! BPixelDescription
	Describes the geometry of a block of pixel. Basically
	this object contains the size and color format of the
	pixels but not the pixels themselves.
		\sa class BPixelData
		\sa IRender::BeginPixelBlock()
		\sa IRender::PalcePixels()
		\sa IRender::EndPixelBlock()
*/

class BPixelDescription
{
public:
	enum {
		B_EDGE_PIXELS = 0x00000001
	};

	BPixelDescription()  { }
	BPixelDescription(uint32 width, uint32 height, pixel_format pf, uint32 flags = 0)
		: 	m_pixelFormat(pf), m_width(width), m_height(height), m_flags(flags) { }
	BPixelDescription(const BValue& from);

	BValue AsValue() const;
	inline operator BValue() const { return AsValue(); }
	
	bool operator == (const BPixelDescription& other) {
		return ((m_width == other.m_width) &&
				(m_height == other.m_height) &&
				(m_pixelFormat == other.m_pixelFormat) &&
				(m_flags == other.m_flags));
	}
	bool operator != (const BPixelDescription& other) { return !operator == (other); }

	uint32				Width() const		{ return m_width; }
	uint32				Height() const		{ return m_height; }
	const pixel_format&	ColorSpace() const	{ return m_pixelFormat; }
	uint32				Flags() const		{ return m_flags; }

private:
	friend class BPixelData;
	pixel_format	m_pixelFormat;
	uint32			m_width;
	uint32			m_height;
	uint32			m_flags;
};


/*! BPixelData
	This object is basically a bitmap. It contains the pixel
	description (size and color format) as well as the data.	
		\sa class BPixelDescription
		\sa IRender::BeginPixelBlock()
		\sa IRender::PalcePixels()
		\sa IRender::EndPixelBlock()
*/

class BPixelData : public BPixelDescription
{
public:
	BPixelData()  { }
	BPixelData(	const BPixelDescription& pd,
				uint32 bytesPerRow,
				const void* data,
				area_id area=-1)
		:	BPixelDescription(pd),
			m_bytesPerRow(bytesPerRow), m_data(data), m_area(area)	{ }
	BPixelData(	uint32 width,
				uint32 height,
				uint32 bytesPerRow,
				const pixel_format& pf,
				const void* data,
				area_id area=-1)
		:	BPixelDescription(width, height, pf, 0),
			m_bytesPerRow(bytesPerRow), m_data(data), m_area(area) { }

	bool operator == (const BPixelData& other) {
		return ((BPixelDescription::operator == (other)) &&
				(m_bytesPerRow == other.m_bytesPerRow) &&
				(m_data == other.m_data) &&
				(m_area == other.m_area));
	}
	bool operator != (const BPixelData& other) { return !operator == (other); }

	
	uint32			BytesPerRow() const		{ return m_bytesPerRow; }
	size_t			Size() const			{ return m_bytesPerRow*m_height; }
	bool			IsValid() const			{ return (m_data!=NULL); }
	const void*		Pixels() const			{ return m_data; };

	//! Deprecated. Will go away.
	const void*		Data() const			{ return Pixels(); }

private:
	bool			IsArea() const			{ return (m_area >= 0); }
	area_id			Area() const			{ return m_area; }

private:
	uint32			m_bytesPerRow;
	const void* 	m_data;
	area_id			m_area;		
};



/*----------------------------------------------------------------*/

} } // namespace B::Render2

#endif	/* _RENDER2_RENDERDEFS_H */
