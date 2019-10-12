#ifndef _VALUE_TO_XML_H

#define _VALUE_TO_XML_H

#include <support2/IByteStream.h>

namespace B {
namespace XML {

status_t ValueToXML (B::Support2::IByteOutput::arg stream,
					const B::Support2::BValue &value);

status_t XMLToValue (B::Support2::IByteInput::arg stream,
						B::Support2::BValue &value);

} } // B::XML
					
#endif
