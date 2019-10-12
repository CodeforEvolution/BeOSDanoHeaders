/***************************************************************************
//
//	File:			support2/StaticValue.h
//
//	Description:	Experiments with static BValue representations.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _SUPPORT2_STATIC_VALUE_H_
#define _SUPPORT2_STATIC_VALUE_H_

#include <support2/SupportDefs.h>
#include <support2/SharedBuffer.h>
#include <support2/TypeConstants.h>

namespace B {
namespace Support2 {

class BValue;

//	--------------------------------------------------------------------

//!	This is a static representation of a BValue containing 8 or fewer bytes
//!	of data (with this data placed in-line), which can be used to create
//!	constant values for placement in the text section of an executable.
struct static_small_value
{
	type_code	type;
	ssize_t		length;
	char		data[8];
	
	inline operator const BValue&() const { return *reinterpret_cast<const BValue*>(this); }
};

typedef const static_small_value value_csml;

//!	This is a static representation of a BValue containing more than 8 bytes
//!	of data (pointing to its data out-of-line), which can be used to create
//!	constant values for placement in the text section of an executable.
/*!	\sa static_string_buffer_12
	\sa static_string_buffer_16
	\sa static_string_buffer_20
	\sa static_string_buffer_24
*/
struct static_large_value
{
	type_code	type;
	ssize_t		length;
	const void*	data;
	
	inline operator const BValue&() const { return *reinterpret_cast<const BValue*>(this); }
};

typedef const static_large_value value_clrg;

//!	This is a static representation of a BValue containing a 32-bit
//!	integer, which can be used to create constant values for placement
//! in the text section of an executable.
struct static_int32_value
{
	type_code	type;
	ssize_t		length;
	int32		value;
	
	inline operator const BValue&() const { return *reinterpret_cast<const BValue*>(this); }
};

typedef const static_int32_value value_cint32;

//!	This is a static representation of a BValue containing a 32-bit
//!	float, which can be used to create constant values for placement
//! in the text section of an executable.
struct static_float_value
{
	type_code	type;
	ssize_t		length;
	float		value;
	
	inline operator const BValue&() const { return *reinterpret_cast<const BValue*>(this); }
};

typedef const static_float_value value_cfloat;

//!	This is a static representation of a BValue containing a boolean,
//!	which can be used to create constant values for placement
//! in the text section of an executable.
struct static_bool_value
{
	type_code	type;
	ssize_t		length;
	bool		value;
	
	inline operator const BValue&() const { return *reinterpret_cast<const BValue*>(this); }
};

typedef const static_bool_value value_cbool;

//	--------------------------------------------------------------------

//!	Convenience macro for making a static BValue containing a string of
//!	8 or fewer (including the terminating \0) characters.
#define B_CONST_STRING_VALUE_8(prefix, ident, length, string)		\
	const static_small_value prefix##ident = {						\
		B_STRING_TYPE, length, string								\
	};																\

//!	Convenience macro for making a static BValue containing a string of
//!	8 or fewer (including the terminating \0) characters.
#define B_STATIC_STRING_VALUE_8(prefix, ident, length, string)		\
	static const static_small_value prefix##ident = {				\
		B_STRING_TYPE, length, string								\
	};																\

//!	Convenience macro for making a static BValue containing a string of
//!	12 or fewer (including the terminating \0) characters.
#define B_CONST_STRING_VALUE_12(prefix, ident, length, string)		\
	static const static_string_buffer_12 ident##str = {				\
		B_STATIC_USERS, 0, 0, length, string						\
	};																\
	const static_large_value prefix##ident = {						\
		B_STRING_TYPE, length, &ident##str							\
	};																\

//!	Convenience macro for making a static BValue containing a string of
//!	12 or fewer (including the terminating \0) characters.
#define B_STATIC_STRING_VALUE_12(prefix, ident, length, string)		\
	static const static_string_buffer_12 ident##str = {				\
		B_STATIC_USERS, 0, 0, length, string						\
	};																\
	static const static_large_value prefix##ident = {				\
		B_STRING_TYPE, length, &ident##str							\
	};																\

//!	Convenience macro for making a static BValue containing a string of
//!	16 or fewer (including the terminating \0) characters.
#define B_CONST_STRING_VALUE_16(prefix, ident, length, string)		\
	static const static_string_buffer_16 ident##str = {				\
		B_STATIC_USERS, 0, 0, length, string						\
	};																\
	const static_large_value prefix##ident = {						\
		B_STRING_TYPE, length, &ident##str							\
	};																\

//!	Convenience macro for making a static BValue containing a string of
//!	16 or fewer (including the terminating \0) characters.
#define B_STATIC_STRING_VALUE_16(prefix, ident, length, string)		\
	static const static_string_buffer_16 ident##str = {				\
		B_STATIC_USERS, 0, 0, length, string						\
	};																\
	static const static_large_value prefix##ident = {				\
		B_STRING_TYPE, length, &ident##str							\
	};																\

//!	Convenience macro for making a static BValue containing a string of
//!	20 or fewer (including the terminating \0) characters.
#define B_CONST_STRING_VALUE_20(prefix, ident, length, string)		\
	static const static_string_buffer_20 ident##str = {				\
		B_STATIC_USERS, 0, 0, length, string						\
	};																\
	const static_large_value prefix##ident = {						\
		B_STRING_TYPE, length, &ident##str							\
	};																\

//!	Convenience macro for making a static BValue containing a string of
//!	20 or fewer (including the terminating \0) characters.
#define B_STATIC_STRING_VALUE_20(prefix, ident, length, string)		\
	static const static_string_buffer_20 ident##str = {				\
		B_STATIC_USERS, 0, 0, length, string						\
	};																\
	static const static_large_value prefix##ident = {				\
		B_STRING_TYPE, length, &ident##str							\
	};																\

//!	Convenience macro for making a static BValue containing a string of
//!	24 or fewer (including the terminating \0) characters.
#define B_CONST_STRING_VALUE_24(prefix, ident, length, string)		\
	static const static_string_buffer_24 ident##str = {				\
		B_STATIC_USERS, 0, 0, length, string						\
	};																\
	const static_large_value prefix##ident = {						\
		B_STRING_TYPE, length, &ident##str							\
	};																\

//!	Convenience macro for making a static BValue containing a string of
//!	24 or fewer (including the terminating \0) characters.
#define B_STATIC_STRING_VALUE_24(prefix, ident, length, string)		\
	static const static_string_buffer_24 ident##str = {				\
		B_STATIC_USERS, 0, 0, length, string						\
	};																\
	static const static_large_value prefix##ident = {				\
		B_STRING_TYPE, length, &ident##str							\
	};																\

//!	Convenience macro for making a static BValue containing a string of
//!	28 or fewer (including the terminating \0) characters.
#define B_CONST_STRING_VALUE_28(prefix, ident, length, string)		\
	static const static_string_buffer_28 ident##str = {				\
		B_STATIC_USERS, 0, 0, length, string						\
	};																\
	const static_large_value prefix##ident = {						\
		B_STRING_TYPE, length, &ident##str							\
	};																\

//!	Convenience macro for making a static BValue containing a string of
//!	28 or fewer (including the terminating \0) characters.
#define B_STATIC_STRING_VALUE_28(prefix, ident, length, string)		\
	static const static_string_buffer_28 ident##str = {				\
		B_STATIC_USERS, 0, 0, length, string						\
	};																\
	static const static_large_value prefix##ident = {				\
		B_STRING_TYPE, length, &ident##str							\
	};																\

//!	Convenience macro for making a static BValue containing a string of
//!	32 or fewer (including the terminating \0) characters.
#define B_CONST_STRING_VALUE_32(prefix, ident, length, string)		\
	static const static_string_buffer_32 ident##str = {				\
		B_STATIC_USERS, 0, 0, length, string						\
	};																\
	const static_large_value prefix##ident = {						\
		B_STRING_TYPE, length, &ident##str							\
	};																\

//!	Convenience macro for making a static BValue containing a string of
//!	32 or fewer (including the terminating \0) characters.
#define B_STATIC_STRING_VALUE_32(prefix, ident, length, string)		\
	static const static_string_buffer_32 ident##str = {				\
		B_STATIC_USERS, 0, 0, length, string						\
	};																\
	static const static_large_value prefix##ident = {				\
		B_STRING_TYPE, length, &ident##str							\
	};																\

//!	Convenience macro for making a static BValue containing an int32.
#define B_CONST_INT32_VALUE(prefix, ident, val)						\
	const static_int32_value prefix##ident = {						\
		B_INT32_TYPE, sizeof(int32), val							\
	};																\

//!	Convenience macro for making a static BValue containing an int32.
#define B_STATIC_INT32_VALUE(prefix, ident, val)					\
	static const static_int32_value prefix##ident = {				\
		B_INT32_TYPE, sizeof(int32), val							\
	};																\

//!	Convenience macro for making a static BValue containing a float.
#define B_CONST_FLOAT_VALUE(prefix, ident, val)						\
	const static_float_value prefix##ident = {						\
		B_FLOAT_TYPE, sizeof(float), val							\
	};																\

//!	Convenience macro for making a static BValue containing a float.
#define B_STATIC_FLOAT_VALUE(prefix, ident, val)					\
	static const static_float_value prefix##ident = {				\
		B_FLOAT_TYPE, sizeof(float), val							\
	};																\

//	--------------------------------------------------------------------

//!	Special shared buffer containing up to 12 bytes.
/*!	\sa static_large_value */
struct static_string_buffer_12
{
	int32		users;
	int32		pad0;
	int32		pad1;
	size_t		length;
	char		data[12];
};

//!	Special shared buffer containing up to 16 bytes.
/*!	\sa static_large_value */
struct static_string_buffer_16
{
	int32		users;
	int32		pad0;
	int32		pad1;
	size_t		length;
	char		data[16];
};

//!	Special shared buffer containing up to 20 bytes.
/*!	\sa static_large_value */
struct static_string_buffer_20
{
	int32		users;
	int32		pad0;
	int32		pad1;
	size_t		length;
	char		data[20];
};

//!	Special shared buffer containing up to 24 bytes.
/*!	\sa static_large_value */
struct static_string_buffer_24
{
	int32		users;
	int32		pad0;
	int32		pad1;
	size_t		length;
	char		data[24];
};

//!	Special shared buffer containing up to 28 bytes.
/*!	\sa static_large_value */
struct static_string_buffer_28
{
	int32		users;
	int32		pad0;
	int32		pad1;
	size_t		length;
	char		data[28];
};

//!	Special shared buffer containing up to 32 bytes.
/*!	\sa static_large_value */
struct static_string_buffer_32
{
	int32		users;
	int32		pad0;
	int32		pad1;
	size_t		length;
	char		data[32];
};

} }	// namespace B::Support2

#endif	/* _SUPPORT2_STATIC_VALUE_H_ */
