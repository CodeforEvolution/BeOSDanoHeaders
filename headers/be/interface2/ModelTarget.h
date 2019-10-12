#ifndef _INTERFACE2_MODELTARGET_H_
#define _INTERFACE2_MODELTARGET_H_

#include <interface2/Model.h>
#include <support2/Binder.h>

namespace B {
namespace Interface2 {

class BModel;

using namespace Support2;

//!	Abstract base class for receiving pushes from an IModel.
class BModelTarget : public BBinder
{
	public:
		B_STANDARD_ATOM_TYPEDEFS(BModelTarget)
		
							BModelTarget(	const IContext::ptr& context,
											const IModel::ptr& model);
							BModelTarget(	const BModelTarget &copyFrom);
							
		IModel::ptr			Model() const;

	protected:
		virtual				~BModelTarget();
		
		virtual	status_t	Acquired(const void* id);
		virtual	status_t	Released(const void* id);
	
		//!	Override this function to return the descriptor of the
		//!	interface you would like this BModelTarget linked to.
		/*! \note You can return a set of descriptors to link to multiple
			interfaces in the source model.
		*/
		virtual	BValue		ModelInterfaceDescriptors() const = 0;
		
		//!	This function gets called in response to an Effect() on the inherited binder.
		/*!	\note You should override this method, NOT the base HandleEffect().
		*/
		virtual	status_t	HandleModelEffect(	const BValue &in,
												const BValue &inBindings,
												const BValue &outBindings,
												BValue *out) = 0;
		
		//!	Redirect BBinder::HandleEffect() to our new HandleModelEffect().
		virtual	status_t	HandleEffect(	const BValue &in,
											const BValue &inBindings,
											const BValue &outBindings,
											BValue *out);

	private:
		IModel::ptr			m_model;
};

} } // namespace B::Interface2

#endif /* _INTERFACE2_MODELTARGET_H_ */
