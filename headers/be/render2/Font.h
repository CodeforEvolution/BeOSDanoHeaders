/*******************************************************************************
/
/	File:			Font.h
/
/   Description:    BFont objects represent individual font styles.
/                   Global font cache and font info functions defined below.
/
/	Copyright 1997-98, Be Incorporated, All Rights Reserved
/
*******************************************************************************/

#ifndef _RENDER2_FONT_H
#define _RENDER2_FONT_H


#include <support2/Value.h>
#include <support2/IContext.h>
#include <render2/2dTransform.h>
#include <render2/IFontEngine.h>


namespace B {
namespace Render2 {

class IRender;

class BFontCache;

using namespace Support2;

//!	Font faces.
enum {
	B_ITALIC_FACE		= 0x0001,
	B_UNDERSCORE_FACE	= 0x0002,
	B_NEGATIVE_FACE		= 0x0004,
	B_OUTLINED_FACE		= 0x0008,
	B_STRIKEOUT_FACE	= 0x0010,
	B_BOLD_FACE			= 0x0020,
	B_REGULAR_FACE		= 0x0040
};

//!	The different kind of spacing modes when placing characters.
enum {
	B_CHAR_SPACING		= 0,	//!	Use raw character spacing, non-integral.
	B_BITMAP_SPACING	= 1,	//!	Use hinted/integral character spacing.
	B_FIXED_SPACING		= 2,	//!	Use fixed-size character spacing.
};

//!	Options for TruncateString().
enum {
	B_TRUNCATE_END       = 0,
	B_TRUNCATE_BEGINNING = 1,
	B_TRUNCATE_MIDDLE    = 2,
	B_TRUNCATE_SMART     = 3
};

//!	This structure describes the horizontal spacing of a font.
struct font_height {
			float 				ascent;
			float 				descent;
			float 				leading;
	
	static	const font_height	undefined;
	
								font_height();
								font_height(float a, float d, float l);
								font_height(const font_height& fh);

			void				set_to(float _ascent, float _descent, float _leading);
						
			font_height &		operator=(const font_height& fh);
			bool				operator==(const font_height& fh) const;
			bool				operator!=(const font_height& fh) const;
};

//!	Description of a font.
class BFont
{
public:
								BFont(const IFontEngine::ptr& engine = IFontEngine::ptr());
								BFont(const IContext::ptr &context, status_t *result = NULL);
								BFont(const BFont &font);
								BFont(const BValue &o, status_t *result = NULL);
								BFont(const value_ref &o, status_t *result = NULL);
								~BFont();
	
		
			status_t			SetTo(const BValue &o);
		
			status_t			SetFamilyAndStyle(const char* family, 
												  const char* style);
			status_t			SetFamilyAndFace(const char* family, uint16 face);
		
			status_t			SetStyle(const char* style);
			status_t			SetFace(uint16 face);

			status_t			SetToNextFamily();
			status_t			SetToNextStyle();
			status_t			GetFamiliesAndStyles(BValue *info) const;
		
			status_t			Status() const;
		
			void				SetSize(float size);
			void				SetTransform(const B2dTransform& transform);
			void				SetSpacing(uint8 spacing);
			void				ApplyFlags(uint32 flags, uint32 which);
	
			void				GetFamilyAndStyle(BString* outFamily,
												  BString* outStyle) const;
			float				Size() const;
			B2dTransform		Transform() const;
			uint8				Spacing() const;
			uint16				Face() const;
			uint32				Flags() const;
			uint32				SelectedFlags() const;
		
			status_t			GetHeight(font_height* outHeight) const;
			
			status_t			GetGlyphFlags(	const char* charArray,
												size_t numChars,
												uint8* outFlags) const;
		
			status_t			GetGlyphMap(BGlyphMap* outMap) const;

			float				StringWidth(const char *string) const;
			float				StringWidth(const char *string, int32 length) const;

			BPoint				StringEscapement(const char *string) const;
			BPoint				StringEscapement(const char *string, int32 length) const;
			
			status_t			GetBoundingBox(BRect *rect) const;



		/* ----- Start of unimplemented API --- */
			void				TruncateString(BString* in_out,
											   uint32 mode,
											   float width) const;
		/* ----- End of unimplemented API --- */

	static	value_csml			key_Font;
	static	value_csml			key_Family;
	static	value_csml			key_Style;
	static	value_csml			key_Face;
	static	value_csml			key_Spacing;
	static	value_csml			key_Size;
	static	value_csml			key_Flags;
	static	value_csml			key_SelectedFlags;
	static	value_csml			key_Transform;

			BValue				AsValue() const;
	inline						operator BValue() const { return AsValue(); }		
		
			BFont &				operator=(const BFont &font);

	static	void				MoveBefore(BFont* to, BFont* from, size_t count = 1);
	static	void				MoveAfter(BFont* to, BFont* from, size_t count = 1);
			void				Swap(BFont& with);
			
			int32				Compare(const BFont &font) const;
			bool				operator!=(const BFont&) const;
			bool				operator==(const BFont&) const;
			bool				operator<(const BFont&) const;
			bool				operator<=(const BFont&) const;
			bool				operator>=(const BFont&) const;
			bool				operator>(const BFont&) const;

			int32				PhysicalCompare(const BFont &font) const;
		
			void				PrintToStream(ITextOutput::arg io, uint32 flags) const;
		
		/* API for internal IFontEngine use Only ------ */
	static	const int32			invalid_token;
		
			void				SetEngine(const IFontEngine::ptr& engine);
			void				SetToken(int32 token);
		
			IFontEngine::ptr	Engine() const;
			int32				Token() const;
		
	static	value_csml			key_EngineFont;
	static	value_csml			key_Engine;
	static	value_csml			key_Token;
			BValue				AsEngineValue() const;
		
			ssize_t				PacketSize() const;
			status_t			Packetize(BParcel& out) const;
			status_t			Unpacketize(BParcel& in);
		
	static	BFontCache *		Cache();
		
		/* -------------------------------------------- */
	private:
		
			struct font_matrix {
				coord				matrix[4];
									font_matrix(const B2dTransform &t);
									font_matrix(coord xx, coord yx, coord xy, coord yy);
									font_matrix(const font_matrix &m);
									font_matrix();
			
				font_matrix & 		scale_by(float x);
			
				font_matrix & 		operator=(const font_matrix &m);
				font_matrix & 		operator=(const B2dTransform &t);
				B2dTransform		as_2dtransform() const;
			
				bool				operator==(const font_matrix &m) const;
				bool				operator!=(const font_matrix &m) const;
				bool				operator==(const B2dTransform &t) const;
				bool				operator!=(const B2dTransform &t) const;
			};

			/* These are the physical attributes. */
			IFontEngine::ptr	m_engine;
			int32				m_token;
			coord				m_size;
			uint32				m_flags;
			uint32				m_selectedFlags;
			font_matrix			m_transform;
		
			/* These are the logical attributes. */
			uint8				m_spacing;
			uint8				_m_pad1;
			uint16				_m_pad2;
	mutable	font_height			m_cachedHeight;
		
	static	BFontCache *		sCache;
};

/*----- Type and STL utilities --------------------------------------*/
void			BMoveBefore(BFont* to, BFont* from, size_t count = 1);
void			BMoveAfter(BFont* to, BFont* from, size_t count = 1);
void			BSwap(BFont& t1, BFont& t2);
int32			BCompare(const BFont& t1, const BFont& t2);
void			swap(BFont& x, BFont& y);

ITextOutput::sptr_ref	operator<<(ITextOutput::sptr_ref io, const BFont& font);

/*-------------------------------------------------------------*/
/*---- No user serviceable parts after this -------------------*/

inline font_height::font_height()
	:	ascent(undefined.ascent), descent(undefined.descent), leading(undefined.leading)
{
}

inline font_height::font_height(float a, float d, float l)
	:	ascent(a), descent(d), leading(l)
{
}

inline font_height::font_height(const font_height& fh)
	:	ascent(fh.ascent), descent(fh.descent), leading(fh.leading)
{
}

inline void font_height::set_to(float _ascent, float _descent, float _leading)
{
	ascent = _ascent; descent = _descent; leading = _leading;
}
					
inline font_height& font_height::operator=(const font_height& fh)
{
	ascent = fh.ascent; descent = fh.descent; leading = fh.leading;
	return *this;
}

inline bool font_height::operator==(const font_height& fh) const
{
	return ((ascent == fh.ascent) && (descent == fh.descent) && (leading == fh.leading));
}

inline bool font_height::operator!=(const font_height& fh) const
{
	return ((ascent != fh.ascent) || (descent != fh.descent) || (leading != fh.leading));
}

inline bool BFont::operator<(const BFont& other) const
{
	return Compare(other) < 0;
}

inline bool BFont::operator<=(const BFont& other) const
{
	return Compare(other) <= 0;
}

inline bool BFont::operator>=(const BFont& other) const
{
	return Compare(other) >= 0;
}

inline bool BFont::operator>(const BFont& other) const
{
	return Compare(other) > 0;
}

inline void BMoveBefore(BFont* to, BFont* from, size_t count)
{
	BFont::MoveBefore(to, from, count);
}

inline void BMoveAfter(BFont* to, BFont* from, size_t count)
{
	BFont::MoveAfter(to, from, count);
}

inline void BSwap(BFont& x, BFont& y)
{
	x.Swap(y);
}

inline int32 BCompare(const BFont& t1, const BFont& t2)
{
	return t1.Compare(t2);
}

inline void swap(BFont& x, BFont& y)
{
	x.Swap(y);
}

} } // end namespace B::Render2

#endif
