/***************************************************************************
//
//	File:			render2/IFontEngine.h
//
//	Description:	Abstract font management and rendering class.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _RENDER2_FONT_ENGINE_INTERFACE_H
#define _RENDER2_FONT_ENGINE_INTERFACE_H

#include <support2/IInterface.h>
#include <support2/Binder.h>
#include <render2/IRender.h>

namespace B { namespace Support2 { class BSharedBuffer; } }

namespace B {
namespace Render2 {

class BFont;
struct font_height;

struct glyph_metrics {
	BRect bounds;			// Location and size of rendered bits
	coord x_escape;			// "Real" escapement to next character
	coord y_escape;
	int32 x_bm_escape;		// Hinted escapement to next character
	int32 y_bm_escape;

	void reset() {
		bounds.Set(0, 0, -1, -1);
		x_bm_escape = y_bm_escape = 0;
		x_escape = y_escape = 0.0;
	}
	
	glyph_metrics() { reset(); }
	glyph_metrics(const BValue &val, status_t *error);
	status_t set_to(const BValue &val);
	BValue	as_value() const;
};

enum {
	B_GLYPH_MAP_TYPE = 'GLMP'
};

class BGlyphMap
{
public:

									BGlyphMap();
									BGlyphMap(const BValue& value);
									BGlyphMap(const BGlyphMap& other);
									~BGlyphMap();
	
			BGlyphMap&				operator=(const BGlyphMap& o);
			
			BValue					AsValue() const;
	inline							operator BValue() const { return AsValue(); }
	
			status_t				Status() const;
			
			status_t				Set(const uint32* bitmap, size_t bitmap_cnt);
			
			bool					HasRange(int32 first, int32 last) const;
			
			void					PrintToStream(ITextOutput::arg io, uint32 flags) const;
		
	typedef	uint32					glyph_block[256/32];
			struct					glyph_range {
				int32				first;
				int32				last;
			};
	
private:
			const BSharedBuffer*	m_data;
};

ITextOutput::arg operator<<(ITextOutput::arg io, const BGlyphMap& map);

class IFontEngine : public IInterface
{
	public:
		B_DECLARE_META_INTERFACE(FontEngine);
		
		
		//!	Given a font specification, render it and return
		//!	The resulting bitmap.
		/*!	Fills in \a outMetrics with the dimensions of the
			rendered glyph, and does the actual rendering in
			to \a render.  The font's left/baseline is located
			at the point (0,0) when rendered.  If \a render is
			NULL, the actual rendering is not performed (but
			metric information is still returned).
			
			In addition, RenderGlyph() may return B_REDIRECT.
			In this case, outMetrics is invalid and no rendering
			is performed; \a inoutFont is filled in with a new
			font to render.  This is intended to support font
			substitution, without duplicating rendered glyphs in
			the caller's cache.
		*/
		virtual	ssize_t		RenderGlyph(	int32 glyph,
											BFont* inoutFont,
											glyph_metrics* outMetrics,
											const IRender::ptr & render,
											bool clipToGlyph = false) const = 0;
		static const BValue	key_RenderGlyph;
											

		//!	Return a BValue filled with all the available font families and styles.
		/*!	Returned format:
			
				Family1->{Style1, Style2, Style3 ...}
				Family2->{Style1, Style2, Style3 ...}
		*/
		virtual BValue		GetFamiliesAndStyles() const = 0;
		static const BValue key_GetFamiliesAndStyles;
		
		//!	Iterate through all available font families.
		/*!	Calling with an empty BFont returns the first family, each
			successive call returns the next family.
		*/
		virtual	status_t	GetNextFamily(BFont* inoutFont) const = 0;
		static const BValue	key_GetNextFamily;
		
		//!	Iterate through all designed styles in a family.
		/*!	Changes the given font to the next style in its
			family.
		*/
		virtual	status_t	GetNextStyle(BFont* inoutFont) const = 0;
		static const BValue	key_GetNextStyle;
		
		//!	Change the font to the given family and style in this engine.
		virtual	status_t	SetFamilyAndStyle(	BFont* inoutFont,
												const char* family,
												const char* style) const = 0;
		static const BValue	key_SetFamilyAndStyle;
		
		//!	Change the font to the given family and face in this engine.
		virtual	status_t	SetFamilyAndFace(	BFont* inoutFont,
												const char* family,
												uint16 face) const = 0;
		static const BValue	key_SetFamilyAndFace;
		
		//!	Return the family and style names for this font.
		virtual	status_t	GetFamilyAndStyle(	const BFont& font,
												BString* outFamily,
												BString* outStyle) const = 0;
		static const BValue	key_GetFamilyAndStyle;
		
		//!	Return the height metrics for the given font.
		virtual	status_t	GetHeight(const BFont& font, font_height* outHeight) = 0;
		static const BValue	key_GetHeight;
		
		//!	Return information about character glyphs in the selected font.
		enum {
			GLYPH_EXISTS = 1<<0,			// Glyph exists in the font.
			GLYPH_LOCAL_OVERLAY = 1<<1,		// Glyph can be produced by local overlay.
			GLYPH_GLOBAL_OVERLAY = 1<<2		// Glyph can be produced by global overlay.
		};
		virtual	status_t	GetGlyphFlags(	const BFont& font,
											const char* charArray,
											size_t numChars,
											uint8* outFlags) const = 0;
		static const BValue	key_GetGlyphFlags;

		
		//!	Return a glyph map indicating which glyphs exist in the selected font.
		virtual	status_t	GetGlyphMap(	const BFont& font,
											BGlyphMap* outMap) const = 0;
		static const BValue	key_GetGlyphMap;
		
		//! Return a BRect representing the max bounding box of the selected font
		virtual status_t	GetBoundingBox( const BFont& font,
											BRect *rect) const = 0;
		static const BValue key_GetBoundingBox;
};

class LFontEngine : public LInterface<IFontEngine>
{
	public:
							LFontEngine() : LInterface<IFontEngine>(IContext::ptr()) {}
	protected:
		virtual	status_t	HandleEffect(const BValue &in, const BValue &inBindings, const BValue &outBindings, BValue *out);

	inline virtual			~LFontEngine() { }
};

} } // namespace B::Render2

#endif
