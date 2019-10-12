#ifndef _SUPPORT2_ITEAM_H
#define _SUPPORT2_ITEAM_H

#include <support2/IInterface.h>
#include <support2/Binder.h>
#include <support2/IContext.h>

namespace B {
namespace Support2 {

//!	Abstract interface to a running team.
class ITeam : public IInterface
{
public:
	B_DECLARE_META_INTERFACE(Team);
	
	//! Create a new component in this team, and return a pointer to it.
	/*!	\param	path	Filesystem location of the addon this component lives in.
		\param	name	Name of the component in its addon (passed to
						BRoot::InstantiateComponent().)
		\param	context	Context for this component to run in.
		\note The normal way to instantiate components is with the
		InstantiateComponent() method in your BBinder's Context().
	*/
	virtual IBinder::ptr InstantiateComponent(	IContext::sptr_ref context,
												const BValue &componentInfo,
												const BString &component) = 0;
	static BValue key_InstantiateComponent;
};

class LTeam : public LInterface<ITeam>
{
public:
	inline					LTeam() : LInterface<ITeam>(IContext::ptr()) { }
	
protected:
	virtual	status_t		HandleEffect(const BValue &in, const BValue &inBindings, const BValue &outBindings, BValue *out);

	inline virtual			~LTeam() { }
};

} } // namespace B::Support2

#endif // _SUPPORT2_ITEAM_H
