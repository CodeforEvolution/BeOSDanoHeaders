/***************************************************************************
//
//	File:			render2/DRender.h
//
//	Description:	DRender interface.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _RENDER2_DRENDER_H_
#define _RENDER2_DRENDER_H_

#include <support2/StdIO.h>
#include <render2/LRender.h>

namespace B {
namespace Render2 {

using namespace Support2;
using namespace Raster2;

class BFont;

/*!
	DRender is used to debug an IRender or LRender. Use DRender instead, the IRender
	to be debugged is passed as a paramter. DRender keeps a strong reference
	on it.
	DRender will provide usefull informations as the IRender is used. Each
	method called produce a trace on stdout, with a dump of the parameters.
	Invalid parameters will be catched and the debugger will be called.
*/


/**************************************************************************************/

class DRender : public LRender
{
public:
					DRender(	const IRender::sptr& into,
								int32 outLevel = 2,
								const ITextOutput::sptr& out = bout);
		virtual		~DRender();
									

		/*	LRender interface */
		virtual	const void *		SetState(const void *state);
		virtual	void				DestroyState(const void *render_state);
		virtual	const void *		CopyState();
		virtual	const void *		ReferenceState();

		/*	IPath interface */
		virtual	void				MoveTo(const BPoint& pt);
		virtual	void				LinesTo(const BPoint* points, int32 lineCount=1);
		virtual	void				BeziersTo(const BPoint* points, int32 bezierCount=1);
		virtual	void				ArcsTo(const BPoint* points, const coord* radius, int32 arcCount=1);
		virtual	void				Arc(const BPoint& center, const BPoint& radii, float startAngle=0, float arcLen=B_2_PI, bool connected=false);
		virtual	void				Line(const BPoint &p0, const BPoint &p1);
		virtual	void				Rect(const BRect &r);
		virtual	void				RoundRect(const BRect &r, const BPoint& radii);
		virtual	void				Close();

		/* Branch */
		virtual	IRender::ptr		Branch(uint32 flags = B_SYNCHRONOUS);
		virtual void				Sync();
		virtual	void				ApplyFlags(uint32 flags, uint32 which);

		/* Text */
		virtual	void				TextAt(	const BPoint&,
											const char *text,
											int32 len=B_CSTR_LEN,
											const escapements& escapements = B_NO_ESCAPEMENT);

		/*	Composite path operations. */
		virtual	void				Stroke();
		virtual	void				TextOnPath(	const char *text,
												int32 len = B_CSTR_LEN,
												const escapements& escapements = B_NO_ESCAPEMENT,
												uint32 flags = B_JUSTIFY_LEFT|B_VALIGN_BASELINE) ;

		/*	Path generation state */
		virtual	void				SetFont(const BFont &font);
		virtual	void				SetWindingRule(winding_rule rule);
		virtual void				SetPenSize(coord penWidth);
		virtual void				SetCaps(cap_mode start_cap, cap_mode end_cap);
		virtual void				SetJoin(join_mode join, float mitter_limit=10.0f);
		virtual void				SetStippling(const stipple_t *stippling=NULL, int32 offset = 0);

		/*	Path filling */
		virtual void				SetColor(const BColor&);
		virtual void				SetGradient(const BGradient&);
		virtual	void				Fill();
		virtual	void				Paint();

		/*	Transformation stacks */
		virtual	void				Transform(const B2dTransform&);
		virtual	void				Transform(const BColorTransform&);

		/*	Grouping of drawing commands */
		virtual	void				BeginGroup(uint32 flags = 0);
		virtual	void				EndGroup();

		/*	Save/restore state */
		virtual	void				PushState();
		virtual	void				PopState();

		/*	Pixmap/sample support */
		virtual	void				BeginPixelBlock(const BPixelDescription& pixels);
		virtual	void				PlacePixels(const BRasterPoint& at, const BPixelData& pixels);
		virtual	void				EndPixelBlock();

		/*	Drawing an IVisual */
		virtual	void				DisplayVisual(	const atom_ptr<IVisual>& view, uint32 flags,
													int32 cache_id);

private:
		bool validate_points(const BString&, const BPoint*, int32 c=1);
		bool validate_radius(const BString&, const coord*, int32 c=1);
		IRender::sptr		m_render;
		LRender				*m_lrender;
		ITextOutput::sptr	m_out;
		int32				m_outLevel;
		bool				m_print : 1;
		bool				m_deep	: 1;
};

} } // namespace B::Render2

#endif	// _RENDER2_DRENDER_H_
