/***************************************************************************
//
//	File:			render2/Canvas.h
//
//	Description:	Abstract drawing interface.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _RENDER2_CANVAS_H_
#define _RENDER2_CANVAS_H_

#include <math.h>
#include <support2/Binder.h>
#include <support2/Locker.h>
#include <support2/Vector.h>
#include <render2/Color.h>
#include <render2/ColorTransform.h>
#include <render2/Point.h>
#include <render2/Rect.h>
#include <render2/LRender.h>
#include <render2/RenderDefs.h>
#include <render2/Path.h>
#include <render2/Font.h>
#include <render2/FontCache.h>
#include <render2/Pixmap.h>
#include <render2/2dTransform.h>
#include <raster2/RasterDefs.h>

namespace B {
namespace Render2 {

using namespace Support2;
using namespace Raster2;

/**************************************************************************************/

class BCanvas : public LRender
{
	public:

		B_STANDARD_ATOM_TYPEDEFS(BCanvas);

										BCanvas(const BStaticPixmap::sptr& onto);
										BCanvas(const BPixelData& onto);
		virtual							~BCanvas();

		virtual	void					MoveTo(const BPoint& pt);
		virtual	void					LinesTo(const BPoint* points, int32 lineCount=1);
		virtual	void					BeziersTo(const BPoint* points, int32 bezierCount=1);
		virtual	void					ArcsTo(const BPoint* points, const coord* radius, int32 arcCount=1) ;
		virtual	void					Close();

		virtual	IRender::sptr			Branch(uint32 flags);
		virtual void					Sync();
		virtual	void					ApplyFlags(uint32 flags, uint32 which);

		virtual	void					TextAt(	const BPoint&,
												const char *text,
												int32 len = B_CSTR_LEN,
												const escapements& escapements = B_NO_ESCAPEMENT);

		virtual	void					Stroke();
		virtual	void					TextOnPath(	const char *text,
													int32 len = B_CSTR_LEN,
													const escapements& escapements = B_NO_ESCAPEMENT,
													uint32 flags = B_JUSTIFY_LEFT|B_VALIGN_BASELINE);

		virtual	void					SetFont(const BFont &font);
		virtual	void					SetWindingRule(winding_rule rule);
		virtual void					SetPenSize(coord penWidth);
		virtual void					SetCaps(cap_mode start_cap, cap_mode end_cap);
		virtual void					SetJoin(join_mode join, float mitter_limit=10.0f);
		virtual void					SetStippling(const stipple_t *stippling=NULL, int32 offset = 0);

		virtual void					SetColor(const BColor&);
		virtual void					SetGradient(const BGradient&);
		virtual	void					Fill();
		virtual	void					Paint();

		virtual	void					Transform(const B2dTransform&);
		virtual	void					Transform(const BColorTransform&);

		virtual	void					BeginGroup(uint32 flags=0);
		virtual	void					EndGroup();

		virtual	void					PushState();
		virtual	void					PopState();

		virtual	void					BeginPixelBlock(const BPixelDescription& pixels);
		virtual	void					PlacePixels(const BRasterPoint& at, const BPixelData& pixels);
		virtual	void					EndPixelBlock();

		virtual	void					DisplayVisual(	const atom_ptr<IVisual>& view,
														uint32 flags, int32 cache_id);

		virtual void					Rect(const BRect& r);


		/*! Use for debugging only. won't be supported in release builds.
			Copies the BPixelData onto the canvas at B_ORIGIN.
		*/
				void					BlitBits(const BPixelData& pixels);

	protected:	
		virtual	const void *			SetState(const void *state);
		virtual	const void *			CopyState();
		virtual	const void *			ReferenceState();
		virtual	void 					DestroyState(const void *state);

	private:
				struct opaque_state;
				struct group;
				struct path_state;
				struct render_state;
				struct cache_state;
				friend struct mgl_state;
				
				void					Init(const BPixelData& onto);
				void					ClearPath();
				void					Build();
				void					Rasterize();
				void					RasterizePoly();
				void					DeferredXForm(bool dump = false);
				void					PreparePrimitive(uint8 shading, bool alpha);
				void					CompositionsChanged();
				group *					find_geometric();

				void *					m_mglState;
				render_state *			m_state;
				BFontCache				m_fontCache;
				cache_state	*			m_bitsCache;
				BPixelData				m_onto;
				uint32					m_isDrawingText;
				B2dTransform			m_priorText2dXform;
};

/**************************************************************************************/

} } // namespace B::Render2

#endif	// _RENDER2_CANVAS_H_
