/******************************************************************************
/
/	File:			support2/TypeConstants.h
/
/	Description:	Constants that represent distinct data types, as used
/					by BValue and BValueMap.
/
/	Copyright 1993-2001, Be Incorporated
/
******************************************************************************/

#ifndef _SUPPORT2_TYPECONSTANTS_H
#define _SUPPORT2_TYPECONSTANTS_H

#ifdef __cplusplus
namespace B {
namespace Support2 {
#endif

/*-------------------------------------------------------------*/
/*----- Data Types --------------------------------------------*/

enum {
	// Special code to match any type code in comparisons.
	B_ANY_TYPE 					= 'ANYT',
	
	// Special system types.
	B_UNDEFINED_TYPE			= 0,
	B_WILD_TYPE					= '_WLD',
	B_VALUE_MAP_TYPE			= '_VMP',
	B_SYSTEM_TYPE				= '_SYS',
	B_BINDER_NODE_TYPE			= '_BDN',
	B_BINDER_WEAK_NODE_TYPE		= '_BWN',
	
	// Binders and atoms.
	B_BINDER_TYPE				= 'BIND',
	B_BINDER_HANDLE_TYPE		= 'BNDH',
	B_BINDER_WEAK_TYPE			= 'BNDW',
	B_BINDER_WEAK_HANDLE_TYPE	= 'BDWH',
	B_ATOM_TYPE 				= 'ATOM',
	B_ATOM_WEAK_TYPE 			= 'ATMW',
	
	// Standard types.
	B_BOOL_TYPE 				= 'BOOL',
	B_INT8_TYPE 				= 'BYTE',
	B_INT16_TYPE 				= 'SHRT',
	B_INT32_TYPE 				= 'LONG',
	B_INT64_TYPE 				= 'LLNG',
	B_FLOAT_TYPE 				= 'FLOT',
	B_DOUBLE_TYPE 				= 'DBLE',
	B_POINT_TYPE 				= 'BPNT',
	B_RASTER_POINT_TYPE 		= 'RPNT',
	B_RECT_TYPE 				= 'RECT',
	B_RASTER_RECT_TYPE 			= 'RRCT',
	B_ENTRY_REF_TYPE 			= 'RREF',
	B_COLOR_32_TYPE 			= 'RGBC',
	B_COLOR_TYPE				= 'RGBF',
	B_STRING_TYPE 				= 'CSTR',
	B_BIGTIME_TYPE 				= 'BTIM',
	B_REGION_TYPE				= 'REGN',
	B_TRANSFORM_2D_TYPE			= 'TR2D',
	B_TRANSFORM_COLOR_TYPE		= 'TRCR',
	B_GRADIENT_TYPE				= 'GRAD',
	B_UPDATE_TYPE				= 'UPDR',
	B_CONSTRAINTS_TYPE			= 'CNST',
	B_FONT_TYPE					= 'FONt',
	B_RAW_TYPE 					= 'RAWT',
	
	// Questionable types.
	B_CHAR_TYPE 				= 'CHAR',
	B_MIME_TYPE					= 'MIME',
	B_OFF_T_TYPE 				= 'OFFT',
	B_SIZE_T_TYPE	 			= 'SIZT',
	B_SSIZE_T_TYPE	 			= 'SSZT',
	B_TIME_TYPE 				= 'TIME',
	B_UINT64_TYPE				= 'ULLG',
	B_UINT32_TYPE				= 'ULNG',
	B_UINT16_TYPE 				= 'USHT',
	B_UINT8_TYPE 				= 'UBYT',
	
	// Old types that aren't applicable to the new world.
	B_MESSAGE_TYPE				= 'MSGG',
	B_COLOR_8_BIT_TYPE 			= 'CLRB',
	B_GRAYSCALE_8_BIT_TYPE		= 'GRYB',
	B_MESSENGER_TYPE			= 'MSNG',
	B_MONOCHROME_1_BIT_TYPE 	= 'MNOB',
	B_OBJECT_TYPE 				= 'OPTR',
	B_PATTERN_TYPE 				= 'PATN',
	B_POINTER_TYPE 				= 'PNTR',
	B_RGB_32_BIT_TYPE 			= 'RGBB',
	B_MEDIA_PARAMETER_TYPE		= 'BMCT',
	B_MEDIA_PARAMETER_WEB_TYPE	= 'BMCW',
	B_MEDIA_PARAMETER_GROUP_TYPE= 'BMCG'
};

/*-------------------------------------------------------------*/

#ifdef __cplusplus
} }	// namespace B::Support2

#endif

#endif /* _SUPPORT2_TYPECONSTANTS_H */
