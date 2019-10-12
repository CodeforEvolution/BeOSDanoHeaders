/*******************************************************************************
/
/	File:			String.h
/
/   Description:    String class supporting common string operations.  
/
/	Copyright 1992-98, Be Incorporated, All Rights Reserved
/
*******************************************************************************/

#ifndef _SUPPORT2_STRING_H
#define _SUPPORT2_STRING_H

#include <support2/SupportDefs.h>
#include <support2/ITextStream.h>
#include <string.h>

namespace B {
namespace Support2 {

class BSharedBuffer;

class BString {
public:
						BString();
	explicit			BString(const char *);
						BString(const BString &);
						BString(const char *, int32 maxLength);
						BString(const BSharedBuffer *buf);
					
						~BString();
			
/*---- Access --------------------------------------------------------------*/

	const char 			*String() const;
						/* returns null-terminated string */

						operator const char *() const { return String(); }

	int32 				Length() const;
						/* length of corresponding string */

	int32				CountChars() const;
						/* returns number of UTF8 characters in string */
/*---- Assignment ----------------------------------------------------------*/
	BString 			&operator=(const BString &);
	BString 			&operator=(const char *);
	
	BString				&SetTo(const char *);
	BString 			&SetTo(const char *, int32 length);

	BString				&SetTo(const BString &from);
	BString				&Adopt(BString &from);
						/* leaves <from> empty, avoiding a copy */
						
	BString 			&SetTo(const BString &, int32 length);
	BString 			&Adopt(BString &from, int32 length);
						/* leaves <from> empty, avoiding a copy */
		
	BString 			&SetTo(char, int32 count);

	void				Swap(BString& with);
	
/*---- Substring copying ---------------------------------------------------*/
	BString 			&CopyInto(BString &into, int32 fromOffset,
							int32 length) const;
						/* returns <into> ref as it's result; doesn't do
						 * anything if <into> is <this>
						 */

	void				CopyInto(char *into, int32 fromOffset,
							int32 length) const;
						/* caller guarantees that <into> is large enough */

/*---- Appending -----------------------------------------------------------*/
	BString 			&operator+=(const BString &);
	BString 			&operator+=(const char *);
	BString 			&operator+=(char);
	
	BString 			&Append(const BString &);
	BString 			&Append(const char *);

	BString 			&Append(const BString &, int32 length);
	BString 			&Append(const char *, int32 length);
	BString 			&Append(char, int32 count);

/*---- Prepending ----------------------------------------------------------*/
	BString 			&Prepend(const char *);
	BString 			&Prepend(const BString &);
	BString 			&Prepend(const char *, int32);
	BString 			&Prepend(const BString &, int32);
	BString 			&Prepend(char, int32 count);

/*---- Inserting ----------------------------------------------------------*/
	BString 			&Insert(const char *, int32 pos);
	BString 			&Insert(const char *, int32 length, int32 pos);
	BString 			&Insert(const char *, int32 fromOffset,
							int32 length, int32 pos);

	BString 			&Insert(const BString &, int32 pos);
	BString 			&Insert(const BString &, int32 length, int32 pos);
	BString 			&Insert(const BString &, int32 fromOffset,
							int32 length, int32 pos);
	BString 			&Insert(char, int32 count, int32 pos);

/*---- Removing -----------------------------------------------------------*/
	BString 			&Truncate(int32 newLength);
						
	BString 			&Remove(int32 from, int32 length);

	BString 			&RemoveFirst(const BString &);
	BString 			&RemoveLast(const BString &);
	BString 			&RemoveAll(const BString &);

	BString 			&RemoveFirst(const char *);
	BString 			&RemoveLast(const char *);
	BString 			&RemoveAll(const char *);
	
	BString 			&RemoveSet(const char *setOfCharsToRemove);
	
	BString				&Compact(const char *setOfCharsToCompact);
	BString				&Compact(const char *setOfCharsToCompact,
								 char* replacementChar);
	
	BString 			&MoveInto(BString &into, int32 from, int32 length);
	void				MoveInto(char *into, int32 from, int32 length);
						/* caller guarantees that <into> is large enough */


/*---- Compare functions ---------------------------------------------------*/
	bool 				operator<(const BString &) const;
	bool 				operator<=(const BString &) const;
	bool 				operator==(const BString &) const;
	bool 				operator>=(const BString &) const;
	bool 				operator>(const BString &) const;
	bool 				operator!=(const BString &) const;
	
	bool 				operator<(const char *) const;
	bool 				operator<=(const char *) const;
	bool 				operator==(const char *) const;
	bool 				operator>=(const char *) const;
	bool 				operator>(const char *) const;
	bool 				operator!=(const char *) const;
	
/*---- strcmp-style compare functions --------------------------------------*/
	int 				Compare(const BString &) const;
	int 				Compare(const char *) const;
	int 				Compare(const BString &, int32 n) const;
	int 				Compare(const char *, int32 n) const;
	int 				ICompare(const BString &) const;
	int 				ICompare(const char *) const;
	int 				ICompare(const BString &, int32 n) const;
	int 				ICompare(const char *, int32 n) const;
	
/*---- Searching -----------------------------------------------------------*/
	int32 				FindFirst(const BString &) const;
	int32 				FindFirst(const char *) const;
	int32 				FindFirst(const BString &, int32 fromOffset) const;
	int32 				FindFirst(const char *, int32 fromOffset) const;
	int32				FindFirst(char) const;
	int32				FindFirst(char, int32 fromOffset) const;

	int32 				FindLast(const BString &) const;
	int32 				FindLast(const char *) const;
	int32 				FindLast(const BString &, int32 beforeOffset) const;
	int32 				FindLast(const char *, int32 beforeOffset) const;
	int32				FindLast(char) const;
	int32				FindLast(char, int32 fromOffset) const;

	int32 				IFindFirst(const BString &) const;
	int32 				IFindFirst(const char *) const;
	int32 				IFindFirst(const BString &, int32 fromOffset) const;
	int32 				IFindFirst(const char *, int32 fromOffset) const;

	int32 				IFindLast(const BString &) const;
	int32 				IFindLast(const char *) const;
	int32 				IFindLast(const BString &, int32 beforeOffset) const;
	int32 				IFindLast(const char *, int32 beforeOffset) const;

/*---- Replacing -----------------------------------------------------------*/

	BString 			&ReplaceFirst(char replaceThis, char withThis);
	BString 			&ReplaceLast(char replaceThis, char withThis);
	BString 			&ReplaceAll(char replaceThis, char withThis,
							int32 fromOffset = 0);
	BString 			&Replace(char replaceThis, char withThis,
							int32 maxReplaceCount, int32 fromOffset = 0);
	BString 			&ReplaceFirst(const char *replaceThis,
							const char *withThis);
	BString 			&ReplaceLast(const char *replaceThis,
							const char *withThis);
	BString 			&ReplaceAll(const char *replaceThis,
							const char *withThis, int32 fromOffset = 0);
	BString 			&Replace(const char *replaceThis, const char *withThis,
							int32 maxReplaceCount, int32 fromOffset = 0);

	BString 			&IReplaceFirst(char replaceThis, char withThis);
	BString 			&IReplaceLast(char replaceThis, char withThis);
	BString 			&IReplaceAll(char replaceThis, char withThis,
							int32 fromOffset = 0);
	BString 			&IReplace(char replaceThis, char withThis,
							int32 maxReplaceCount, int32 fromOffset = 0);
	BString 			&IReplaceFirst(const char *replaceThis,
							const char *withThis);
	BString 			&IReplaceLast(const char *replaceThis,
							const char *withThis);
	BString 			&IReplaceAll(const char *replaceThis,
							const char *withThis, int32 fromOffset = 0);
	BString 			&IReplace(const char *replaceThis, const char *withThis,
							int32 maxReplaceCount, int32 fromOffset = 0);
	
	BString				&ReplaceSet(const char *setOfChars, char with);
	BString				&ReplaceSet(const char *setOfChars, const char *with);
/*---- Unchecked char access -----------------------------------------------*/
	char 				operator[](int32 index) const;

/*---- Checked char access -------------------------------------------------*/
	char 				ByteAt(int32 index) const;

/*---- Fast low-level manipulation -----------------------------------------*/
	char 				*LockBuffer(int32 maxLength);
	
		/* Make room for characters to be added by C-string like manipulation.
		 * Returns the equivalent of String(), <maxLength> includes space for
		 * trailing zero while used as C-string, it is illegal to call other
		 * BString routines that rely on data/length consistency until
		 * UnlockBuffer sets things up again.
		 */

	BString 			&UnlockBuffer(int32 length = -1);
	
		/* Finish using BString as C-string, adjusting length. If no length
		 * passed in, strlen of internal data is used to determine it.
		 * BString is in consistent state after this.
		 */
	
	const	BSharedBuffer*	SharedBuffer() const;
		
		/* Get the BSharedBuffer that the BString is using, or NULL if it's
		 * empty.
		 */

/*---- Upercase<->Lowercase ------------------------------------------------*/
	BString				&ToLower();
	BString 			&ToUpper();

	BString 			&Capitalize();
						/* Converts first character to upper-case, rest to
						 * lower-case
						 */

	BString 			&CapitalizeEachWord();
						/* Converts first character in each white-space-separated
						 * word to upper-case, rest to lower-case
						 */
/*----- Escaping and Deescaping --------------------------------------------*/
	BString				&CharacterEscape(const char *original,
							const char *setOfCharsToEscape, char escapeWith);
						/* copies original into <this>, escaping characters
						 * specified in <setOfCharsToEscape> by prepending
						 * them with <escapeWith>
						 */
	BString				&CharacterEscape(const char *setOfCharsToEscape,
							char escapeWith);
						/* escapes characters specified in <setOfCharsToEscape>
						 * by prepending them with <escapeWith>
						 */

	BString				&CharacterDeescape(const char *original, char escapeChar);
						/* copy <original> into the string removing the escaping
						 * characters <escapeChar> 
						 */
	BString				&CharacterDeescape(char escapeChar);
						/* remove the escaping characters <escapeChar> from 
						 * the string
						 */

/*----- Handling Whitespace ------------------------------------------------*/
						/*! Removes all leading and trailing whitespace, and
						 *	converts all other whitepace to exactly one
						 *	space character
						 */
	BString				&Mush();
	
						/*! Removes all whitespace
						 */
	BString				&StripWhitespace();
						
						/*!	Returns true if the string contains one or more
							non-whitespace characters, or the length is
							zero.  Returns false otherwise
						 */
	bool				IsOnlyWhitespace() const;
	
/*---- Simple sprintf replacement calls ------------------------------------*/
/*---- Slower than sprintf but type and overflow safe ----------------------*/
	BString 		&operator<<(const char *);
	BString 		&operator<<(const BString &);
	BString 		&operator<<(char);
	BString 		&operator<<(int);
	BString 		&operator<<(unsigned int);
	BString 		&operator<<(uint32);
	BString 		&operator<<(int32);
	BString 		&operator<<(uint64);
	BString 		&operator<<(int64);
	BString 		&operator<<(float);
		/* float output hardcodes %.2f style formatting */
	
/*---- Path handling -------------------------------------------------------*/

	status_t		PathSetTo(const BString &dir, const BString &leaf = BString(),
						bool normalize = false);
	status_t		PathSetTo(const char *dir, const char *leaf = NULL,
						bool normalize = false);
	const char *	PathLeaf(void) const;
	status_t		PathGetParent(BString *parentPath) const;
	status_t 		PathAppend(const BString &leaf, bool normalize = false);
	status_t 		PathAppend(const char *leaf, bool normalize = false);
	
/*----- Private or reserved ------------------------------------------------*/
private:
	void 			_Init(const char *, int32);
	void 			_DoAssign(const char *, int32);
	void 			_DoSetTo(const char *, int32);
	status_t		_PathSetTo(const char *dir, int32 dirLength,
						const char *leaf, int32 leafLength, bool normalize);
	void 			_DoAppend(const char *, int32);
	char 			*_GrowBy(int32);
	char 			*_OpenAtBy(int32, int32);
	char 			*_ShrinkAtBy(int32, int32);
	void 			_DoPrepend(const char *, int32);
	void			_DoCompact(const char* set, const char* replace);
	void			_DoReplaceAt(int32 offset, int32 sourceLength, int32 replaceLength, const char *withThis);
	int32 			_FindAfter(const char *, int32, int32) const;
	int32 			_IFindAfter(const char *, int32, int32) const;
	int32 			_ShortFindAfter(const char *, int32) const;
	int32 			_FindBefore(const char *, int32, int32) const;
	int32 			_IFindBefore(const char *, int32, int32) const;
	
	void			_SetUsingAsCString(bool);
	void 			_AssertNotUsingAsCString() const;

	BSharedBuffer*	_Edit(size_t newStrLen);

protected:
	const char *_privateData;
};

/*----- Type and STL utilities --------------------------------------*/
void				BMoveBefore(BString* to, BString* from, size_t count = 1);
void				BMoveAfter(BString* to, BString* from, size_t count = 1);
void				BSwap(BString& v1, BString& v2);
int32				BCompare(const BString& v1, const BString& v2);
void				swap(BString& x, BString& y);

/*----- Comutative compare operators --------------------------------------*/
bool 				operator<(const char *, const BString &);
bool 				operator<=(const char *, const BString &);
bool 				operator==(const char *, const BString &);
bool 				operator>(const char *, const BString &);
bool 				operator>=(const char *, const BString &);
bool 				operator!=(const char *, const BString &);

/*----- Non-member compare for sorting, etc. ------------------------------*/
int 				Compare(const BString &, const BString &);
int 				ICompare(const BString &, const BString &);
int 				Compare(const BString *, const BString *);
int 				ICompare(const BString *, const BString *);

/*----- Streaming into IDebugOutput --------------------------------------------*/

ITextOutput::arg operator<<(ITextOutput::arg io, const BString& string);

/*-------------------------------------------------------------------------*/
/*---- No user serviceable parts after this -------------------------------*/

inline int32 
BString::Length() const
{
	return _privateData ? (*((int32 *)_privateData - 1) & 0x7fffffff) - 1: 0;
		/* the most significant bit is reserved; accessing
		 * it in any way will cause the computer to explode
		 */
}

inline const char *
BString::String() const
{
	if (!_privateData)
		return "";
	return _privateData;
}

inline BString &
BString::SetTo(const char *str)
{
	return operator=(str);
}

inline char 
BString::operator[](int32 index) const
{
	return _privateData[index];
}

inline char 
BString::ByteAt(int32 index) const
{
	if (!_privateData || index < 0 || index > Length())
		return 0;
	return _privateData[index];
}

inline BString &
BString::operator+=(const BString &string)
{
	_DoAppend(string.String(), string.Length());
	return *this;
}

inline BString &
BString::Append(const BString &string)
{
	_DoAppend(string.String(), string.Length());
	return *this;
}

inline BString &
BString::Append(const char *str)
{
	return operator+=(str);
}

inline bool 
BString::operator==(const BString &string) const
{
	return strcmp(String(), string.String()) == 0;
}

inline bool 
BString::operator<(const BString &string) const
{
	return strcmp(String(), string.String()) < 0;
}

inline bool 
BString::operator<=(const BString &string) const
{
	return strcmp(String(), string.String()) <= 0;
}

inline bool 
BString::operator>=(const BString &string) const
{
	return strcmp(String(), string.String()) >= 0;
}

inline bool 
BString::operator>(const BString &string) const
{
	return strcmp(String(), string.String()) > 0;
}

inline bool 
BString::operator!=(const BString &string) const
{
	return strcmp(String(), string.String()) != 0;
}

inline bool 
BString::operator!=(const char *str) const
{
	return !operator==(str);
}

inline void
BSwap(BString& v1, BString& v2)
{
	v1.Swap(v2);
}

inline int32
BCompare(const BString& v1, const BString& v2)
{
	return strcmp(v1.String(), v2.String());
}

inline void
swap(BString& x, BString& y)
{
	x.Swap(y);
}

inline bool 
operator<(const char *str, const BString &string)
{
	return string > str;
}

inline bool 
operator<=(const char *str, const BString &string)
{
	return string >= str;
}

inline bool 
operator==(const char *str, const BString &string)
{
	return string == str;
}

inline bool 
operator>(const char *str, const BString &string)
{
	return string < str;
}

inline bool 
operator>=(const char *str, const BString &string)
{
	return string <= str;
}

inline bool 
operator!=(const char *str, const BString &string)
{
	return string != str;
}

} }	// namespace B::Support2

#endif /* _SUPPORT2_STRING_H */
