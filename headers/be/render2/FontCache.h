/*******************************************************************************
/
/	File:			FontCache.h
/
/   Description:    Computes and caches font metric information.
/
/	Copyright 2001, Be Incorporated, All Rights Reserved
/
*******************************************************************************/

#ifndef _RENDER2_FONT_CACHE_H
#define _RENDER2_FONT_CACHE_H

#include <support2/SupportDefs.h>
#include <support2/String.h>
#include <support2/KeyedVector.h>
#include <support2/Locker.h>
#include <render2/IFontEngine.h>
#include <render2/Font.h>

namespace B {

namespace Render2 {

class BFont;

using namespace Support2;

// This is a rough outline of what the font cache should look like.


class BFontCache
{
public:
							BFontCache();
	virtual					~BFontCache();

	
			BPoint			StringEscapement(	const BFont& font,
												const char *string) const;
			BPoint			StringEscapement(	const BFont& font,
												const char *string,
												int32 length) const;
			
			float			StringWidth(		const BFont& font,
												const char *string) const;
			float			StringWidth(		const BFont& font,
												const char *string,
												int32 length) const;
		
			void			TruncateString(		const BFont& font,
												BString* in_out,
										  		uint32 mode,
										  		float width) const;
			
			status_t		GetMetrics(			const BFont& font,
												int32 glyph,
												glyph_metrics* out_metrics);
			status_t		GetGlyphMap(		const BFont& font,
												BGlyphMap* outMap);

			//! This is all the data we can cache about a particular glyph.
			struct metrics_entry {
				glyph_metrics			metrics;	// Standard metric info.
				void*					data;		// For use by subclasses.
			};

			enum {
				MAX_RUN_LENGTH = 20
			};

			//!	This is a block of characters that are going to be processed.
			struct glyph_run {
				/*BFont					font;*/
				int32					length;
				int32					glyphs[MAX_RUN_LENGTH];
			};

	static	const char*		NextGlyphRun(	const BFont& font,
											const char* string,
											const char* end,
											glyph_run* outRun);
			
			const char*		LockMetrics(	const glyph_run& run,
											const metrics_entry** outMetrics);
			void			UnlockMetrics(	const glyph_run& run,
											const metrics_entry** outMetrics);
			
			void			MoveCursor(		const BFont& font,
											const glyph_metrics& metrics,
											BPoint* inoutCursor) const;
			
protected:

			//!	Use the font's IFontEngine to place information about
			//!	\a glyph in to \a out_metrics.
			/*!	The default implementation only retrieves glyph_metrics,
				and sets \a out_metrics->data to NULL.  Subclasses can
				store custom information here, such as rendered bitmaps.
			*/
	virtual	status_t		CacheMetrics(	const BFont& font,
											int32 glyph,
											metrics_entry* out_metrics) const;
			
private:
			// This all the information about glyphs for a single BFont.
			struct font_entry {
				BFont								font;
				BGlyphMap							map;
				BKeyedVector<int32, metrics_entry>*	glyphs;
			};
	
	// All the cached information.
	mutable	BOrderedVector<font_entry>	m_fonts;
	mutable	font_entry					m_badFont;
	
			// looks for a font_entry for this font in m_fonts - if
			// not found creates one
			font_entry &	FontEntryFor(const BFont& font) const;
			// looks in the cache for the metrics for this glyph,
			// and if not present will generate it and cache it
			status_t		get_metrics(font_entry &entry, int32 glyph, glyph_metrics *out_metrics) const;
				
			// a temporary dumb locker for the cache
			// will be replaced by a smart read write locker later
			// and the code will be cleaned up considerably
	mutable		BNestedLocker					m_dumbLock;

};

// Implement comparison for font_entry, using only their
// physical attributes.
static	inline int32 BCompare(const BFontCache::font_entry& f1, const BFontCache::font_entry& f2)
{
	return f1.font.PhysicalCompare(f2.font);
}
static	inline int32 BLessThan(const BFontCache::font_entry& f1, const BFontCache::font_entry& f2)
{
	return f1.font.PhysicalCompare(f2.font) < 0;
}

} } // end namespace B::Render2

#endif
