#ifndef _B_XML2_PARSER_H
#define _B_XML2_PARSER_H

#include <support2/Atom.h>
#include <support2/IByteStream.h>
#include <support2/Binder.h>
#include <support2/Vector.h>

#include <xml2/IXMLOStr.h>
#include <xml2/XMLDefs.h>
#include <xml2/DataSource.h>

namespace B {
namespace XML {

using namespace Support2;


// Forward References in namespace B::XML
// =====================================================================
class BXMLParseContext;
class BParser;
class BCreator;


// BXMLParseContext -- Hook Functions for what the parser encounters
// =====================================================================
class BXMLParseContext
{
public:
				BXMLParseContext();
	virtual		~BXMLParseContext();
	
	// These fields are always set to the current line and column (or as
	// close of an approximation as possible).  They are probably most
	// useful for error messages.
	int32 line;
	int32 column;
	
						// Called at the beginning of the document parsing process.
	virtual status_t	OnDocumentBegin(uint32 flags);
	
						// Called at the end of the document parsing process.
						// If you get this call, then you know that parsing was successful.
	virtual status_t	OnDocumentEnd();
	
	// The following functions are fairly self explanitory.
	// Whenever there's a BString that isn't const, you are allowed to use the
	// BString::Adopt function to take the string buffer, and leave the original
	// string empty.  This is just a performance optimization.
	
	virtual status_t	OnStartTag(				BString		& name,
												BValue		& attributes		);
									
	virtual status_t	OnEndTag(				BString		& name				);
	
	virtual status_t	OnTextData(				const char	* data,
												int32		size				);
	
	virtual status_t	OnCData(				const char	* data,
												int32		size				);
	
	virtual status_t	OnComment(				const char	* data,
												int32		size				);
	
	virtual status_t	OnDocumentTypeBegin(	BString		& name				);
	
	virtual status_t	OnExternalSubset(		BString		& publicID,
												BString		& systemID,
												uint32 		parseFlags			);
	
	virtual status_t	OnInternalSubsetBegin();
	
	virtual status_t	OnInternalSubsetEnd();
	
	virtual status_t	OnDocumentTypeEnd();
	
	virtual status_t	OnProcessingInstruction(BString		& target,
												BString		& data				);
	
	virtual	status_t	OnElementDecl(			BString		& name,
												BString		& data				);
	
	virtual status_t	OnAttributeDecl(		BString		& element,
												BString		& name,
												BString		& type,
												BString		& behavior,
												BString		& defaultVal		);
	
	virtual status_t	OnInternalParsedEntityDecl(	BString	& name,
													BString & internalData,
													bool	parameter,
													uint32	parseFlags			);
	
	virtual status_t	OnExternalParsedEntityDecl(	BString	& name,
													BString & publicID,
													BString & systemID,
													bool	 parameter,
													uint32	parseFlags			);
	
	virtual status_t	OnUnparsedEntityDecl(		BString	& name,
													BString & publicID,
													BString & systemID,
													BString & notation			);
	
	virtual status_t	OnNotationDecl(				BString	& name,
													BString	& value				);
	
						// This is a hook function to notify the subclass that we
						// encountered a PE in a text section.  Subclasses might
						// either look up replacement text and insert it, or look
						// parsed objects and insert them.
	virtual status_t	OnGeneralParsedEntityRef(	BString	& name				);
	
						// This is a hook function to find out the replacement text
						// for a general entity when it occurs in an attribute.  The
						// value is then substituted into the attribute as if it
						// had never been there.  If you want this behavior, you must
						// set the B_XML_HANDLE_ATTRIBUTE_ENTITIES flag.
	virtual status_t	OnGeneralParsedEntityRef(	BString	& name,
													BString & replacement		);
	
						// This is a hook function to notify the subclass when an 
						// entity occurred in the DTD, but in a place where it would
						// be better for the subclass to just insert its objects into
						// the stream than to send back the replacement text, and 
						// have this parser have to reparse it each time it occurs.
	virtual status_t	OnParameterEntityRef(		BString	& name				);
	
						// This is a hook function to find the replacement text for
						// a parameter entity.  It will then be parsed, and the normal
						// other functions will be called.
	virtual status_t	OnParameterEntityRef(		BString	& name,
													BString & replacement		);

						// An error occurred.  If you would like to ignore the error,
						// and continue processing, then return B_OK from this
						// function, and processing will continue.
						// If fatal is true, a fatal error occurred, and we're not
						// going to be able to recover, no matter what you return.
						// debugLineNo is the line number from which OnError was called
						// XXX Should debugLineNo be made public?
						// The code and data parameters are currently unused.  I have
						// visions of using them to help recover from errors 
						// (for example, pass in potentially corrupted structures, and
						// allow the OnError function to have a go at correcting them)
	virtual status_t	OnError(status_t error, bool fatal, int32 debugLineNo,
									uint32 code = 0, void * data = NULL);

};


class BCreator : public BAtom
{
public:
	B_STANDARD_ATOM_TYPEDEFS(BCreator)
	
	
	virtual 			~BCreator();
	
	virtual status_t	OnStartTag(				BString			& name,
												BValue			& attributes,
												BCreator::ptr	& newCreator	);
									
	virtual status_t	OnEndTag(				BString			& name			);
	
	virtual status_t	OnText(					BString			& data			);
	
	virtual status_t	OnComment(				BString			& name			);
	
	virtual status_t	OnProcessingInstruction(BString			& target,
												BString			& data			);
												
	virtual status_t	Done();
};


// ParseXML -- Do some parsing.
// =====================================================================
status_t	ParseXML(BXMLParseContext *context, BXMLDataSource *data, uint32 flags = 0);
status_t	ParseXML(BCreator::arg creator, BXMLDataSource *data, uint32 flags = 0);


}; // namespace XML
}; // namespace B


#endif // _B_XML2_PARSER_H
