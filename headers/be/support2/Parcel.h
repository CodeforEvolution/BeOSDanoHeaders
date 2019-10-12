/***************************************************************************
//
//	File:			support2/Parcel.h
//
//	Description:	Container for a raw block of data.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef	_SUPPORT2_PARCEL_H
#define	_SUPPORT2_PARCEL_H

#include <support2/SupportDefs.h>
#include <support2/IBinder.h>
#include <support2/IByteStream.h>
#include <support2/ITextStream.h>
#include <support2/Vector.h>
#include <kernel/OS.h>

namespace B {
namespace Support2 {

class IBinder;

/*-------------------------------------------------------------*/
/*------- BParcel Class ---------------------------------------*/

class BParcel
{
	public:
	
		typedef	void			(*free_func)(	const void* data,
												ssize_t len,
												void* context);
		typedef	status_t		(*reply_func)(	const BParcel& buffer,
												void* context);
	
								BParcel(ssize_t bufferSize = 256);
								BParcel(IByteOutput::sptr output,
										IByteInput::sptr input,
										IByteSeekable::sptr seek,
										ssize_t bufferSize = 256);
								BParcel(reply_func replyFunc,
										void* replyContext = NULL);
								BParcel(const void* data, ssize_t len,
										free_func freeFunc = NULL,
										void* freeContext = NULL,
										reply_func replyFunc = NULL,
										void* replyContext = NULL);
		virtual					~BParcel();
	
				//!	Return a pointer to the data in this parcel.
				const void*		Data() const;
				
				//!	Return an editable pointer to the data in this parcel.
				void*			EditData();
				
				//!	Return the number of bytes in this parcel, or an error code.
				ssize_t			Length() const;
				
				//!	Return the bytes available for use in this parcel, or an error code.
				ssize_t			Avail() const;
				
				//!	Return the current operating status of the parcel.
				status_t		ErrorCheck() const;
				
				//!	Set the parcel to reference an external block of data.
				void			Reference(	const void* data, ssize_t len,
											free_func freeFunc = NULL,
											void* context = NULL);
				
				//!	Allocate data in the parcel, copying from somewhere else.
				status_t		Copy(const void* data, ssize_t len);
				
				//!	Allocate data in the parcel, copying from another parcel.
				status_t		Copy(const BParcel& src);
				
				//!	Create space in the parcel for data.
				status_t		Reserve(ssize_t len);
				
				//!	Allocate data in the parcel that you can edit.
				void*			Alloc(ssize_t len);
				
				//!	Modify data in the parcel.
				void*			ReAlloc(ssize_t len);
				
				//!	Allocate data in the parcel as an array of flattened values.
				status_t		SetValues(const BValue* value1, ...);
				
				//!	Return the number of values in the parcel.
				int32			CountValues() const;
				
				//!	Retrieve flattened values from the parcel.
				int32			GetValues(int32 maxCount, BValue* outValues) const;
				
				//!	Transfer ownership of the data from the given BParcel
				//!	to this one, leaving \param src empty.
				void			Transfer(BParcel* src);
				
				// Sending replies.
				bool			ReplyRequested() const;
				status_t		Reply();
				
				//!	Deallocating data currently in the parcel.
				void			Free();
				
				off_t			Position() const;
				void			SetPosition(off_t pos);
				status_t		SetLength(ssize_t len);
				
				ssize_t			Write(const void* buffer, size_t amount);
				ssize_t			WriteBool(bool val);
				ssize_t			WriteInt8(int8 val);
				ssize_t			WriteInt16(int16 val);
				ssize_t			WriteInt32(int32 val);
				ssize_t			WriteInt64(int64 val);
				ssize_t			WriteFloat(float val);
				ssize_t			WriteDouble(double val);
				ssize_t			WriteString(const BString& val);
				ssize_t			WriteString(const char* val);
				
		static	ssize_t			BinderSize(IBinder::sptr_ref val);
		static	ssize_t			WeakBinderSize(IBinder::wptr_ref val);
				ssize_t			WriteBinder(IBinder::sptr_ref val);
				ssize_t			WriteWeakBinder(IBinder::wptr_ref val);
				ssize_t			WriteRawBinder(	const void* buffer,
												size_t size,
												size_t typeIndex,
												size_t objIndex);
				
				ssize_t			WriteValue(const BValue& val);
				
				ssize_t			Flush();
				ssize_t			Sync();
				
				ssize_t			Read(void* buffer, size_t amount);
				const void*		ReadInplace(size_t amount);
				ssize_t			Drain(size_t amount);
				bool			ReadBool();
				int8			ReadInt8();
				int16			ReadInt16();
				int32			ReadInt32();
				int64			ReadInt64();
				float			ReadFloat();
				double			ReadDouble();
				BString			ReadString();
				
				IBinder::sptr	ReadBinder();
				IBinder::wptr	ReadWeakBinder();
				
				BValue			ReadValue();
				
				//@{
				//!	Retrieving and restoring binder information in the parcel.
				const void*		BinderOffsetsData() const;
				size_t			BinderOffsetsLength() const;
				status_t		SetBinderOffsets(	const void* offsets,
													size_t length,
													bool takeRefs = true);
				//@}
				
				void			PrintToStream(ITextOutput::arg io, uint32 flags = 0) const;
	
	protected:
	
		virtual	ssize_t			WriteBuffer(const void* buffer, size_t len) const;
		virtual	ssize_t			ReadBuffer(void* buffer, size_t len);
												
	private:
								BParcel(const BParcel& o);
				BParcel&		operator=(const BParcel& o);
				
				void			do_free();
				void			acquire_binders();
				void			release_binders();
				
				uint8*				m_data;
				ssize_t				m_length;
				ssize_t				m_avail;
				
				free_func			m_free;
				void*				m_freeContext;
				
				reply_func			m_reply;
				void*				m_replyContext;
				
				off_t				m_base;
				ssize_t				m_pos;
				IByteOutput::sptr	m_out;
				IByteInput::sptr	m_in;
				IByteSeekable::sptr	m_seek;
				
				bool				m_dirty : 1;
				bool				m_ownsBinders : 1;
				bool				m_reserved: 30;
				
				struct entry {
					size_t type;
					size_t object;
				};
				B_IMPLEMENT_BASIC_TYPE_FUNCS(entry);
		
				BVector<entry>		m_binders;
				
				area_id				m_area;
				uint8				m_inline[256];
};

ITextOutput::arg	operator<<(ITextOutput::arg io, const BParcel& value);

/*-------------------------------------------------------------*/

} } // namespace B::Support2

#endif	// _SUPPORT2_PARCEL_H
