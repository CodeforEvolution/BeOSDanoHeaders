
#ifndef _INTERFACE2_VIEWBAGGROUP_H_
#define _INTERFACE2_VIEWBAGGROUP_H_

#include <interface2/ViewCellLayout.h>

namespace B {
namespace Render2 { class BUpdate; }
namespace Interface2 {

/**************************************************************************************/

class BViewBagGroup : public BViewCellLayout
{
	public:

										BViewBagGroup(const BValue &attr = BValue::undefined);
										BViewBagGroup(const BViewBagGroup &copyFrom);
		virtual							~BViewBagGroup();

		virtual	BView *					Copy();

		virtual	BLayoutConstraints		InternalConstraints() const;
		virtual	void					Layout();

	private:
	
		static	void					sum_maximums(const BVector<BLayoutConstraint> &file_max,
										             const dimth &before,
										             const dimth &intra,
										             const dimth &after,
										             hv_sums *out_sums);

		static	void					find_max_sum(const BVector<hv_sums> &sum,
										             coord *out_min,
										             coord *out_pref,
										             coord *out_max);
		
		mutable int32				m_cells[2];

};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_VIEWBAGGROUP_H_ */
