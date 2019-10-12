/***************************************************************************
//
//	File:			support2/IStorage.h
//
//	Description:	Abstract interface to a block of raw data.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef	_SUPPORT2_ISTORAGE_INTERFACE_H
#define	_SUPPORT2_ISTORAGE_INTERFACE_H

#include <sys/uio.h>
#include <support2/SupportDefs.h>
#include <support2/IInterface.h>

namespace B {
namespace Support2 {

/*-------------------------------------------------------------*/
/*------- IStorage Class --------------------------------------*/

//! Raw storage read and write transaction codes.
enum {
	B_WRITE_AT_TRANSACTION		= 'WRAT',
	B_READ_AT_TRANSACTION		= 'RDAT'
};

//! This is the contents of a B_READ_AT_TRANSACTION parcel.
/*!	The reply parcel contains a ssize_t followed by the raw data. */
struct read_at_transaction
{
	off_t	position;
	size_t	size;
};

//!	This is the front of a B_WRITE_AT_TRANSACTION parcel; it
//!	is immediately followed by the data to be written.
/*!	The reply parcel contains a single ssize_t indicating the
	number of bytes actually written, or an error code.
*/
struct write_at_transaction
{
	off_t	position;
};

class IStorage : public IInterface
{
	public:

		B_DECLARE_META_INTERFACE(Storage);

				//!	Return the total number of bytes in the store.
		virtual	off_t		Size() const = 0;
		static	value_csml	key_Size;
				//!	Set the total number of bytes in the store.
		virtual	status_t	SetSize(off_t size) = 0;
		static	value_csml	key_SetSize;

				//!	Read the bytes described by \a iovec from location
				//!	\a position in the storage.
				/*!	Returns the number of bytes actually read, or a
					negative error code. */
		virtual	ssize_t		ReadAtV(off_t position, const struct iovec *vector, ssize_t count) = 0;
		static	value_csml	key_ReadAt;
		
				//!	Convenience for reading a vector of one buffer.
				ssize_t		ReadAt(off_t position, void* buffer, size_t size);
				
				//!	Write the bytes described by \a iovec at location
				//!	\a position in the storage.
				/*!	Returns the number of bytes actually written,
					or a negative error code. */
		virtual	ssize_t		WriteAtV(off_t position, const struct iovec *vector, ssize_t count) = 0;
		static	value_csml	key_WriteAt;
		
				//!	Convenience for reading a vector of one buffer.
				ssize_t		WriteAt(off_t position, const void* buffer, size_t size);
		
				//!	Make sure all data in the storage is written to its
				//!	physical device.
				/*!	Returns B_OK if the data is safely stored away, else
					an error code. */
		virtual	status_t	Sync() = 0;
		static	value_csml	key_Sync;
};

/*-------------------------------------------------------------*/
/*---- No user serviceable parts after this -------------------*/

inline ssize_t IStorage::ReadAt(off_t position, void *buffer, size_t size)
{
	iovec v;
	v.iov_base = buffer;
	v.iov_len = size;
	return ReadAtV(position, &v,1);
}

inline ssize_t IStorage::WriteAt(off_t position, const void *buffer, size_t size)
{
	iovec v;
	v.iov_base = const_cast<void*>(buffer);
	v.iov_len = size;
	return WriteAtV(position, &v,1);
}

/*--------------------------------------------------------------*/

} } // namespace B::Support2

#endif	// _SUPPORT2_ISTORAGE_INTERFACE_H
