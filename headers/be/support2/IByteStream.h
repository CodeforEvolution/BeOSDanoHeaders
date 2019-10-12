/***************************************************************************
//
//	File:			support2/IByteStream.h
//
//	Description:	Raw streaming interfaces.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef	_SUPPORT2_BYTESTREAM_INTERFACE_H
#define	_SUPPORT2_BYTESTREAM_INTERFACE_H

#include <sys/uio.h>
#include <support2/IInterface.h>

namespace B {
namespace Support2 {

//! Raw byte input and output transaction codes.
enum {
	B_WRITE_TRANSACTION			= 'WRTE',
	B_WRITE_FLAGS_TRANSACTION	= 'WRFL',
	B_READ_TRANSACTION			= 'READ'
};

//! Flags for IByteOutput::Write().
enum {
	//!	Mark the final location as the end of the stream.
	/*!	Any data currently after this location will be removed;
		further writes will extend the stream from here.  This
		flag will NOT be used if the write operation does not
		include all supplied bytes.  (I.e., it returns fewer
		bytes than the caller supplied or an error code.)
	*/
	B_WRITE_END					= 0x00010000
};

/*-------------------------------------------------------------*/
/*------- IByteInput Interface --------------------------------*/

class IByteInput : public IInterface
{
	public:

		B_DECLARE_META_INTERFACE(ByteInput);

				//!	Read the bytes described by "iovec" from the stream.
				/*!	Returns the number of bytes actually read, or a
					negative error code.  A NULL 'vector' is valid if 'count'
					is <= 0, in which case 'count' is returned.  
				*/
		virtual	ssize_t		ReadV(const iovec *vector, ssize_t count) = 0;
		static	value_csml	key_Read;
		
				//!	Convenience for reading a vector of one buffer.
				ssize_t		Read(void *buffer, size_t size);
};

/*-------------------------------------------------------------*/
/*------- IByteOutput Interface -------------------------------*/

class IByteOutput : public IInterface
{
	public:

		B_DECLARE_META_INTERFACE(ByteOutput);

				//!	Write the bytes described by "iovec" in to the stream.
				/*!	Returns the number of bytes actually written, or a
					negative error code.  A NULL 'vector' is valid if 'count'
					is <= 0, in which case count is returned.  If 'count' is
					zero and the B_WRITE_END flag is supplied, the stream is
					truncated at its current location.
				*/
		virtual	ssize_t		WriteV(const iovec *vector, ssize_t count, uint32 flags = 0) = 0;
		static	value_csml	key_Write;
		
				//!	Convenience for writing a vector of one buffer.
				ssize_t		Write(const void *buffer, size_t size, uint32 flags = 0);
				
				//!	Make sure all data in the stream is written to its
				//!	physical device.
				/*!	Returns B_OK if the data is safely stored away, else an error code.
				*/
		virtual	status_t	Sync() = 0;
		static	value_csml	key_Sync;
};

/*-------------------------------------------------------------*/
/*------- IByteSeekable Interface -----------------------------*/

class IByteSeekable : public IInterface
{
	public:

		B_DECLARE_META_INTERFACE(ByteSeekable);

				//!	Return the current location in the stream, or a
				//!	negative error code.
		virtual	off_t		Position() const = 0;
		static	value_clrg	key_Position;
		
				//!	Move to a new location in the stream.
				/*!	The seek_mode can be either SEEK_SET, SEEK_END, or SEEK_CUR.
					Returns the new location, or a negative error code.
				*/
		virtual off_t		Seek(off_t position, uint32 seek_mode) = 0;
		static	value_csml	key_Seek;
};

/*-------------------------------------------------------------*/
/*---- No user serviceable parts after this -------------------*/

inline ssize_t IByteInput::Read(void *buffer, size_t size)
{
	iovec v;
	v.iov_base = buffer;
	v.iov_len = size;
	return ReadV(&v,1);
}

inline ssize_t IByteOutput::Write(const void *buffer, size_t size, uint32 flags)
{
	iovec v;
	v.iov_base = const_cast<void*>(buffer);
	v.iov_len = size;
	return WriteV(&v,1,flags);
}

/*-------------------------------------------------------------*/

} } // namespace B::Support2

#endif /* _SUPPORT2_BYTESTREAM_INTERFACE_H */
