/******************************************************************************
//
//	File:			support2/IContext.h
//
//	Description:	Context associated with each BBinder
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
******************************************************************************/

#ifndef _SUPPORT2_ICONTEXT_H_
#define _SUPPORT2_ICONTEXT_H_

#include <support2/IBinder.h>
#include <support2/IInterface.h>
#include <support2/IPackageManager.h>

namespace B {
namespace Support2 {

class ITeam;

/*-------------------------------------------------------------*/
/*------- IContext Interface ----------------------------------*/

class IContext : public IInterface
{
public:
	B_DECLARE_META_INTERFACE(Context);

	//!	Return the object implementing the service with the given name.
	/*!	This is the equivalent to retrieving the "Services" dictionary
		and looking up the object there.
	*/
	virtual	IBinder::ptr		LookupService(const BString& name) = 0;
	static const BValue			key_LookupService;
	
	//!	Install the object for the given service name.
	/*!	This is the equivalent to retrieving the "Services" dictiory
		and overlaying your object there.
	*/
	virtual	status_t			PublishService(	const BString& name,
												const IBinder::ptr& object) = 0;
	static const BValue			key_PublishService;
	
	//!	Instantiate a component.
	/*!	\param	id		The name of the component to instantiate.  If
						of the form <vendor>:<package>:<component> then
						that particular will be created.  Otherwise, this
						is taken as a standard class name to instantiate.
	*/
	virtual	IBinder::ptr		New(const BString& id) = 0;
	static const BValue			key_New;
	
	//!	Instantiate a component in another team.
	/*!	\param	id		Component ID or class name to instantiate.
		\param	team	Team this component should be created in.
		\sa New()
	*/
	virtual	IBinder::ptr		RemoteNew(	const BString& id,
											const atom_ptr<ITeam>& team) = 0;
	static const BValue			key_RemoteNew;
	
	//!	Return the package manager for this context.
	/*!	This is the equivalent to retrieving the "Components" dictionary
		and casting it to an IPackageManager.
	*/
	virtual	IPackageManager::ptr	PackageManager() = 0;
	static const BValue				key_PackageManager;
	
	/*!	Retrieves value at a location in the context.
	*/
	virtual BValue				Lookup(const BString &location) = 0;
	static const BValue			key_Lookup;
	
	/*!	Puts value at a location in the context.
	*/
	virtual status_t			Publish(const BString &location, const BValue& val) = 0;
	static const BValue			key_Publish;
	
	/*!	Return the top-level dictionary of this context.
	*/
	virtual IBinder::ptr		Dictionary() = 0;
	static const BValue			key_Dictionary;

	/*! Return a dictionary-component given a slash-delimited path.
	*/
	virtual IBinder::ptr		DictionaryForPath(const BString &path) = 0;
	static const BValue 		key_DictionaryForPath;
	
	//!	Return information needed to instantiate a component.
	/*!	\param	id			The component you are looking for.
		\param	out_info	(RETURN) Information about this component.
		\param	out_name	(RETURN) Name to instantiate this component.
	*/
	virtual	status_t			LookupComponent(const BString& id,
												BValue* out_info,
												BString* out_name,
												BString* fullID = NULL) = 0;
	static const BValue			key_LookupComponent;
};



} } // namespace B::Support2

#endif // _SUPPORT2_ICONTEXT_H_
