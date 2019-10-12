
#ifndef _INTERFACE2_VIEWHVGROUP_H_
#define _INTERFACE2_VIEWHVGROUP_H_

#include <interface2/ViewCellLayout.h>

namespace B {
namespace Render2 { class BUpdate; }
namespace Interface2 {

/**************************************************************************************/

class BViewHVGroup : public BViewCellLayout
{
	public:

										BViewHVGroup(const BValue &attr = BValue::undefined);
										BViewHVGroup(const BViewHVGroup &copyFrom);
		virtual							~BViewHVGroup();

		virtual	BView *					Copy();

		virtual	BLayoutConstraints		InternalConstraints() const;
		virtual	void					Layout();

	private:
	
				orientation			m_orientation;

};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_VIEWHVGROUP_H_ */
