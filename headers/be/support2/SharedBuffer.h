/***************************************************************************
//
//	File:			support2/SharedBuffer.h
//
//	Description:	Standard representation of a block of shared data that
//					supports copy-on-write.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _SUPPORT2_SHARED_BUFFER_H_
#define _SUPPORT2_SHARED_BUFFER_H_

#include <support2/SupportDefs.h>

namespace B {
namespace Support2 {

enum {
	//!	This is a magic value to be used as the reference count of a shared
	//!	memory buffer, indicating that the data exists in a static text section.
	/*!	\sa BSharedBuffer */
	B_STATIC_USERS	= 0x40000000
};

// *************************************************************************
/*! \class BSharedBuffer
	\brief A chunk of memory that supports copy-on-write semantics.

	A class for managing raw blocks of memory, which also includes support
	for sharing among multiple users with copy-on-write semantics.
	
	To correctly use the class, you will usually keep a const BSharedBuffer*
	to your buffer, and call Edit() when you would like to modify its data.
	This ensures you follow the correct copy-on-write semantics, not allowing
	you to modify a buffer if it has more than one user.

	\note Because you do not own this memory, you can not place anything
	in it for which you must do some cleanup when freeing its memory.
	(For example, the memory can not contain pointer to binders or any
	other BAtom-derived class, which will need a Release() call when the
	shared buffer it is in goes away.)
*/

class BSharedBuffer
{
public:
			//!	Create a new buffer of the given size.  A buffer starts
			//!	out with a user count of 1; call DecUsers() to free it.
	static	BSharedBuffer*	Alloc(size_t length);
			
			//!	Return a read-only version of the buffer's data.
			const void*		Data() const;
			//!	Return a read/write version of the buffer's data.
			/*!	\note You MUST call Edit() to ensure you are the sole
				owner of this buffer, before modifying its data.
				\sa Edit()
			*/
			void*			Data();
			//!	Return the number of bytes in this buffer.
			size_t			Length() const;
			
			/*! Inverse of Data(): Given a data pointer returned by Data(),
				returns the BSharedBuffer that the pointer came from.  This
				exists largely for the use of BString, which likes to store
				just the pointer to its string data.  In general, it's better
				to store a pointer to the BSharedBuffer, and use Data() to
				access its data.
			 */
	static	const BSharedBuffer* BufferFromData(const void *data);

			//!	Add a user to the buffer.
			void			IncUsers() const;
			//!	Remove a user from the buffer.  If you are the last user,
			//!	the buffer is deallocated.
			void			DecUsers() const;
			//!	Return the number of users this buffer has.
			/*!	\note Because of threading issues, you can only safely use
				this value to check if the user count is 1 or greater than 1;
				and if it is greater than one, you can't assume it will stay
				that way. */
			int32			Users() const;
			
			//!	Given a read-only buffer, start modifying it.
			/*!	If the buffer currently has multiple users, it will be
				copied and a new one returned to you.
			*/
			BSharedBuffer*	Edit(size_t newLength) const;

private:
	inline					BSharedBuffer() { }
	inline					~BSharedBuffer() { }
	
			void			Free();
	
	mutable	int32			m_users;
			int32			pad0;
			int32			pad1;
			size_t			m_length;
};

/*-------------------------------------------------------------*/
/*---- No user serviceable parts after this -------------------*/

inline const void* BSharedBuffer::Data() const
{
	return this+1;
}

inline void* BSharedBuffer::Data()
{
	return this+1;
}

inline const BSharedBuffer* BSharedBuffer::BufferFromData(const void *data)
{
	return ((static_cast<const BSharedBuffer *>(data)) - 1);
}

inline size_t BSharedBuffer::Length() const
{
	return m_length;
}

inline void BSharedBuffer::IncUsers() const
{
	if (m_users < B_STATIC_USERS) atomic_add(&m_users, 1);
}

inline void BSharedBuffer::DecUsers() const
{
	// The common case is that that we are the only user, so
	// optimize that to avoid a call to atomic_add().
	if (m_users == 1 || (m_users < B_STATIC_USERS && atomic_add(&m_users, -1) == 1))
		const_cast<BSharedBuffer*>(this)->Free();
}

inline int32 BSharedBuffer::Users() const
{
	return m_users;
}

} }	// namespace B::Support2

#endif	/* _SUPPORT2_SHARED_BUFFER_H_ */
