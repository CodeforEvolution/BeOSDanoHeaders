
#ifndef _INTERFACE2_VIEWPROXY_H_
#define _INTERFACE2_VIEWPROXY_H_

#include <interface2/ViewParent.h>
#include <interface2/ParentedView.h>
#include <support2/Locker.h>

namespace B {
namespace Interface2 {

/**************************************************************************************/

class BViewProxy : public BViewParent
{
	public:

										BViewProxy(IView::sptr_ref proxyFor);
		virtual							~BViewProxy();

		virtual	bool					SetTransform(const B2dTransform& transform);
		virtual bool					SetFrame(const BRect& bounds);
		virtual	bool					SetExternalConstraints(const BLayoutConstraints &c);
		virtual	void					Hide();
		virtual	void					Show();

		virtual	status_t				InvalidateChildConstraints(const atom_ptr<IView> &child);

		virtual	void					PreTraversal();
		virtual	IView::ptr				PostTraversal(BUpdate &outDirty);

		virtual	void					Display(const IRender::ptr& into);
		virtual	void					Draw(const IRender::ptr& into);
		virtual	void					DispatchEvent(	const BMessage &msg,
														const BPoint& where,
														event_dispatch_result *result = NULL);

		virtual	BLayoutConstraints		Constraints() const;
		virtual	BRegion					Shape() const;
		virtual BRect					Frame() const;
		virtual	B2dTransform			Transform() const;
		virtual	bool					IsHidden() const;

	protected:

		virtual	status_t				Acquired(const void* id);
		virtual	status_t				Released(const void* id);
	
				IView::sptr				Child() const;

	private:

				IView::sptr				m_child;
		mutable	BNestedLocker			m_childLock;
};

/**************************************************************************************/

} } // namespace B::Interface2

#endif	/* _INTERFACE2_VIEWPROXY_H_ */
