/***************************************************************************
//
//	File:			support2/Value.h
//
//	Description:	A general-purpose data container.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _SUPPORT2_VALUE_H_
#define _SUPPORT2_VALUE_H_

#include <support2/SupportDefs.h>
#include <support2/StaticValue.h>
#include <support2/IByteStream.h>
#include <support2/ITextStream.h>
#include <support2/IBinder.h>
#include <support2/ByteOrder.h>
#include <support2/TypeConstants.h>

#include <string.h>

namespace B {
namespace Support2 {

class BParcel;
class BFlattenable;
class BSharedBuffer;
class BString;
class BValueMap;
class BValue;

//!	Flags for BValue operations.
enum {
	//! don't restructure to avoid complex keys?
	B_NO_VALUE_FLATTEN			= 0x00000001,
	//!	don't traverse into nested values?
	B_NO_VALUE_RECURSION		= 0x00000002
};

//!	Flags for BValue::LockData().
enum {
	//!	keep existing data as-is.
	B_EDIT_VALUE_DATA		= 0x00000001
};

//!	Additional flags for PrintToStream().
enum {
	B_PRINT_VALUE_TYPES			= 0x00010000,	//!< Print full type information?
	B_PRINT_CATALOG_CONTENTS	= 0x00020000,	//!< Print contents of a context catalog?
	B_PRINT_BINDER_INTERFACES	= 0x00040000,	//!< Print interfaces implemented by a binder?
	B_PRINT_BINDER_CONTENTS		= 0x00080000	//!< Print properties in a binder?
};

//!	Some useful constants.
extern value_csml	B_EMPTY_STRING;
extern value_cbool	B_FALSE_BOOL;
extern value_cbool	B_TRUE_BOOL;
extern value_cint32	B_0_INT32;
extern value_cint32	B_1_INT32;
extern value_cint32	B_2_INT32;
extern value_cint32	B_3_INT32;
extern value_cint32	B_4_INT32;
extern value_cint32	B_5_INT32;
extern value_cint32	B_6_INT32;
extern value_cint32	B_7_INT32;
extern value_cint32	B_8_INT32;
extern value_cint32	B_9_INT32;
extern value_cint32	B_10_INT32;
extern value_cfloat	B_0_0_FLOAT;
extern value_cfloat	B_0_1_FLOAT;
extern value_cfloat	B_0_2_FLOAT;
extern value_cfloat	B_0_25_FLOAT;
extern value_cfloat	B_0_3_FLOAT;
extern value_cfloat	B_0_4_FLOAT;
extern value_cfloat	B_0_5_FLOAT;
extern value_cfloat	B_0_6_FLOAT;
extern value_cfloat	B_0_7_FLOAT;
extern value_cfloat	B_0_75_FLOAT;
extern value_cfloat	B_0_8_FLOAT;
extern value_cfloat	B_0_9_FLOAT;
extern value_cfloat	B_1_0_FLOAT;

//	--------------------------------------------------------------------

//!	A value_ref represents a single, type pile of bits.
/*!
	An instance of a value_ref does not own the data to which it refers --
	you must guarantee that the data exists for the lifetime of any value_ref
	pointing to it.  Generally a value_ref is only used as a transient
	object, such as to convert a string in to something that can be
	used with various BValue methods.  See BValue for more information
	on undefined and wild values, comparison, etc.
	\sa BValue
*/
struct value_ref
{
			type_code			type;
			ssize_t				length;
			const void*			data;
	
			//!	Note that the default constructor \e DOES \e NOT
			//!	initialize the value_ref's fields.
								value_ref();
								
			//!	Create value_ref with an explicit initial value.
								value_ref(const value_ref& o);
								value_ref(type_code t, const void* d, size_t l);
								
			//! Create a value_ref for standard types.
			/*! Doing a \c value_ref(1) will work if you are careful, as
				a temporary integer will be created which exists for the
				rest of the scope the value_ref is in.
			*/
								value_ref(const char* string);
								value_ref(const int& integer);
								
			//!	Create a value_ref from a BValue().
			/*!	This constructor is explicit because information may be lost --
				not all BValue data can be turned in to a value_ref.
			*/
	explicit					value_ref(const BValue& o);
	
								~value_ref();
	
			//!	Not-a-value.
	static	const value_ref		undefined;
			//!	The all-encompassing value.
	static	const value_ref		wild;
			
			value_ref&			operator=(const value_ref& o);
	
			//!	Comparison that creates a "nice" (but undefined) order.
			int32				compare(const value_ref& o) const;
			//!	Comparison that is fast is well-defined.
			int32				fast_compare(const value_ref& o) const;
			
			//!	Synonyms for compare().
			//!	\sa compare
	inline	bool				operator==(const value_ref& o) const	{ return compare(o) == 0; }
	inline	bool				operator!=(const value_ref& o) const	{ return compare(o) != 0; }
	inline	bool				operator<(const value_ref& o) const		{ return compare(o) < 0; }
	inline	bool				operator<=(const value_ref& o) const	{ return compare(o) <= 0; }
	inline	bool				operator>=(const value_ref& o) const	{ return compare(o) >= 0; }
	inline	bool				operator>(const value_ref& o) const		{ return compare(o) > 0; }
			
			//!	Write a textual description of the value_ref
			//!	to the text stream \a io.
			status_t			print_to_stream(ITextOutput::arg io, uint32 flags = 0) const;

			//!	value_ref pretty printer function.
			/*!	\param	io		The stream to print in.
				\param	val		The value to print.
				\param	flags	Additional formatting information.
			*/
	typedef	status_t			(*print_func)(	ITextOutput::arg io,
												const value_ref& val,
												uint32 flags);
	
			//!	Add a new printer function for a type code.
			/*!	When value_ref::print_to_stream() or BValue::PrintToStream() is
				called, values of the given \a type will be printed by \a func.
			*/
	static	status_t			register_print_func(type_code type, print_func func);
			//!	Remove printer function for type code.
	static	status_t			unregister_print_func(type_code type, print_func func=NULL);
	
private:
	friend	class				BValue;

			const void*			object;
};

ITextOutput::arg operator<<(ITextOutput::arg io, const value_ref& value);

// --------------------------------------------------------------------

//!	The BValue is a general-purpose data container.
/*!	BValue is formally an abstract group of (key->value) mappings, where
	each key and value can be any typed blob of data.  Standard data types
	are defined for strings, integers, Binder objects, and other common data
	types.  In addition, there are two special values: "undefined" is a
	BValue that is not set; "wild" is a special value that can be thought of
	as being every possible value all at the same time.

	Note that the definition of BValue is very recursive -- it is a
	collection of key/value mappings, where both the key and value are
	themselves a BValue.  As such, a BValue containing a single data
	item is conceptually actually the mapping \c (wild->value); because of
	the properties of wild, a wild key can appear more than one time
	so that a set of the values \c { A, B } can be constructed in a BValue
	as the mapping \c { (wild->A), (wild->B) }.  When iterating over and
	thinking of operations on a value, it is always viewed as a map in
	this way -- however, there are many convenience functions for working
	with a value in its common form of a single \c (wild->A) item.
*/
class BValue
{
public:
							BValue();
							BValue(const value_ref& o);
							BValue(const BValue& o);
							
			//!	Create a value for a single explicit mapping.
							BValue(const BValue& key, const BValue& value);
							BValue(const BValue& key, const BValue& value, uint32 flags);
							BValue(const value_ref& key, const BValue& value);
							BValue(const BValue& key, const value_ref& value);
							BValue(const BValue& key, const value_ref& value, uint32 flags);
							BValue(const value_ref& key, const value_ref& value);
							
			//!	Create specific value types, implicitly (wild->A)
							BValue(type_code type, const void* data, size_t len);
							BValue(type_code type, const BSharedBuffer* buffer);
							BValue(const BString& str);
							BValue(IBinder::sptr_ref binder);
							BValue(IBinder::wptr_ref binder);
							
							~BValue();
	
	/*	The two "special" values */
	
			//! not-a-value
	static	value_csml		undefined;
			//!	the all-encompassing value
	static	value_csml		wild;
	
	/*	Generating standard values */
	
	static	const BValue&	Undefined();
	static	const BValue&	Wild();
	static	BValue			Binder(IBinder::sptr_ref value);
	static	BValue			WeakBinder(IBinder::wptr_ref value);
	static	BValue			String(const char *value);
	static	BValue			String(const BString& value);
	static	BValue			Int8(int8 value);
	static	BValue			Int16(int16 value);
	static	BValue			Int32(int32 value);
	static	BValue			Int64(int64 value);
	static	BValue			Time(bigtime_t value);
	static	BValue			Bool(bool value);
	static	BValue			Float(float value);
	static	BValue			Double(double value);
	static	BValue			Flat(const BFlattenable& flat);
	static	BValue			Atom(const atom_ptr<BAtom>& value);
	static	BValue			WeakAtom(const atom_ref<BAtom>& value);
	static	BValue			TypeInfo(const std::type_info& value);
	static	BValue			Status(status_t error);
	
	static	BValue			SSize(ssize_t value);
	static	BValue			Offset(off_t value);

	/* Assignment, status, existance */
	
			status_t		ErrorCheck() const;
			void			SetError(status_t error);
			
			BValue&			Assign(const BValue& o);
			BValue&			Assign(const value_ref& o);
			BValue&			Assign(type_code type, const void* data, size_t len);
	inline	BValue&			operator=(const BValue& o)			{ return Assign(o); }
	inline	BValue&			operator=(const value_ref& o)		{ return Assign(o); }

			//! Move value backward in memory (for implementation of BMoveBefore).
	static	void			MoveBefore(BValue* to, BValue* from, size_t count = 1);
			//! Move value forward in memory (for implementation of BMoveAfter).
	static	void			MoveAfter(BValue* to, BValue* from, size_t count = 1);

			//!	Exchange contents of 'this' and 'with'.
			void			Swap(BValue& with);
			
			//!	Set value to 'undefined'.
			void			Undefine();
			//!	Check whether the value is not 'undefined'.
			bool			IsDefined() const;
			/*!	\note This is really IsDefined() -- it returns a
				non-NULL value if the value is defined.
				\sa IsDefined
			*/
							operator const void*() const;
	
			//!	Checking whether this value is 'wild'.
			bool			IsWild() const;
			
			//!	Check whether this value is neither 'undefined' nor 'wild'.
			bool			IsSpecified() const;
			
			//!	Check whether this value is a single item of the form (wild->A).
			/*!	A value_ref can only represent this kind of value.
			*/
			bool			IsSimple() const;
			
			status_t		CanByteSwap() const;
			status_t		ByteSwap(swap_action action);
			
	/* Raw data characteristics. */
	
			//!	Raw access.
			/*!	\note Data() and Length() are not valid if this object contains
				anything besides a simple values (i.e., two values in a set or
				one or more (key->value) mappings).
			*/
			type_code		Type() const;
			const void*		Data() const;
			size_t			Length() const;
			const BSharedBuffer*	SharedBuffer() const;
			
			//!	Raw editing of data in value.
			/*!	Only valid on values for which IsSimple() is true.
				\sa IsSimple
			*/
			void*			BeginEditBytes(type_code type, size_t length, uint32 flags=0);
			status_t		EndEditBytes(ssize_t final_length=-1);
			
	/* Mapping operations */
	
			//!	Combine contents to two values together.
			/*!	Joins all mappings in both values to create the final
				result.  Duplicate terminal values will create sets.
				
				For example:
					{ (A->B), (D->E) }.Join( { (A->G), (B->I) } )
				
				Results in:
					{ (A->(B,G)), (B->I), (D->E) }
				
				Note that key A now contains the contents of both mappings,
				resulting in a set.
			*/
			BValue&			Join(const BValue& from, uint32 flags = 0);
			const BValue	JoinCopy(const BValue& from, uint32 flags = 0) const;
			
			//!	Convenience functions for overlaying a single map item.
			/*!	\note JoinItem(wild, A) is the same as Join(A).
			*/
			BValue&			JoinItem(const BValue& key, const BValue& value, uint32 flags=0);
			BValue&			JoinItem(const value_ref& key, const BValue& value, uint32 flags=0);
			BValue&			JoinItem(const BValue& key, const value_ref& value, uint32 flags=0);
			BValue&			JoinItem(const value_ref& key, const value_ref& value, uint32 flags=0);
			
			//!	Apply mappings on top of this value.
			/*!	Overwrites any existing mappings in the value that are the
				same as the new mappings.
				
				For example:
					{ (A->B), (D->E) }.Overlay( { (A->G), (B->I) } )
				
				Results in:
					{ (A->G), (B->I), (D->E) }
				
				Note that key A contains the value from the right hand side.
			*/
			BValue&			Overlay(const BValue& from, uint32 flags = 0);
			const BValue	OverlayCopy(const BValue& from, uint32 flags = 0) const;
			
			//!	Apply mappings underneath this value.
			/*!	Does not modify any existing mappings in the value.
			
				For example:
					{ (A->B), (D->E) }.Inherit( { (A->G), (B->I) } )
				
				Results in:
					{ (A->B), (B->I), (D->E) }
				
				Note that key A contains the value from the left hand side.
			*/
			BValue&			Inherit(const BValue& from, uint32 flags = 0);
			const BValue	InheritCopy(const BValue& from, uint32 flags = 0) const;
			
			//!	Perform a value remapping operation.
			/*!	This operation is defined as
				(A->B).MapValues((D->E)) == (A->((D->E)[B])).  This is performed for
				every mapping in 'this', the results of which are aggregated
				using Join().
				
				For example:
					{ (A->B), (D->E) }.MapValues( { (A->G), (B->I) } )
				
				Results in:
					{ (A->I) }
			*/
			BValue&			MapValues(const BValue& from, uint32 flags = 0);
			const BValue	MapValuesCopy(const BValue& from, uint32 flags = 0) const;
	
			//!	Remove mappings from this value.
			/*!	For every mapping in \a from, if that \e mapping appears in
				\a this then it is removed.  To remove an entire key, use
				the mapping (key->wild).  This is equivalent to a set
				subtraction.
			*/
			BValue&			Remove(const BValue& from, uint32 flags = 0);
			const BValue	RemoveCopy(const BValue& from, uint32 flags = 0) const;
			status_t		RemoveItem(	const value_ref& key,
										const value_ref& value = value_ref::wild);
			
			//! Keep mappings in this value.
			/*!	For every mapping in \a this, if that mapping \e doesn't appear
				in \a from then it is removed.  This is equivalent to a set
				intersection.
			*/
			BValue&			Retain(const BValue& from, uint32 flags = 0);
			const BValue	RetainCopy(const BValue& from, uint32 flags = 0) const;
			
			//!	Change the name of a key in the value.
			/*!	\note The old and new keys must be a specified value --
				you can not rename using wild values.
				\sa IsSpecified
			*/
			status_t		RenameItem(	const value_ref& old_key,
										const BValue& new_key);
			
			//!	Check whether this value contains the given mapping.
			bool			HasItem(const BValue& key,
									const BValue& value = BValue::undefined) const;
			bool			HasItem(const value_ref& key,
									const value_ref& value = value_ref::undefined) const;
			
			//!	Look up a key in this value.
			/*!	Looking up a wild key returns the entire value as-is.
				Looking up with a key that is a map will match all items
				of the map.
				
				For example:
					{ (A->(B->(D->E))) }.ValueFor( { (A->(B->D)) } )
				
				Results in:
					{ E }
			*/
			const BValue	ValueFor(const BValue& key, uint32 flags = 0) const;
			const BValue	ValueFor(const value_ref& key, uint32 flags = 0) const;
	
			//!	Count the number of mappings in the value.
			int32			CountItems() const;
			//!	Iterate over the mappings in the value.
			status_t		GetNextItem(void** cookie, BValue* out_key, BValue* out_value) const;
			
			//! Direct editing of sub-items in the value.
			/*!	Only valid for a value that contains full mappings (A->B);
				\a key must be a simple value of one of these mappings.
			*/
			BValue*			BeginEditItem(const BValue &key);
			BValue*			BeginEditItem(const value_ref &key);
			void			EndEditItem(BValue* item);
			
			/*!	\sa ValueFor */
	inline	const BValue	operator[](const value_ref& key) const	{ return ValueFor(key); }
	inline	const BValue	operator[](const BValue& key) const		{ return ValueFor(key); }
	
			/*!	\sa JoinCopy */
	inline	const BValue	operator+(const BValue& o) const		{ return JoinCopy(o); }
			/*!	\sa RemoveCopy */
	inline	const BValue	operator-(const BValue& o) const		{ return RemoveCopy(o); }
			/*!	\sa MapValuesCopy */
	inline	const BValue	operator*(const BValue& o) const		{ return MapValuesCopy(o); }

			/*!	\sa Join */
	inline	BValue&			operator+=(const BValue& o)				{ return Join(o); }
			/*!	\sa Remove */
	inline	BValue&			operator-=(const BValue& o)				{ return Remove(o); }
			/*!	\sa MapValues */
	inline	BValue&			operator*=(const BValue& o)				{ return MapValues(o); }
	
	/* Archiving */
	
			ssize_t			ArchivedSize() const;
			ssize_t			Archive(BParcel& into) const;
			ssize_t			Archive(IByteOutput::sptr_ref into) const;
			ssize_t			Unarchive(BParcel& from);
			ssize_t			Unarchive(IByteInput::sptr_ref from);
			
	/* Comparison */
	
			//!	Perform a comparison between two values.
			/*!	This comparison is smart about types, trying to get standard
				types in their natural order.  Because of this, note that the
				relative ordering of Compare() WILL CHANGE between operating
				system releases.
			*/
			int32			Compare(const BValue& o) const;
			int32			Compare(const value_ref& o) const;
			
			//!	Perform a quick comparison of values.
			/*!	Not only is this fast, but the way values are compared is
				type-independent and thus will never change.
			*/
			int32			FastCompare(const BValue& o) const;
			int32			FastCompare(const value_ref& o) const;
			
	inline	bool			operator==(const BValue& o) const		{ return FastCompare(o) == 0; }
	inline	bool			operator!=(const BValue& o) const		{ return FastCompare(o) != 0; }
	inline	bool			operator<(const BValue& o) const		{ return Compare(o) < 0; }
	inline	bool			operator<=(const BValue& o) const		{ return Compare(o) <= 0; }
	inline	bool			operator>=(const BValue& o) const		{ return Compare(o) >= 0; }
	inline	bool			operator>(const BValue& o) const		{ return Compare(o) > 0; }
	
	inline	bool			operator==(const value_ref& o) const	{ return FastCompare(o) == 0; }
	inline	bool			operator!=(const value_ref& o) const	{ return FastCompare(o) != 0; }
	inline	bool			operator<(const value_ref& o) const		{ return Compare(o) < 0; }
	inline	bool			operator<=(const value_ref& o) const	{ return Compare(o) <= 0; }
	inline	bool			operator>=(const value_ref& o) const	{ return Compare(o) >= 0; }
	inline	bool			operator>(const value_ref& o) const		{ return Compare(o) > 0; }
			
	/* Viewing the value as a standard type, potentially performing a conversion */
	
			//!	Convert value to a strong IBinder pointer.
			/*!	B_BINDER_TYPE: Returned as-is.
				B_BINDER_HANDLE_TYPE: Returns binder proxy.
				B_WEAK_BINDER_TYPE: Attempts to promote to strong pointer.
				B_WEAK_BINDER_HANDLE_TYPE: Retrieve proxy and attempt to promote.
			*/
			IBinder::sptr	AsBinder(status_t* result = NULL) const;
			
			//!	Convert value to a weak IBinder pointer.
			/*!	B_BINDER_TYPE: Returned as a weak pointer.
				B_BINDER_HANDLE_TYPE: Returns binder proxy as a weak pointer.
				B_WEAK_BINDER_TYPE: Returns as-is.
				B_WEAK_BINDER_HANDLE_TYPE: Returns binder proxy.
			*/
			IBinder::wptr	AsWeakBinder(status_t* result = NULL) const;
			
			//!	Convert value to a BString.
			/*!	B_STRING_TYPE: Returned as-is.
				B_DOUBLE_TYPE: Converted to a string using "%g".
				B_FLOAT_TYPE: Converted to a string using "%g".
				B_INT64_TYPE, B_BIGTIME_TYPE: Converted using "%Ld".
				B_INT32_TYPE: Convered to a string using "%ld".
				B_INT16_TYPE, B_INT8_TYPE: Converted using "%d".
				B_BOOL_TYPE: Converted to the string "true" or "false".
				B_ATOM_TYPE: Converted to string with the address.
				B_ATOM_WEAK_TYPE: Converted to string with the address.
				B_NULL_TYPE: Converted to the string "<wild>".
			*/
			BString			AsString(status_t* result = NULL) const;
			
			//!	Convert value to boolean.
			/*!	B_BOOL_TYPE: Returned as-is.
				B_STRING_TYPE: Converted to true if "true" or numeric > 0.
				B_DOUBLE_TYPE, B_FLOAT_TYPE, B_INT64_TYPE, B_BIGTIME_TYPE,
					B_INT32_TYPE, B_INT16_TYPE, B_INT8_TYPE:
					Converted to true if value is not zero.
				B_ATOM_TYPE: Converted to true if address is not NULL.
				B_ATOM_WEAK_TYPE: Converted to true if address is not NULL.
			*/
			bool			AsBool(status_t* result = NULL) const;
			
			//!	Convert value to int32.
			/*!	B_INT32_TYPE: Returned as-is.
				B_STRING_TYPE: Converted using strtol().
				B_DOUBLE_TYPE, B_FLOAT_TYPE, B_INT64_TYPE, B_BIGTIME_TYPE,
					B_INT16_TYPE, B_INT8_TYPE:
					Converted to an int32, losing precision as needed.
					No bounds checking is performed.
				B_BOOL_TYPE: Returned a 1 or 0.
				B_ATOM_TYPE: Converted to an integer containing the address.
				B_ATOM_WEAK_TYPE: Converted to integer containing the address.
			*/
			int32			AsInt32(status_t* result = NULL) const;
			
			//!	Convert value to int64.
			/*!	B_INT64_TYPE: Returned as-is.
				B_STRING_TYPE: Converted using strtoll().
				B_DOUBLE_TYPE, B_FLOAT_TYPE, B_BIGTIME_TYPE,
					B_INT32_TYPE, B_INT16_TYPE, B_INT8_TYPE:
					Converted to an int64, losing precision as needed.
					No bounds checking is performed.
				B_BOOL_TYPE: Returned a 1 or 0.
				B_ATOM_TYPE: Converted to an integer containing the address.
				B_ATOM_WEAK_TYPE: Converted to integer containing the address.
			*/
			int64			AsInt64(status_t* result = NULL) const;
			
			//!	Convert value to bigtime_t.
			/*!	B_BIGTIME_TYPE: Returned as-is.
				B_STRING_TYPE: Converted using strtoll().
				B_DOUBLE_TYPE, B_FLOAT_TYPE, B_INT64_TYPE,
					B_INT32_TYPE, B_INT16_TYPE, B_INT8_TYPE:
					Converted to a bigtime_t, losing precision as needed.
					No bounds checking is performed.
				B_BOOL_TYPE: Returned a 1 or 0.
				B_ATOM_TYPE: Converted to an integer containing the address.
				B_ATOM_WEAK_TYPE: Converted to integer containing the address.
			*/
			bigtime_t		AsTime(status_t* result = NULL) const;
			
			//!	Convert value to float.
			/*!	B_FLOAT_TYPE: Returned as-is.
				B_STRING_TYPE: Converted using strtod().
				B_DOUBLE_TYPE, B_INT64_TYPE, B_BIGTIME_TYPE,
					B_INT32_TYPE, B_INT16_TYPE, B_INT8_TYPE:
					Converted to a float, losing precision as needed.
					No bounds checking is performed.
				B_BOOL_TYPE: Returned a 1 or 0.
			*/
			float			AsFloat(status_t* result = NULL) const;
			
			//!	Convert value to double.
			/*!	B_DOUBLE_TYPE: Returned as-is.
				B_STRING_TYPE: Converted using strtod().
				B_FLOAT_TYPE, B_INT64_TYPE, B_BIGTIME_TYPE,
					B_INT32_TYPE, B_INT16_TYPE, B_INT8_TYPE:
					Converted to a double, losing precision as needed.
				B_BOOL_TYPE: Returned a 1 or 0.
			*/
			double			AsDouble(status_t* result = NULL) const;
			
			//!	Convert value to a strong BAtom pointer.
			/*!	B_ATOM_TYPE: Returned as-is.
				B_ATOM_WEAK_TYPE: Attempts to promote to strong pointer.
			*/
			atom_ptr<BAtom>	AsAtom(status_t* result = NULL) const;
			
			//!	Convert value to a weak BAtom pointer.
			/*!	B_ATOM_TYPE: Returned as a weak pointer.
				B_ATOM_WEAK_TYPE: Returns as-is.
			*/
			atom_ref<BAtom>	AsWeakAtom(status_t* result = NULL) const;
	
			//!	Convert value to a status code.
			/*!	If the value's ErrorCheck() is not B_OK, this is returned.
				If AsInt32() succeeds, (val < B_OK ? val : B_OK) is returned.
				Otherwise, B_BAD_TYPE is returned.
			*/
			status_t		AsStatus(status_t* result = NULL) const;
			
			//!	Convert value to a status/size code.
			/*!	If the value's ErrorCheck() is not B_OK, this is returned.
				If AsInt32() succeeds, its value is returned.
				Otherwise, B_BAD_TYPE is returned.
			*/
			ssize_t			AsSSize(status_t* result = NULL) const;
			
			//!	Synonym for AsInt64().
			off_t			AsOffset(status_t* result = NULL) const;
			
			/*! \deprecated Use AsInt32() instead. */
	inline	int32			AsInteger(status_t* result = NULL) const { return AsInt32(result); }
			
	/* Retrieving basic value as an explicit standard type */
	
			status_t		GetBinder(IBinder::sptr *obj) const;
			status_t		GetWeakBinder(IBinder::wptr *obj) const;
			status_t		GetString(const char** a_string) const;
			status_t		GetString(BString* a_string) const;
			status_t		GetBool(bool* val) const;
			status_t		GetInt8(int8* val) const;
			status_t		GetInt16(int16* val) const;
			status_t		GetInt32(int32* val) const;
			status_t		GetInt64(int64* val) const;
			status_t		GetTime(bigtime_t* val) const;
			status_t		GetFloat(float* a_float) const;
			status_t		GetDouble(double* a_double) const;
			status_t		GetFlat(BFlattenable* obj) const;
			status_t		GetAtom(atom_ptr<BAtom>* atom) const;
			status_t		GetWeakAtom(atom_ref<BAtom>* atom) const;

			//!	Print the value's contents in a pretty format.
			status_t		PrintToStream(ITextOutput::arg io, uint32 flags = 0) const;
			
			struct examine_data {
				value_ref	value;
				bool		is_defined : 1;
				bool		is_wild : 1;
				bool		is_simple : 1;
			};
			
			void			GetExamineData(examine_data* out, size_t len=sizeof(examine_data)) const;
			
private:
	
	friend	class			value_ref;
	friend	class			BValueMap;
	
			void			free_data();
			void*			alloc_data(size_t len);
			void			init_as_map(const BValue& key, const BValue& value, uint32 flags);
			void			init_as_shared_buffer(type_code type, const BSharedBuffer* buffer);
			void			get_value_ref(value_ref* out) const;
			bool			is_error() const;
			bool			is_map() const;
			bool			is_final() const;	// wild or error
			status_t		remove_item_index(size_t index);
			status_t		find_item_index(size_t index,
											BValue* out_key, BValue* out_value) const;
			void			shrink_map(BValueMap* map);
			void*			edit_data(size_t len);
			status_t		copy_data(type_code type, void* buf, size_t len) const;
			ssize_t			ArchivedSize(uint32 flags) const;
			ssize_t			Archive(BParcel& into, uint32 flags) const;
			ssize_t			Unarchive(BParcel& from, size_t amount, uint32 flags);
			BValueMap*		edit_map();
			BValueMap*		make_map_without_sets();
			status_t		set_error(ssize_t code);
			bool			check_integrity() const;
			
			struct value_ptr {
				// if m_length >= B_OK, this is set to the raw flattened data.
				const BSharedBuffer*	flat;
				
				// if m_length == OBJECT_MAGIC_LENGTH, this is set to the object.
				const BValueMap*		map;
			};
			
			type_code		m_type;
			ssize_t			m_length;
			union {
				value_ptr	heap;
				uint8		local[8];
			}				m_data;
};

/*----- Type and STL utilities --------------------------------------*/
void			BMoveBefore(BValue* to, BValue* from, size_t count = 1);
void			BMoveAfter(BValue* to, BValue* from, size_t count = 1);
void			BSwap(BValue& v1, BValue& v2);
int32			BCompare(const BValue& v1, const BValue& v2);
void			swap(BValue& x, BValue& y);

ITextOutput::arg	operator<<(ITextOutput::arg io, const BValue& value);

/*-------------------------------------------------------------*/
/*---- No user serviceable parts after this -------------------*/

inline value_ref::value_ref()
	{ }
inline value_ref::value_ref(const value_ref& o)
	: type(o.type), length(o.length), data(o.data), object(o.object) { }
inline value_ref::value_ref(type_code t, const void* d, size_t l)
	: type(t), length(l), data(d), object(NULL) { }
inline value_ref::value_ref(const char* string)
	: type(B_STRING_TYPE), length(string?strlen(string)+1:1), data(string?string:""), object(NULL) { }
inline value_ref::value_ref(const int& integer)
	: type(B_INT32_TYPE), length(sizeof(integer)), data(&integer), object(NULL) { }
inline value_ref::~value_ref()
	{ }

inline value_ref& value_ref::operator=(const value_ref& o)
{
	type = o.type;
	length = o.length;
	data = o.data;
	object = o.object;
	return *this;
}

inline const BValue& BValue::Undefined()				{ return undefined; }
inline const BValue& BValue::Wild()						{ return wild; }
	
inline int32 BValue::Compare(const value_ref& o) const
{
	return value_ref(*this).compare(o);
}

inline int32 BValue::FastCompare(const value_ref& o) const
{
	return value_ref(*this).fast_compare(o);
}

inline BValue & BValue::JoinItem(const value_ref& key, const value_ref& value, uint32 flags)
{
	return JoinItem(key, BValue(value), flags);
}

inline BValue & BValue::JoinItem(const BValue& key, const value_ref& value, uint32 flags)
{
	return JoinItem(key, BValue(value), flags);
}

inline BValue BValue::SSize(ssize_t value)
{
	return Int32(value);
}

inline BValue BValue::Offset(off_t value)
{
	return Int64(value);
}

inline off_t BValue::AsOffset(status_t* result) const
{
	return AsInt64(result);
}

inline void BMoveBefore(BValue* to, BValue* from, size_t count)
{
	BValue::MoveBefore(to, from, count);
}

inline void BMoveAfter(BValue* to, BValue* from, size_t count)
{
	BValue::MoveAfter(to, from, count);
}

inline void BSwap(BValue& v1, BValue& v2)
{
	v1.Swap(v2);
}

inline int32 BCompare(const BValue& v1, const BValue& v2)
{
	return v1.Compare(v2);
}

inline void swap(BValue& x, BValue& y)
{
	x.Swap(y);
}

} }	// namespace B::Support2

#endif	/* _SUPPORT2_VALUE_H_ */
