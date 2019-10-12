
#ifndef _SUPPORT2_TEAM_H
#define _SUPPORT2_TEAM_H

#include <support2/ITeam.h>

#include <kernel/OS.h>
#include <support2/SupportDefs.h>
#include <support2/Atom.h>
#include <support2/IBinder.h>
#include <support2/KeyedVector.h>
#include <support2/Locker.h>
#include <support2/String.h>
#include <support2/Vector.h>

namespace B {
namespace Support2 {

class BHandler;
class IVirtualMachine;

extern int32 this_team();

/*----------------------------------------------------------------*/

enum {
	B_FORGET_CURRENT_ENVIRONMENT	= 0x00000001
};

//!	Representation of your currently running team.
class BTeam : public LTeam
{
public:
			B_STANDARD_ATOM_TYPEDEFS(BTeam);

								BTeam(team_id tid);

	//!	Create a new, empty team, which you can start instantiating
	//!	components in.
	static	ITeam::ptr			Spawn(	const BString& name = BString(),
										const BValue& env = BValue::undefined,
										uint32 flags = 0);
	
	//!	Load the requested image as a new team, and return its root object.
	static	IBinder::ptr		SpawnFile(	const BString& file_path,
											const BValue& env = BValue::undefined,
											uint32 flags = 0);
	
			team_id				ID() const;
	
	//! Implement ITeam::InstantiateComponent().
	virtual IBinder::ptr		InstantiateComponent(	IContext::sptr_ref context,
														const BValue &componentInfo,
														const BString &component);
	
	// ----------- The remaining methods are not for public use -----------
	
			void				DispatchMessage();

			IBinder::ptr		GetProxyForHandle(int32 handle);
			IBinder::ref		GetProxyRefForHandle(int32 handle);
			void				ExpungeHandle(int32 handle, IBinder* binder);
			
			class RootImage;
			bool				ExpungeImage(const atom_ref<RootImage>& image);
			
protected:
	virtual						~BTeam();
	virtual	status_t			Acquired(const void* id);
	virtual	status_t			Released(const void* id);

private:

	friend	class BHandler;
	
	static	bool				ResumingScheduling();
	static	void				ClearSchedulingResumed();
	static	bool				InsertHandler(BHandler **handlerP, BHandler *handler);
			void				UnscheduleHandler(BHandler* h, bool lock=true);
			void				ScheduleNextEvent();
			void				ScheduleHandler(BHandler* h);
			IBinder* & 			BinderForHandle(int32 handle);
			IBinder::ptr		DoInstantiate(	IContext::sptr_ref context,
												const BValue &componentInfo,
												const BString &component,
												BVector<BString> &vmIDs);
			
			const team_id				m_id;
			BLocker						m_lock;
			BHandler*					m_pendingHandlers;
			bigtime_t					m_nextEventTime;
			
			// Information about loaded component images.
			
			BNestedLocker				m_imageLock;
			
			// All images that are loaded.
			struct root_image_entry {
				sem_id				loading;
				atom_ref<RootImage> image;
			};
			BKeyedVector<BString, root_image_entry >
										m_images;
			
			// All virtual machines that are running.
			BKeyedVector<BString, atom_ptr<IVirtualMachine> >
										m_vms;
			
			// Information about remote binders.
			
			BNestedLocker				m_handleRefLock;
			BVector<IBinder*>			m_handleRefs;
			
};

} } // namespace B::Support2


#endif /* _SUPPORT2_TEAM_H */
