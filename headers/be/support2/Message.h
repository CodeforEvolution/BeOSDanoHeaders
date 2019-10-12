/******************************************************************************
/
/	File:			Message.h
/
/	Description:	BMessage class creates objects that store data and that
/					can be processed in a message loop.
/
/	Copyright 1995-98, Be Incorporated, All Rights Reserved.
/
*******************************************************************************/

#ifndef _SUPPORT2_MESSAGE_H
#define _SUPPORT2_MESSAGE_H

#include <support2/SupportDefs.h>
#include <support2/IHandler.h>
#include <support2/ITextStream.h>
#include <support2/Value.h>

namespace B {
namespace Support2 {

class BMessageList;

/*-------------------------------------------------------------*/
/* --------- BMessage class----------------------------------- */

class BMessage
{
public:

							BMessage();
							BMessage(uint32 what);
							BMessage(const BMessage &);
							BMessage(const BValue &);
	virtual					~BMessage();
	
			uint32			What() const;
			BMessage&		SetWhat(uint32 val);
			
			bigtime_t		When() const;
			BMessage&		SetWhen(bigtime_t val);
			
			IHandler::sptr	Sender() const;
			BMessage&		SetSender(IHandler::wptr_ref val);
			
			IHandler::sptr	ReplyTo() const;
			BMessage&		SetReplyTo(IHandler::wptr_ref val);
			
			const BValue&	Data() const;
			BValue&			Data();
			BMessage&		SetData(const BValue& data);
			
			// Convenience functions.  These are the same as calling
			// the corresponding function on Data().
			BMessage&		Join(const BValue& from, uint32 flags = 0);
			BMessage&		JoinItem(const BValue& key, const BValue& value, uint32 flags=0);
			BMessage&		JoinItem(const value_ref& key, const BValue& value, uint32 flags=0);
			BMessage&		JoinItem(const BValue& key, const value_ref& value, uint32 flags=0);
			BMessage&		JoinItem(const value_ref& key, const value_ref& value, uint32 flags=0);
			const BValue	ValueFor(const BValue& key, uint32 flags = 0) const;
			const BValue	ValueFor(const value_ref& key, uint32 flags = 0) const;
	inline	const BValue	operator[](const value_ref& key) const	{ return ValueFor(key); }
	inline	const BValue	operator[](const BValue& key) const		{ return ValueFor(key); }
			
			BValue			AsValue() const;
	inline					operator BValue() const			{ return AsValue(); }
	
			status_t		PrintToStream(ITextOutput::sptr_ref io, uint32 flags = 0) const;
	
private:

	friend	class			BMessageList;
	
			uint32			m_what;
			bigtime_t		m_when;
			IHandler::wptr	m_sender;
			IHandler::wptr	m_replyTo;
			BValue			m_data;
			
			BMessage *		m_next;
			BMessage *		m_prev;
};

ITextOutput::sptr_ref operator<<(ITextOutput::sptr_ref io, const BMessage& message);

/*-------------------------------------------------------------*/

} } // namespace B::Support2

#endif /* _MESSAGE_H */

