/***************************************************************************
//
//	File:			support2/ITextStream.h
//
//	Description:	Abstract interface for a formatted text output
//					stream, with C++ iostream-like operators.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef	_SUPPORT2_TEXTSTREAM_INTERFACE_H
#define	_SUPPORT2_TEXTSTREAM_INTERFACE_H

#include <support2/IInterface.h>

#include <sys/uio.h>

namespace B {
namespace Support2 {

class BValue;

/*-----------------------------------------------------------------*/

//!	Abstract interface for streaming formatted text.
class ITextOutput : public IInterface
{
	public:

		// Note: binder remote interface not yet implemented.
		B_STANDARD_ATOM_TYPEDEFS(ITextOutput)

		//!	Attributes that can be included in a LogV() call.
		struct log_info {
			int32		tag;
			int32		team;
			int32		thread;
			bigtime_t	time;
			int32		indent;
			bool		front : 1;
			bool		reserved : 30;
			BValue*		extra;
		};
		
		//!	Print the given text to the stream.  The current indentation
		//!	level, and possibly other formatting, will be applied to each line.
		/*!	If \a len is negative, all text up to the first \0 character will
			be printed.  Otherwise, up to \a len characters will be printed. */
		virtual	status_t				Print(	const char *debugText,
												ssize_t len = -1) = 0;
		
		//!	Adjust the current indentation level by \a delta amount.
		virtual void					MoveIndent(	int32 delta) = 0;
		
		//!	Write a log line.
		/*!	Metadata about the text is supplied in \a info.  The text itself
			is supplied in the iovec, which is handled as one atomic unit.
			You can supply B_WRITE_END for \a flags to indicate the item
			being logged has ended. */
		virtual	status_t				LogV(	const log_info& info,
												const iovec *vector,
												ssize_t count,
												uint32 flags = 0) = 0;
};

/*-----------------------------------------------------------------*/

// Generic manipulator function for the stream.
typedef ITextOutput::arg (*ITextOutputManipFunc)(ITextOutput::arg);

ITextOutput::arg endl(ITextOutput::arg io);
ITextOutput::arg indent(ITextOutput::arg io);
ITextOutput::arg dedent(ITextOutput::arg io);

ITextOutput::arg operator<<(ITextOutput::arg io, const char* str);
ITextOutput::arg operator<<(ITextOutput::arg io, char);		// writes raw character

ITextOutput::arg operator<<(ITextOutput::arg io, bool);
ITextOutput::arg operator<<(ITextOutput::arg io, int);
ITextOutput::arg operator<<(ITextOutput::arg io, unsigned int);
ITextOutput::arg operator<<(ITextOutput::arg io, uint32);
ITextOutput::arg operator<<(ITextOutput::arg io, int32);
ITextOutput::arg operator<<(ITextOutput::arg io, uint64);
ITextOutput::arg operator<<(ITextOutput::arg io, int64);
ITextOutput::arg operator<<(ITextOutput::arg io, float);
ITextOutput::arg operator<<(ITextOutput::arg io, double);

ITextOutput::arg operator<<(ITextOutput::arg io, ITextOutputManipFunc func);

ITextOutput::arg operator<<(ITextOutput::arg io, const void*);

template <class TYPE>
inline ITextOutput::arg operator<<(ITextOutput::arg io, const atom_ptr<TYPE>& a)
{
	return io << a.ptr();
}

template <class TYPE>
inline ITextOutput::arg operator<<(ITextOutput::arg io, const atom_ref<TYPE>& a)
{
	// cough, cough.
	return io << (*(void**)&a);
}

/*-----------------------------------------------------------------*/

inline ITextOutput::arg operator<<(ITextOutput::arg io, ITextOutputManipFunc func)
{
	return (*func)(io);
}

/*-----------------------------------------------------------------*/
/*------- Formatting type_code values -----------------------------*/

class BTypeCode {
public:
	BTypeCode(type_code type);
	~BTypeCode();

	type_code TypeCode() const;
	
private:
	type_code fType;
	int32 _reserved;
};

ITextOutput::arg operator<<(ITextOutput::arg io, const BTypeCode& type);

/*-----------------------------------------------------------------*/
/*------- Dumping raw bytes as hex --------------------------------*/

class BHexDump {
public:
	BHexDump(const void *buf, size_t length, size_t bytesPerLine=16);
	~BHexDump();
	
	BHexDump& SetBytesPerLine(size_t bytesPerLine);
	BHexDump& SetSingleLineCutoff(int32 bytes);
	BHexDump& SetAlignment(size_t alignment);
	
	const void* Buffer() const;
	size_t Length() const;
	size_t BytesPerLine() const;
	int32 SingleLineCutoff() const;
	size_t Alignment() const;

private:
	const void* fBuffer;
	size_t fLength;
	size_t fBytesPerLine;
	int32 fSingleLineCutoff;
	size_t fAlignment;
	
	uint32 _reserved[8];
};

ITextOutput::arg operator<<(ITextOutput::arg io, const BHexDump& buffer);

/*-----------------------------------------------------------------*/
/*------- printf like output --------------------------------------*/

class BPrintf {
public:
	BPrintf(const char *fmt, ...);
	~BPrintf();
	const char* Buffer() const;
private:
	char* m_buffer;
	uint32 _reserved[3];
};

ITextOutput::arg operator<<(ITextOutput::arg io, const BPrintf& data);

/*-----------------------------------------------------------------*/

} } // namespace B::Support2

#endif /* _SUPPORT2_TEXTSTREAM_INTERFACE_H */
