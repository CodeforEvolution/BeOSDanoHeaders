
#ifndef _SUPPORT2_LOOPER_H
#define _SUPPORT2_LOOPER_H

#include <OS.h>
#include <support2/Atom.h>
#include <support2/SupportDefs.h>
#include <support2/IBinder.h>
#include <support2/Parcel.h>
#include <support2/StreamPipe.h>
#include <support2/Team.h>
#include <support2/OrderedVector.h>

namespace B {
namespace Support2 {

class BParcel;
class BImage;
struct handle_refs;

/*----------------------------------------------------------------*/
/*----- BLooper class --------------------------------------------*/

class BLooper {
	
	public:

		static	BLooper *				This();
		static	BLooper *				This(const atom_ptr<BTeam>& team);
		
		static	void					SetThreadPriority(int32 priority);
		
		static	status_t				LastError();
		static	void					SetLastError(status_t error);
		
		static	status_t				Loop();
		
		//!	spawn_thread for binder debugging.
		/*!	This is just like spawn_thread(), except it takes care of propagating
			simulated team information when debugging the binder driver.
		*/
		static	thread_id				SpawnThread(	thread_func		function_name, 
														const char 		*thread_name, 
														int32			priority, 
														void			*arg);
		
		static	status_t				SpawnLooper();
		
		static	int32					Thread();
		static	atom_ptr<BTeam>			Team();
		static	int32					TeamID();

		static	void					KillTeam();
		
		static	status_t				SetRootObject(IBinder::ptr rootNode);
	#if BINDER_DEBUG_LIB
		static	IBinder::ptr			GetRootObject(thread_id id, team_id team);
	#else
		static	IBinder::ptr			GetRootObject(thread_id id);
	#endif
	
		typedef status_t (*catch_root_func)(const IBinder::ptr& node);
		static	void					CatchRootObjects(catch_root_func func);

		static	IBinder::ptr			GetProxyForHandle(int32 handle);
		static	IBinder::ref			GetProxyRefForHandle(int32 handle);

				status_t				Transact(	int32 handle,
													uint32 code, const BParcel& data,
													BParcel* reply, uint32 flags);
		
		// Only called by the main thread during static initialization
		static	status_t				InitMain(const atom_ptr<BTeam>& team = atom_ptr<BTeam>());
		static	status_t				InitOther(const atom_ptr<BTeam>& team = atom_ptr<BTeam>());

		// For use only by IBinder proxy object
				status_t				IncrefsHandle(int32 handle);
				status_t				AcquireHandle(int32 handle);
				status_t				ReleaseHandle(int32 handle);
				status_t				DecrefsHandle(int32 handle);
				status_t				AttemptAcquireHandle(int32 handle);
		static	status_t				ExpungeHandle(int32 handle, IBinder* binder);

	private:
	
		friend	class BTeam;
		friend	class BTeam::RootImage;

										BLooper(const atom_ptr<BTeam>& team = atom_ptr<BTeam>());
										~BLooper();

		static	int32					TLS;
		static	void					_SetNextEventTime(bigtime_t when);
		static	bool					_ResumingScheduling();
		static	void					_ClearSchedulingResumed();
		static	int32					_ThreadEntry(void *info);
		static	int32					_DriverLoop(BLooper *parent);
#if !BINDER_DEBUG_LIB
		static	int32					_DriverLoopFromKernelLand(BLooper *parent);
#endif
		static	int32					_Loop(BLooper *parent);
		static	void					_DeleteSelf(void *self);
		static	status_t				_BufferReply(const BParcel& buffer, void* context);
		static	void					_BufferFree(const void* data, ssize_t len, void* context);
		
				IBinder::ptr			_GetRootObject(thread_id id, team_id team);
				void					_CatchRootObjects(catch_root_func func);
				void					_SetThreadPriority(int32 priority);
				void					_KillTeam();
				status_t				_HandleCommand(int32 cmd);
				status_t				_WaitForCompletion(	BParcel *reply = NULL,
															status_t *acquireResult = NULL);
				status_t				_WriteTransaction(	int32 cmd, uint32 binderFlags,
															int32 handle, uint32 code,
															const BParcel& data,
															status_t* statusBuffer = NULL);
				status_t				_Reply(uint32 flags, const BParcel& reply);
				status_t				_TransactWithDriver(bool doRead = true);
				void					_ExpungeImages();

				int32					_Main();
				int32					_LoopSelf();

				atom_ptr<BTeam>			m_team;
				team_id					m_teamID;
				int32					m_thid;
				int32					m_priority;
				int32					m_binderDesc;
				BParcel					m_in;
				BParcel					m_out;
				int32					m_flags;
				status_t				m_lastError;
				
				BOrderedVector< atom_ref<BTeam::RootImage> >
										m_dyingImages;
};

} } // namespace B::Support2

#endif /* _SUPPORT2_LOOPER_H */
