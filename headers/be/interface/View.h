/*******************************************************************************
/
/	File:			View.h
/
/   Description:    BView is the base class for all views (clipped regions
/                   within a window).
/
/	Copyright 1992-98, Be Incorporated, All Rights Reserved
/
*******************************************************************************/

#ifndef	_VIEW_H
#define	_VIEW_H

#include <BeBuild.h>
#include <InterfaceDefs.h>
#include <Font.h>
#include <Handler.h>
#include <Rect.h>
#include <String.h>

/*----------------------------------------------------------------*/
/*----- view definitions -----------------------------------------*/

enum {
	B_FIRST_MOUSE_BUTTON = 0x01,
	B_SECOND_MOUSE_BUTTON = 0x02,
	B_THIRD_MOUSE_BUTTON = 0x04,
	
	// or if you prefer...
	B_MOUSE_BUTTON_1 = B_FIRST_MOUSE_BUTTON,
	B_MOUSE_BUTTON_2 = B_SECOND_MOUSE_BUTTON,
	B_MOUSE_BUTTON_3 = B_THIRD_MOUSE_BUTTON
};

enum {
	B_ENTERED_VIEW = 0,
	B_INSIDE_VIEW,
	B_EXITED_VIEW,
	B_OUTSIDE_VIEW
};

enum {
	B_UPDATE_INVALIDATED	= 0x00000001,
	B_UPDATE_SCROLLED		= 0x00000002,
	B_UPDATE_RESIZED		= 0x00000004,
	B_UPDATE_EXPOSED		= 0x00000008
};

enum {
	B_NO_EVENTS				= 0x00000000,
	B_POINTER_EVENTS		= 0x00000001,
	B_KEYBOARD_EVENTS		= 0x00000002
};

enum {
	B_LOCK_WINDOW_FOCUS		= 0x00000001,
	B_SUSPEND_VIEW_FOCUS	= 0x00000002,
	B_NO_POINTER_HISTORY	= 0x00000004
};

enum {
	B_TRACK_WHOLE_RECT,
	B_TRACK_RECT_CORNER
};

const uint32 B_FULL_UPDATE_ON_RESIZE 	= 0x80000000UL;	/* 31 */
const uint32 _B_RESERVED1_ 				= 0x40000000UL;	/* 30 */
const uint32 B_WILL_DRAW 				= 0x20000000UL;	/* 29 */
const uint32 B_PULSE_NEEDED 			= 0x10000000UL;	/* 28 */
const uint32 B_NAVIGABLE_JUMP 			= 0x08000000UL;	/* 27 */
const uint32 B_FRAME_EVENTS				= 0x04000000UL;	/* 26 */
const uint32 B_NAVIGABLE 				= 0x02000000UL;	/* 25 */
const uint32 B_SUBPIXEL_PRECISE 		= 0x01000000UL;	/* 24 */
const uint32 B_DRAW_ON_CHILDREN 		= 0x00800000UL;	/* 23 */
const uint32 B_INPUT_METHOD_AWARE 		= 0x00400000UL;	/* 22 */
const uint32 _B_RESERVED7_ 				= 0x00200000UL;	/* 21 */
const uint32 B_OVERLAPPING_CHILDREN		= 0x00100000UL;	/* 20 */

#define _RESIZE_MASK_ ~(B_FULL_UPDATE_ON_RESIZE|_B_RESERVED1_|B_WILL_DRAW|\
		 	B_PULSE_NEEDED|B_NAVIGABLE_JUMP|B_FRAME_EVENTS|B_NAVIGABLE|\
			B_SUBPIXEL_PRECISE|B_DRAW_ON_CHILDREN|B_INPUT_METHOD_AWARE|_B_RESERVED7_|B_OVERLAPPING_CHILDREN)

const uint32 _VIEW_TOP_ 	= 1UL;
const uint32 _VIEW_LEFT_ 	= 2UL;
const uint32 _VIEW_BOTTOM_ 	= 3UL;
const uint32 _VIEW_RIGHT_ 	= 4UL;
const uint32 _VIEW_CENTER_ 	= 5UL;

inline uint32 _rule_(uint32 r1, uint32 r2, uint32 r3, uint32 r4)
	{ return ((r1 << 12) | (r2 << 8) | (r3 << 4) | r4); }

#define B_FOLLOW_NONE 0
#define B_FOLLOW_ALL_SIDES		_rule_(_VIEW_TOP_, _VIEW_LEFT_, _VIEW_BOTTOM_,\
										_VIEW_RIGHT_)
#define B_FOLLOW_ALL  			B_FOLLOW_ALL_SIDES

#define B_FOLLOW_LEFT			_rule_(0, _VIEW_LEFT_, 0, _VIEW_LEFT_)
#define B_FOLLOW_RIGHT			_rule_(0, _VIEW_RIGHT_, 0, _VIEW_RIGHT_)
#define B_FOLLOW_LEFT_RIGHT		_rule_(0, _VIEW_LEFT_, 0, _VIEW_RIGHT_)
#define B_FOLLOW_H_CENTER		_rule_(0, _VIEW_CENTER_, 0, _VIEW_CENTER_)

#define B_FOLLOW_TOP			_rule_(_VIEW_TOP_, 0, _VIEW_TOP_, 0)
#define B_FOLLOW_BOTTOM			_rule_(_VIEW_BOTTOM_, 0, _VIEW_BOTTOM_, 0)
#define B_FOLLOW_TOP_BOTTOM		_rule_(_VIEW_TOP_, 0, _VIEW_BOTTOM_, 0)
#define B_FOLLOW_V_CENTER		_rule_(_VIEW_CENTER_, 0, _VIEW_CENTER_, 0)

class BBitmap;
class BCursor;
class BMessage;
class BPicture;
class BPolygon;
class BRegion;
class BScrollBar;
class BScrollView;
class BShape;
class BShelf;
class BString;
class BToolTipInfo;
class BTransform2d;
class BWindow;
struct _view_attr_;
struct _array_data_;
struct _array_hdr_;
struct overlay_restrictions;

/*----------------------------------------------------------------*/
/*----- BView class ----------------------------------------------*/

class BView : public BHandler {

public:
						BView(	BRect frame,
								const char *name,
								uint32 resizeMask,
								uint32 flags);
virtual					~BView();

						BView(BMessage *data);
static	BArchivable		*Instantiate(BMessage *data);
virtual	status_t		Archive(BMessage *data, bool deep = true) const;

		void			SetToolTipText(const char* text);
		const char*		ToolTipText() const;
	
virtual	void			AttachedToWindow();
virtual	void			AllAttached();
virtual	void			DetachedFromWindow();
virtual	void			AllDetached();

virtual	void			MessageReceived(BMessage *msg);

		void			AddChild(BView *child, BView *before = NULL);
		bool			RemoveChild(BView *child);
		int32			CountChildren() const;
		BView			*ChildAt(int32 index) const;
		BView			*NextSibling() const;
		BView			*PreviousSibling() const;
		bool			RemoveSelf();

		BWindow			*Window() const;

		uint32			GetUpdateHints();

virtual	void			Draw(BRect updateRect);
virtual	void			MouseDown(BPoint where);
virtual	void			MouseUp(BPoint where);
virtual	void			MouseMoved(	BPoint where,
									uint32 code,
									const BMessage *a_message);
virtual	void			WindowActivated(bool state);
virtual	void			KeyDown(const char *bytes, int32 numBytes);
virtual	void			KeyUp(const char *bytes, int32 numBytes);
virtual	void			Pulse();
virtual	void			FrameMoved(BPoint new_position);
virtual	void			FrameResized(float new_width, float new_height);

virtual	void			TargetedByScrollView(BScrollView *scroll_view);
		void			BeginRectTracking(	BRect startRect,
											uint32 style = B_TRACK_WHOLE_RECT);
		void			EndRectTracking();
	
		void			GetMouse(	BPoint* location,
									uint32 *buttons,
									bool checkMessageQueue = true);

		void			DragMessage(BMessage *aMessage,
									BRect dragRect,
									BHandler *reply_to = NULL);
		void			DragMessage(BMessage *aMessage,
									BBitmap *anImage,
									BPoint offset,
									BHandler *reply_to = NULL);
		void			DragMessage(BMessage *aMessage,
									BBitmap *anImage,
									drawing_mode dragMode,
									BPoint offset,
									BHandler *reply_to = NULL);

		BView			*FindView(const char *name) const;
		BView			*Parent() const;
		BRect			Bounds() const;
		BRect			Frame() const;
		void			ConvertToScreen(BPoint* pt) const;
		BPoint			ConvertToScreen(BPoint pt) const;
		void			ConvertFromScreen(BPoint* pt) const;
		BPoint			ConvertFromScreen(BPoint pt) const;
		void			ConvertToScreen(BRect *r) const;
		BRect			ConvertToScreen(BRect r) const;
		void			ConvertFromScreen(BRect *r) const;
		BRect			ConvertFromScreen(BRect r) const;
		void			ConvertToParent(BPoint *pt) const;
		BPoint			ConvertToParent(BPoint pt) const;
		void			ConvertFromParent(BPoint *pt) const;
		BPoint			ConvertFromParent(BPoint pt) const;
		void			ConvertToParent(BRect *r) const;
		BRect			ConvertToParent(BRect r) const;
		void			ConvertFromParent(BRect *r) const;
		BRect			ConvertFromParent(BRect r) const;
		BPoint			LeftTop() const;

		void			GetClippingRegion(BRegion *region) const;
virtual	void			ConstrainClippingRegion(BRegion *region);
		void			ClipToPicture(	BPicture *picture,
										BPoint where = B_ORIGIN,
										bool sync = true);
		void			ClipToInversePicture(	BPicture *picture,
												BPoint where = B_ORIGIN,
												bool sync = true);

		uint32			DoubleBuffering();
		void			SetDoubleBuffering(uint32 updateTypes);

virtual	void			SetDrawingMode(drawing_mode mode);
		drawing_mode 	DrawingMode() const;

		void			SetBlendingMode(source_alpha srcAlpha, alpha_function alphaFunc);
		void	 		GetBlendingMode(source_alpha *srcAlpha, alpha_function *alphaFunc) const;

virtual	void			SetPenSize(float size);
		float			PenSize() const;

		// Setting view colors as abstract UI colors.  The view will automatically
		// update the underlying color if the user interface settings change.
		// NOTE WELL: These strings are -not- copied.  They must remain valid
		// for the life of the view.
		void			SetViewUIColor(const char* name);
		void			SetLowUIColor(const char* name);
		void			SetHighUIColor(const char* name);
		const char*		ViewUIColor() const;
		const char*		LowUIColor() const;
		const char*		HighUIColor() const;
		
		// A convenience function to copy your view and low colors from your
		// parent.  You'll typically call this in AttachedToWindow() and
		// possibly UISettingsChanged();
		void			SetColorsFromParent();
		
		void			SetViewCursor(const BCursor *cursor, bool sync=true);

virtual	void			SetViewColor(rgb_color c);
		void			SetViewColor(uchar r, uchar g, uchar b, uchar a = 255);
		rgb_color		ViewColor() const;

		void			SetViewBitmap(	const BBitmap *bitmap,
										BRect srcRect, BRect dstRect,
										uint32 followFlags=B_FOLLOW_TOP|B_FOLLOW_LEFT,
										uint32 options=B_TILE_BITMAP);
		void			SetViewBitmap(	const BBitmap *bitmap,
										uint32 followFlags=B_FOLLOW_TOP|B_FOLLOW_LEFT,
										uint32 options=B_TILE_BITMAP);
		void			ClearViewBitmap();

		status_t		SetViewOverlay(	const BBitmap *overlay,
										BRect srcRect, BRect dstRect,
										rgb_color *colorKey,
										uint32 followFlags=B_FOLLOW_TOP|B_FOLLOW_LEFT,
										uint32 options=0);
		status_t		SetViewOverlay(	const BBitmap *overlay, rgb_color *colorKey,
										uint32 followFlags=B_FOLLOW_TOP|B_FOLLOW_LEFT,
										uint32 options=0);
		void			ClearViewOverlay();

		rgb_color		NextFocusColor() const;
		bigtime_t		NextFocusTime() const;
		
virtual	void			SetHighColor(rgb_color a_color);
		void			SetHighColor(uchar r, uchar g, uchar b, uchar a = 255);
		rgb_color		HighColor() const;

virtual	void			SetLowColor(rgb_color a_color);
		void			SetLowColor(uchar r, uchar g, uchar b, uchar a = 255);
		rgb_color		LowColor() const;

		void			SetLineMode(	cap_mode lineCap,
										join_mode lineJoin,
										float miterLimit=B_DEFAULT_MITER_LIMIT);
		join_mode		LineJoinMode() const;
		cap_mode		LineCapMode() const;
		float			LineMiterLimit() const;

		void			SetTransform(const BTransform2d& transform);
		void			GetTransform(BTransform2d* target) const;

		void			SetOrigin(BPoint pt);
		void			SetOrigin(float x, float y);
		BPoint			Origin() const;

		void			PushState();
		void			PopState();

		void			MovePenTo(BPoint pt);
		void			MovePenTo(float x, float y);
		void			MovePenBy(float x, float y);
		BPoint			PenLocation() const;
		void			StrokeLine(	BPoint toPt,
									pattern p = B_SOLID_HIGH);
		void			StrokeLine(	BPoint pt0,
									BPoint pt1,
									pattern p = B_SOLID_HIGH);
		void			BeginLineArray(int32 count);
		void			AddLine(BPoint pt0, BPoint pt1, rgb_color col);
		void			EndLineArray();
	
		void			StrokePolygon(	const BPolygon *aPolygon,
									    bool  closed = true,
										pattern p = B_SOLID_HIGH);
		void			StrokePolygon(	const BPoint *ptArray,
										int32 numPts,
									    bool  closed = true,
										pattern p = B_SOLID_HIGH);
		void			StrokePolygon(	const BPoint *ptArray,
										int32 numPts,
										BRect bounds,
									    bool  closed = true,
										pattern p = B_SOLID_HIGH);
		void			FillPolygon(const BPolygon *aPolygon,
									pattern p = B_SOLID_HIGH);
		void			FillPolygon(const BPoint *ptArray,
									int32 numPts,
									pattern p = B_SOLID_HIGH);
		void			FillPolygon(const BPoint *ptArray,
									int32 numPts,
									BRect bounds,
									pattern p = B_SOLID_HIGH);
	
		void			StrokeTriangle(	BPoint pt1,
										BPoint pt2,
										BPoint pt3,
										BRect bounds,
										pattern p = B_SOLID_HIGH);
		void			StrokeTriangle(	BPoint pt1,
										BPoint pt2,
										BPoint pt3,
										pattern p = B_SOLID_HIGH);
		void			FillTriangle(	BPoint pt1,
										BPoint pt2,
										BPoint pt3,
										pattern p = B_SOLID_HIGH);
		void			FillTriangle(	BPoint pt1,
										BPoint pt2,
										BPoint pt3,
										BRect bounds,
										pattern p = B_SOLID_HIGH);

		void			StrokeRect(BRect r, pattern p = B_SOLID_HIGH);
		void			FillRect(BRect r, pattern p = B_SOLID_HIGH);
		void			FillRegion(BRegion *a_region, pattern p= B_SOLID_HIGH);
		void			InvertRect(BRect r);

		void			StrokeRoundRect(BRect r,
										float xRadius,
										float yRadius,
										pattern p = B_SOLID_HIGH);
		void			FillRoundRect(	BRect r,
										float xRadius,
										float yRadius,
										pattern p = B_SOLID_HIGH);

		void			StrokeEllipse(	BPoint center,
										float xRadius,
										float yRadius,
										pattern p = B_SOLID_HIGH);
		void			StrokeEllipse(BRect r, pattern p = B_SOLID_HIGH);
		void			FillEllipse(BPoint center,
									float xRadius,
									float yRadius,
									pattern p = B_SOLID_HIGH);
		void			FillEllipse(BRect r, pattern p = B_SOLID_HIGH);
				
		void			StrokeArc(	BPoint center,
									float xRadius,
									float yRadius,
									float start_angle,
									float arc_angle,
									pattern p = B_SOLID_HIGH);
		void			StrokeArc(	BRect r,
									float start_angle,
									float arc_angle,
									pattern p = B_SOLID_HIGH);
		void			FillArc(BPoint center,
								float xRadius,
								float yRadius,
								float start_angle,
								float arc_angle,
								pattern p = B_SOLID_HIGH);
		void			FillArc(BRect r,
								float start_angle,
								float arc_angle,
								pattern p = B_SOLID_HIGH);

		void			StrokeBezier(	BPoint *controlPoints,
										pattern p = B_SOLID_HIGH);
		void			FillBezier(		BPoint *controlPoints,
										pattern p = B_SOLID_HIGH);

		void			StrokeShape(	BShape *shape,
										pattern p = B_SOLID_HIGH);
		void			FillShape(		BShape *shape,
										pattern p = B_SOLID_HIGH);
			
		void			CopyBits(BRect src, BRect dst);
		void			DrawBitmapAsync(const BBitmap *aBitmap,
										BRect srcRect,
										BRect dstRect);
		void			DrawBitmapAsync(const BBitmap *aBitmap);
		void			DrawBitmapAsync(const BBitmap *aBitmap, BPoint where);
		void			DrawBitmapAsync(const BBitmap *aBitmap, BRect dstRect);
		void			DrawBitmap(	const BBitmap *aBitmap,
									BRect srcRect,
									BRect dstRect);
		void			DrawBitmap(const BBitmap *aBitmap);
		void			DrawBitmap(const BBitmap *aBitmap, BPoint where);
		void			DrawBitmap(const BBitmap *aBitmap, BRect dstRect);

		void			DrawChar(char aChar);
		void			DrawChar(char aChar, BPoint location);
		void			DrawString(const char *aString,
								   escapement_delta *delta = NULL);
		void			DrawString(const char *aString, BPoint location,
								   escapement_delta *delta = NULL);
		void			DrawString(const char *aString, int32 length,
								   escapement_delta *delta = NULL);
		void			DrawString(const char *aString,
								   int32 length,
								   BPoint location,
								   escapement_delta *delta = 0L);

virtual void            SetFont(const BFont *font, uint32 mask = B_FONT_ALL);

#if !_PR3_COMPATIBLE_
		void            GetFont(BFont *font) const;
#else
		void            GetFont(BFont *font);
#endif
		void			TruncateString(BString* in_out,
									   uint32 mode,
									   float width) const;
		float			StringWidth(const char *string) const;
		float			StringWidth(const char *string, int32 length) const;
		void			GetStringWidths(char *stringArray[],
										int32 lengthArray[],
										int32 numStrings,
										float widthArray[]) const;
		void			SetFontSize(float size);
		void			ForceFontFlags(uint32 flags);
		void			ForceFontAliasing(bool enable);	// a.k.a. ForceFontFlags(B_DISABLE_ANTIALIASING)
		void			GetFontHeight(font_height *height) const;
	
		void			Invalidate(BRect invalRect);
		void			Invalidate(const BRegion *invalRegion);
		void			Invalidate();

		void			DelayedInvalidate(bigtime_t delay, BRect invalRect=BRect());
		void			InvalidateAtTime(bigtime_t time, BRect invalRect=BRect());
		bool			IsInvalidatePending() const;
		
		void			SetDiskMode(char *filename, long offset);

		void			BeginPicture(BPicture *a_picture);
		void			AppendToPicture(BPicture *a_picture);
		BPicture		*EndPicture();

		void			DrawPicture(const BPicture *a_picture);
		void			DrawPicture(const BPicture *a_picture, BPoint where);
		void			DrawPicture(const char *filename, long offset, BPoint where);
		void			DrawPictureAsync(const BPicture *a_picture);
		void			DrawPictureAsync(const BPicture *a_picture, BPoint where);
		void			DrawPictureAsync(const char *filename, long offset, BPoint where);

		status_t		SetEventMask(uint32 mask, uint32 options=0);
		uint32			EventMask();
		status_t		SetMouseEventMask(uint32 mask, uint32 options=0);

virtual	void			SetFlags(uint32 flags);
		uint32			Flags() const;
virtual	void			SetResizingMode(uint32 mode);
		uint32			ResizingMode() const;
		void			MoveBy(float dh, float dv);
		void			MoveTo(BPoint where);
		void			MoveTo(float x, float y);
		void			ResizeBy(float dh, float dv);
		void			ResizeTo(float width, float height);
		void			ScrollBy(float dh, float dv);
		void			ScrollTo(float x, float y);
virtual	void			ScrollTo(BPoint where);

virtual	void			MakeFocus(bool focusState = true);
		void			MakeFocusNoScroll(bool focusState = true);
		bool			IsFocus() const;
		bool			IsNavigating() const;
		
		// After your view has focus, you can set the "explicit" state.
		// Having explicit focus means that the user has directly requested
		// focus, so nobody else should take focus away from you unless
		// also explicitly directed by the user.
		void			SetExplicitFocus(bool explicitState = true);
		bool			IsExplicitFocus() const;
		
virtual	void			Show();
virtual	void			Hide();
		bool			IsHidden() const;
		bool			IsHidden(const BView* looking_from) const;
	
		void			Flush() const;
		void			Sync() const;

virtual	void			GetPreferredSize(float *width, float *height);
virtual	void			ResizeToPreferred();

		BScrollBar		*ScrollBar(orientation posture) const;

virtual BHandler		*ResolveSpecifier(BMessage *msg,
										int32 index,
										BMessage *specifier,
										int32 form,
										const char *property);
virtual status_t		GetSupportedSuites(BMessage *data);

		bool			IsPrinting() const;
		void			SetScale(float scale) const;

/*----- Private or reserved -----------------------------------------*/
virtual status_t		Perform(perform_code d, void *arg);

virtual	void			DrawAfterChildren(BRect r);

	// This method is called in two sitations --
	// * To figure out how the pointer is moving around tipable
	//   objects, GetToolTipInfo() will be called with outInfo
	//   set to NULL.  In this case, you should just quickly return
	//   the rectangle corresponding to the tipable area containing
	//   the point 'screenWhere'.  This point is in screen
	//   coordinates, so you will probably need to convert it to
	//   your view's coordinates; outViewRegion is in your view's
	//   coordinates.
	// * To actually display a tool tip, GetToolTipInfo() is called
	//   with a non-zero outInfo object.  You should then fill in
	//   the object with the information to display.  Any
	//   coordinates you supply to outInfo are relative to
	//   outViewRegion.
	//
	// The default implementation of this simply returns the view's
	// frame rectangle (offset to (0,0)) as the region, and plugs
	// the current ToolTipText() into outInfo.
	
virtual	status_t		GetToolTipInfo(	BPoint screenWhere,
										BRect* outViewRegion,
										BToolTipInfo* outInfo = NULL);

	// Cause your view's GetToolTipInfo() to be called (if your tip
	// is currently being shown), to change what is being displayed.
	
		void			RefreshToolTipInfo() const;

virtual	status_t		UISettingsChanged(const BMessage* changes, uint32 flags);

private:

friend class BScrollBar;
friend class BWindow;
friend class BBitmap;
friend class BPrintJob;
friend class BShelf;
friend class BTabView;

virtual	void			_ReservedView4();
virtual	void			_ReservedView5();
virtual	void			_ReservedView6();
virtual	void			_ReservedView7();
virtual	void			_ReservedView8();

#if !_PR3_COMPATIBLE_
virtual	void			_ReservedView9();
virtual	void			_ReservedView10();
virtual	void			_ReservedView11();
virtual	void			_ReservedView12();
virtual	void			_ReservedView13();
virtual	void			_ReservedView14();
virtual	void			_ReservedView15();
virtual	void			_ReservedView16();
#endif

					BView(const BView &);
		BView		&operator=(const BView &);

		void		InitData(BRect f, const char *name, uint32 rs, uint32 fl);
		status_t	ArchiveChildren(BMessage *data, bool deep) const;
		status_t	UnarchiveChildren(BMessage *data, BWindow *w = NULL);
		status_t	SetViewImage(const BBitmap *bitmap,BRect srcRect, BRect dstRect, uint32 followFlags, uint32 options);
		void		BeginPicture_pr(BPicture *a_picture, BRect r);
		void		SetPattern(pattern pat);
		void		DoBezier(int32 gr, BPoint *controlPoints, pattern p);
		void		DoShape(int32 gr, BShape *shape, pattern p);
		void		DoPictureClip(BPicture *picture, BPoint where, bool invert, bool sync);
		bool		remove_from_list(BView *a_view);
		bool		remove_self();
		bool		do_owner_check() const;
		void		set_owner(BWindow *the_owner);
		void		do_activate(int32 state);
		void		check_lock() const;
		void		check_lock_no_pick() const;
		void		movesize(uint32 code, int32 h, int32 v);
		void		handle_tick();
		void		handle_ui_settings(const BMessage* changes, uint32 flags);
		char		*test_area(int32 length);
		void		remove_comm_array();
		_array_hdr_	*new_comm_array(int32 cnt);
		BView		*RealParent() const;
		void		SetScroller(BScrollBar *sb);
		void		UnsetScroller(BScrollBar *sb);
		void		RealScrollTo(BPoint);
		void		init_cache();
		void		set_cached_state();
		void		update_cached_state();
		void        set_font_state(const BFont *font, uint32 mask);
		void		fetch_font();
		uchar		font_encoding() const;
		BShelf		*shelf() const;
		void		set_shelf(BShelf *);
		void		refresh_colors(const BMessage& settings);
		
		int32			server_token;
		uint32			f_type;
		float			origin_h;
		float			origin_v;
		BWindow*		owner;
		BView*			parent;
		BView*			next_sibling;
		BView*			prev_sibling;
		BView*			first_child;

		int16 			fShowLevel;
		bool			top_level_view;
		bool			fNoISInteraction;
		BPicture		*cpicture;
		_array_data_	*comm;

		BScrollBar		*fVerScroller;
		BScrollBar		*fHorScroller;
		bool			f_is_printing;
		bool			attached;
		bool			fFocusNoScroll;
		bool			fExplicitFocus;
		_view_attr_		*fPermanentState;
		_view_attr_		*fState;
		BRect			fCachedBounds;
		BShelf			*fShelf;
		void			*pr_state;
		uint32			fEventMask;
		uint32			fEventOptions;
		uint32			m_doubleBuffering;
		BString			fToolTipText;
mutable	bool			fPulseDir;
mutable	uint8			fPulseValue;
		uint16			fStateLevel;
		uint32			_reserved2;
#if !_PR3_COMPATIBLE_
		uint32			_more_reserved[3];
#endif
};


/*----------------------------------------------------------------*/
/*----- inline definitions ---------------------------------------*/

inline void	BView::ScrollTo(float x, float y)
	{ ScrollTo(BPoint(x, y)); }

inline void	BView::SetViewColor(uchar r, uchar g, uchar b, uchar a)
{
	rgb_color	a_color;
	a_color.red = r;		a_color.green = g;
	a_color.blue = b;		a_color.alpha = a;
	SetViewColor(a_color);
}

inline void	BView::SetHighColor(uchar r, uchar g, uchar b, uchar a)
{
	rgb_color	a_color;
	a_color.red = r;		a_color.green = g;
	a_color.blue = b;		a_color.alpha = a;
	SetHighColor(a_color);
}

inline void	BView::SetLowColor(uchar r, uchar g, uchar b, uchar a)
{
	rgb_color	a_color;
	a_color.red = r;		a_color.green = g;
	a_color.blue = b;		a_color.alpha = a;
	SetLowColor(a_color);
}

/*----------------------------------------------------------------*/
/*----- backwards compatibility ----------------------------------*/

enum {
	B_PRIMARY_MOUSE_BUTTON = B_FIRST_MOUSE_BUTTON,
	B_SECONDARY_MOUSE_BUTTON = B_SECOND_MOUSE_BUTTON,
	B_TERTIARY_MOUSE_BUTTON = B_THIRD_MOUSE_BUTTON
};

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

#endif /* _VIEW_H */
