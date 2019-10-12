
#ifndef _INTERFACE2_VIEWCELLLAYOUT_H_
#define _INTERFACE2_VIEWCELLLAYOUT_H_

#include <interface2/ViewLayout.h>

namespace B {
namespace Render2 { class BUpdate; }
namespace Interface2 {

/**************************************************************************************/

class BViewCellLayout : public BViewLayout
{
	public:

										BViewCellLayout(const BValue &attr = BValue::undefined);
										BViewCellLayout(const BViewCellLayout &copyFrom);
		virtual							~BViewCellLayout();
	 /*!	
		Keys for parameter values to pass to the constructor in the
		\a attr \c BValue.

	 	\li \c B_KEY_ORIENTATION (int32): Orientation of the object --
	 	that is, one of B_HORIZONTAL or B_VERTICAL.
	 	B_HORIZONTAL means the children are placed side-by-side
	 	next to each other; B_VERTICAL places them one on top of
	 	another.

		\li \c B_KEY_INSET_LEFT, B_KEY_INSET_RIGHT, B_KEY_INSET_TOP,
		B_KEY_INSET_BOTTOM:  Space to place on sides, between the
		children and this View's frame.  All defaults are zero.
		If specified as strings, they're interpreted like the values
		given to BView's attr, where norms are relative to \e this
		View's size.  If specified as floats or int32s, they're
		interpreted as absolute pixel values.
		
		\li \c B_KEY_INTRA_SPACE:  The amount of space to put between
		children.  Dimensions are as above, and the default is also
		zero.
	 */

		static const BValue B_KEY_ORIENTATION;
		static const BValue B_KEY_INSET_LEFT;
		static const BValue B_KEY_INSET_RIGHT;
		static const BValue B_KEY_INSET_TOP;
		static const BValue B_KEY_INSET_BOTTOM;
		static const BValue B_KEY_INTRA_SPACE;
		
	/*!	
		Keys for parameter values to pass to the AddView() in the
		\a attr \c BValue.  These affect the layout of the added
		child.
		
		\li \c B_KEY_WEIGHT  The weight of a child, relative to its
		other siblings.  When there is more room available than
		the children take up, this determines how much extra space
		is assigned to each column and row.  The weight of each row
		is equal to the sum of the weights of the children in that
		row, and similarly for columns.  Specified as a string
		(e.g., ".67"), a float, or an int32.  Defaults to 1.
		
		\li \c B_KEY_GRAVITY  Controls the size and placement of a child
		View within the space we allocate to it.
		Defaults to B_CENTER, which means to size the child to its
		preferred size (if there's enough room) and to center it if there
		is extra room for it.
		
		\li \c B_KEY_COLUMN  Which column (zero-based) to lay out this
		child in. This is not used by BViewHVGroup: That class just lays
		out its children in index order, in a row or column based on the
		value you pass to its constructor in attr[B_KEY_ORIENTATION].
		
		\li \c B_KEY_ROW  Which row (zero-based) to lay out this child in.
		This is not used by BViewHVGroup: That class just lays out its
		children in index order, in a row or column based on the value you
		pass to its constructor in attr[B_KEY_ORIENTATION].
		
		Note that there is no collision detection: If you specify the same
		row and column for two different children, they will both go into
		the same cell, and will probably not draw correctly.
	 */
		static const BValue B_KEY_WEIGHT;
		static const BValue B_KEY_GRAVITY;
		static const BValue B_KEY_COLUMN;
		static const BValue B_KEY_ROW;

	/*! Gravity flags.
		
		These flags choose the largest size which a child View may be
		stretched to, and which edges, if any, a View will be "pulled" to.
		As an example of the latter, B_NORTH forces the top edge of the
		child View to the top edge of the space we have allocated to it.
		B_NORTH | B_WEST would force the child to the top left corner.
		If no directional flags are specified, the View will be centered.
		Specifying opposite edges (e.g., B_NORTH | B_SOUTH) will stretch
		the View all the way between those edges, possibly forcing the
		View to be larger than the maximum size it requests with its
		constraints.
		
		If you don't specify opposite edges, then the limiting size that
		the child View will be stretched to is controlled by the "strength"
		of the gravity:
		
		\li Normal gravity: The limiting size is the child View's
		preferred size.  That is, if we have allocated more room to the
		child than its preferred size, it will be stretched to its
		preferred size, but no larger.  It will then be placed within the
		too-big space according to which directional gravity flags have
		been specified for it.
		
		\li Strong gravity: The limiting size is the child View's minimum
		size.
		
		\li Weak gravity: The limiting size is the child View's maximum
		size.
		
		These calculations are done independently for each axis, so you can
		specify different strengths for the vertical (north-south) axis and
		horizontal (east-west) axis.
		
		Given these rules, certain combinations are not meaningful (such
		as B_STRONG_NS | B_WEAK_NS and B_NORTH | B_SOUTH | B_STRONG_NS). The
		combinations of flags defined below are those which are useful for
		each axis. Note that combinations of flags for \e different axes are
		always meaningful (so that B_NORTH | B_SOUTH | B_STRONG_EW is useful).
	 */
		enum {
			B_CENTER				= 0x0000,
			
			//! Directional flags
			B_NORTH					= 0x0001,
			B_SOUTH					= 0x0002,
			B_WEST					= 0x0010,
			B_EAST					= 0x0020,
			
			//! Strength flags
			B_STRONG_NS				= 0x0004,
			B_WEAK_NS				= 0x0008,
			B_STRONG_EW				= 0x0040,
			B_WEAK_EW				= 0x0080,
			
			//! Meaningful combinations for each axis
			B_STRONG_NORTH			= B_NORTH | B_STRONG_NS,
			B_WEAK_NORTH			= B_NORTH | B_WEAK_NS,
			B_STRONG_SOUTH			= B_SOUTH | B_STRONG_NS,
			B_WEAK_SOUTH			= B_SOUTH | B_WEAK_NS,
			B_NORTH_SOUTH			= B_NORTH | B_SOUTH,
			
			B_STRONG_EAST			= B_EAST | B_STRONG_EW,
			B_WEAK_EAST				= B_EAST | B_WEAK_EW,
			B_STRONG_WEST			= B_WEST | B_STRONG_EW,
			B_WEAK_WEST				= B_WEST | B_WEAK_EW,
			B_EAST_WEST				= B_EAST | B_WEST,
		};

		virtual	status_t			AddView(const IView::ptr &child, const BValue &attr = BValue::undefined);
		virtual	status_t			RemoveView(const IView::ptr &child);
		virtual	status_t			InvalidateChildConstraints(const IView::ptr &child);
		virtual	status_t			ReplaceAttributes(int32 i, const BValue &attr);

	protected:
	
		struct hv_sums {
			coord min_px;
			coord pref_px;
			coord max_px;
			coord min_nm;
			coord pref_nm;
			coord max_nm;
			
			hv_sums() : min_px(0), pref_px(0), max_px(0), min_nm(0), pref_nm(0), max_nm(0)
				{ }
		}; 
		
		static 	dimth				get_dimth(const BValue &attr, const BValue &name);
		static	void				add_gutter_to_sum(const dimth &gutter,
									                  coord multiplier,
									                  hv_sums *sum);
		static	inline	coord		safe_scale(coord x, coord numerator, coord denominator)
									{
										return (denominator > 0) ?
												((x*numerator)/denominator) :
												(x > 0 ? x : 0);
									}

		mutable	bool				m_cached_cnst_dirty;
		mutable BLayoutConstraints	m_internal_cnst;

		mutable coord				m_total_weight; // sum of B_KEY_WEIGHTs of all chidren
				dimth				m_inset_before[2];
				dimth				m_inset_after[2];
				dimth				m_intra_s;
};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_VIEWCELLLAYOUT_H_ */
