#ifndef _B_INTERFACE2_CHOICE_CONTROLLER_H_
#define _B_INTERFACE2_CHOICE_CONTROLLER_H_

#include <support2/Binder.h>
#include <interface2/Control.h>
#include <support2/Value.h>
#include <support2/KeyedVector.h>

namespace B {
namespace Interface2 {

using namespace B::Support2;

class BChoiceController : public BBinder
{
public:
	B_STANDARD_ATOM_TYPEDEFS(BChoiceController)
	
				BChoiceController();
	virtual 	~BChoiceController();
	
	status_t			Add(			BControl::sptr_ref control,
										const BValue &key);
	
	status_t			Remove(			BControl::sptr_ref control);
	
private:
	virtual	status_t	HandleEffect(	const BValue &in,
										const BValue &inBindings,
										const BValue &outBindings,
										BValue *out);
	
	status_t put_value(const BValue& value);
	status_t put_new_selection(const BValue& which);
	
	BKeyedVector<BValue, BControl::sptr>	m_list;
};


} }	// namespace B::Interface2

#endif // _B_INTERFACE2_CHOICE_CONTROLLER_H_
