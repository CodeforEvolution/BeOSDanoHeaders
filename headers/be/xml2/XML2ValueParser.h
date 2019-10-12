#ifndef _VALUE_PARSER_H

#define _VALUE_PARSER_H

#include <xml2/XMLParser.h>

namespace B {
namespace XML {

class BXML2ValueParser : public BXMLParseContext
{
	B::Support2::BValue &mTargetValue;
	
	struct building_node
	{
		B::Support2::BValue value,key,type;
	};
	
	B::Support2::BVector<building_node> mStack;
	B::Support2::BString mData;
	
	static status_t ParseSignedInteger (const B::Support2::BString &from,
										int64 maximum, int64 *val);
	
	public:
		BXML2ValueParser (B::Support2::BValue &v);
		
		virtual status_t OnStartTag (B::Support2::BString &name,
										B::Support2::BValue &attributes);
										
		virtual status_t OnEndTag (B::Support2::BString &);
		virtual status_t OnTextData (const char *data, int32 size);
};

} } // B::XML

#endif
