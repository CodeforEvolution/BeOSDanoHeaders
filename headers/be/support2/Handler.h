
#ifndef _SUPPORT2_HANDLER_H
#define _SUPPORT2_HANDLER_H

#include <support2/IHandler.h>

#include <support2/Binder.h>
#include <support2/Locker.h>
#include <support2/Message.h>
#include <support2/MessageList.h>
#include <support2/SupportDefs.h>

namespace B {
namespace Support2 {

//! Actions returned to BHandler::FilterMessages().
enum filter_action {
	B_FILTER_REMOVE				= 0,			//!< Remove message from queue
	B_FILTER_KEEP				= 1,			//!< Keep message in queue
	B_FILTER_STOP				= 2				//!< Do not filter any more messages
};

//! Flags for BHandler::FilterMessages() and BHandler::RemoveMessages().
enum {
	B_FILTER_REVERSE_FLAG		= 0x0001,		//!< Filter in reverse time order
	B_FILTER_FUTURE_FLAG		= 0x0002		//!< Include messages after current time
};

class BTeam;

/*----------------------------------------------------------------*/
/*----- BHandler class --------------------------------------------*/

class BHandler : public LHandler
{
public:
	B_STANDARD_ATOM_TYPEDEFS(BHandler);
		
							BHandler(); // deprecated?
							BHandler(const IContext::ptr& context);

			// IHandler API.
	virtual	status_t		PostMessageAtTime(	const BMessage &message,
												bigtime_t absoluteTime,
												uint32 flags = 0);
	virtual	status_t		PostDelayedMessage(	const BMessage &message,
												bigtime_t delay,
												uint32 flags = 0);
	virtual	status_t		PostMessage(		const BMessage &message,
												uint32 flags = 0);
	virtual	status_t		EnqueueMessage(const BMessage& message);
	virtual	void			RedirectMessagesTo(	IHandler::arg new_dest);

			//!	Subclasses override this to receive messages.
	virtual	status_t		HandleMessage(const BMessage &msg);

			//!	Make handler ready to process the next message in its queue.
			/*!	You can call this function while in your HandleMessage()
				method to let the handler schedule itself for the next
				message in its queue, allowing it to continue processing
				even before you have returned from HandleMessage().
			*/
	virtual	void			ResumeScheduling();

			//!	Return the time of the next message in the queue.
			bigtime_t		NextMessageTime();
			
			//!	Remove next pending message with code \a what from the
			//!	handler's message queue.
			/*!	You are responsible for deleting the returned message.
				If there are no matching pending message, NULL is returned.
			*/
			BMessage*		DequeueMessage(uint32 what);
			
			//!	Immediately handle all messages currently in the queue.
			void 			DispatchAllMessages();
			
			//!	Return the number of pending message that match \a what.
			int32			CountMessages(uint32 what=B_ANY_WHAT);

			//! Function callback for FilterMessages().
			/*!	\sa FilterMessages() */
	typedef	filter_action	(*filter_func)(const BMessage* message, void* data);
	
			//!	Iterate through this handler's message queueue, selecting whether
			//!	to keep or remove messages.
			/*!	Your filter_func selects the action to perform for each message
				by the result it turns.  If it selects to remove a message, the
				message will either be placed into \a outRemoved if that is non-NULL,
				or immediately deleted.
				\note filter_func is called WITH THE HANDLER LOCKED.  You can not
				call back into the handler from it, and must be very careful about
				what other locks you acquire.
			*/
			void			FilterMessages(	filter_func func, uint32 flags,
											void* data,
											BMessageList* outRemoved = NULL);
			
			//!	Perform FilterMessages() where all messages that match
			//!	'what' are deleted.
			/*!	\sa FilterMessages() */
			void			RemoveMessages(	uint32 what, uint32 flags,
											BMessageList* outRemoved = NULL);
			
			//!	Perform FilterMessages() where all messages are deleted.
			/*!	\sa FilterMessages() */
			void			RemoveAllMessages(BMessageList* outRemoved = NULL);

			//!	Perform message dispatch.  Should not be called by user
			//!	code.
			status_t		DispatchMessage();

protected:
	virtual					~BHandler();
	
	virtual	status_t		Acquired(const void* id);
	virtual	status_t		Released(const void* id);
		
private:
	friend	class			BTeam;
	
			enum scheduling {
				CANCEL_SCHEDULE = 0,
				DO_SCHEDULE,
				DO_RESCHEDULE
			};

			status_t		Enqueue(BMessage* message);
			void			DeferScheduling();
			void			Unschedule();
			scheduling		StartSchedule();
			void			DoneSchedule();

			atom_ptr<BTeam>	m_team;
			BLocker			m_lock;
			BMessageList	m_msgQueue;
			uint32			m_state;
			
			BHandler *		m_next;
			BHandler *		m_prev;
			IHandler::ptr	m_redirectedTo;

};

/**************************************************************************************/

} } // namespace B::Support2

#endif /* _SUPPORT2_HANDLER_H */
