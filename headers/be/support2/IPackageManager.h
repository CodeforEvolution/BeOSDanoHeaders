#ifndef _IPACKAGE_MANAGER_H
#define _IPACKAGE_MANAGER_H

#include <support2/IInterface.h>

namespace B {
namespace Support2 {

class IPackageManager : public IInterface
{
public:
	B_DECLARE_META_INTERFACE(PackageManager);

	/*!	Returns the real id or "" of a component class.  CURRENTLY UNIMPLEMENTED
	 */
	virtual	BString		DereferenceClass(const BString &id) = 0;
	static	value_clrg	key_DereferenceClass;
	
	/*!	Fills in info with he component info BValue for the given component, keyed
		by vendor, package and component
	 */
	virtual	BValue		GetComponentInfo(const BString &id) = 0;
	static	value_clrg	key_GetComponentInfo;
	
	/*!	Fills in results with a mapping of Component id --> component info BValue for
		all components that support the given interface
	 */
	virtual	BValue		FindComponentsByInterface(const BString &interface) = 0;
	static	value_clrg	key_FindComponentsByInterface;

	//!	Convenience function to divide a component ID into its constituent parts.
	static	status_t	SplitComponentID(	const char* id,
											BString* out_vendor,
											BString* out_package,
											BString* out_component);
	
	//!	Convenience function to build a component ID from its constituent parts.
	static	status_t	JoinComponentID(	const char* vendor,
											const char* package,
											const char* component,
											BString* out_id);
};

} } // namespace B::Support2

#endif // _IPACKAGE_MANAGER_H
