/***************************************************************************
//
//	File:			font2/FontEngine.h
//
//	Description:	BFontEngine is a concrete implementation of IFontEngine
//					based on the FontFusion font engine
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _FONT2_FONT_ENGINE_H
#define _FONT2_FONT_ENGINE_H

#include <render2/IFontEngine.h>
#include <storage2/Entry.h>
#include <storage2/Node.h>
#include <support2/IStorage.h>
#include <support2/ITextStream.h>
#include <support2/Locker.h>
#include <support2/Vector.h>

namespace B {

namespace Font2 {

using namespace B::Render2;
using namespace B::Storage2;

namespace Private {
	class FontHandle;
	class ActiveFont;
	class FontScanner;

	typedef atom_ptr<FontHandle>	fh_ptr;
	typedef BVector<fh_ptr > HandleVector;
	typedef BVector<ActiveFont> ActiveVector;
	typedef int16	font_token;
}

enum {
	SCAN_FONT_DIRS = (1<<0)
};

class BFontEngine : public LFontEngine
{
	public:
							BFontEngine(uint32 flags = 0, int32 activeFonts = 2);
		virtual				~BFontEngine();
		virtual	ssize_t		RenderGlyph(	int32 glyph,
											BFont* inoutFont,
											glyph_metrics* outMetrics,
											const IRender::ptr & render,
											bool clipToGlyph) const;
		
		virtual BValue		GetFamiliesAndStyles() const;
		virtual	status_t	GetNextFamily(BFont* inoutFont) const;
		virtual	status_t	GetNextStyle(BFont* inoutFont) const;
		
		virtual	status_t	SetFamilyAndStyle(	BFont* inoutFont,
												const char* family,
												const char* style) const;
		
		virtual	status_t	SetFamilyAndFace(	BFont* inoutFont,
												const char* family,
												uint16 face) const;
		
		virtual	status_t	GetFamilyAndStyle(	const BFont& font,
												BString* outFamily,
												BString* outStyle) const;
		
		virtual	status_t	GetHeight(const BFont& font, font_height* outHeight);
		virtual	status_t	GetGlyphFlags(	const BFont& font,
											const char* charArray,
											size_t numChars,
											uint8* outFlags) const;
		virtual	status_t	GetGlyphMap(	const BFont& font,
											BGlyphMap* outMap) const;
		virtual status_t	GetBoundingBox(	const BFont &font,
											BRect *rect) const;
	
		// debug output
		void				PrintToStream(ITextOutput::arg io, uint32 flags) const;
	
		// called by the FontScanner to Add and Remove fonts
		void				AddFont(const node_ref *node, const entry_ref *entry);
		void				RemoveFont(const node_ref *node, const entry_ref *entry);

		Private::fh_ptr		HandleFor(const BFont &font) const;
		Private::fh_ptr		HandleFor(const char *family, const char *style) const;
		Private::fh_ptr		HandleFor(const char *family, uint16 faces) const;
		Private::fh_ptr		HandleFor(const node_ref *node) const;
	
	private:
		status_t			ScanFontDirs();
		
		
		Private::ActiveFont & ActivateFont(const BFont &font, lock_status_t &lock, Private::fh_ptr &fh) const;
						
		BLocker							fLock;
		Private::font_token				fNextToken;
		mutable BLocker					fHandleLock;
		Private::HandleVector			fHandles;
		mutable	Private::ActiveVector	fActives;
		Private::FontScanner *			fScanner;
		BValue							fFamiliesAndStyles;
};

} } // namespace B::Font2

#endif
