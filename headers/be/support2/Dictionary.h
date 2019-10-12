/******************************************************************************
//
//	File:			support2/Dictionary.h
//
//	Description:	Node within the hierarchical namespace associated
//                  with the IContexts
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
******************************************************************************/

#ifndef _SUPPORT2_DICTIONARY_H_
#define _SUPPORT2_DICTIONARY_H_

#include <support2/Binder.h>
#include <support2/KeyedVector.h>

namespace B {
namespace Support2 {


/*-------------------------------------------------------------*/
/*------- BDictionary Class -----------------------------------*/
class BDictionary : public BBinder
{
public:
	BDictionary();
	
	virtual	status_t				Told(BValue &in);
	virtual	status_t				Asked(const BValue &outBindings, BValue &out);

private:
	BValue m_mapping;
};


} } // namespace B::Support2

#endif // _SUPPORT2_DICTIONARY_H_

