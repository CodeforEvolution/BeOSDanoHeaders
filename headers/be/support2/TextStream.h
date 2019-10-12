
#ifndef	_SUPPORT2_TEXTSTREAM_H
#define	_SUPPORT2_TEXTSTREAM_H

#include <support2/ITextStream.h>
#include <support2/ByteStream.h>

namespace B {
namespace Support2 {

//! Flags for BTextOutput constructor.
enum {
	B_TEXT_OUTPUT_THREADED		= 0x00000001,	//!< Buffer text per-thread.
	B_TEXT_OUTPUT_COLORED		= 0x00000002,	//!< Lines are colored by thread.
	B_TEXT_OUTPUT_TAG_THREAD	= 0x00000004,	//!< Prefix output with thread id.
	B_TEXT_OUTPUT_TAG_TEAM		= 0x00000008,	//!< Prefix output with team id.
	B_TEXT_OUTPUT_TAG_TIME		= 0x00000010,	//!< Prefix output with timestamp.
	B_TEXT_OUTPUT_FROM_ENV		= 0x10000000	//!< Get above flags from environment var.
};

/*-----------------------------------------------------------------*/

class BTextOutput : public ITextOutput
{
	public:

										BTextOutput(IByteOutput::arg stream, uint32 flags = 0);
		virtual							~BTextOutput();

		virtual	status_t				Print(	const char *debugText,
												ssize_t len = -1);
		virtual void					MoveIndent(	int32 delta);
		
		virtual	status_t				LogV(	const log_info& info,
												const iovec *vector,
												ssize_t count,
												uint32 flags = 0);

	protected:

										BTextOutput(IByteOutput *This, uint32 flags = 0);
	
		// TO DO: Implement LTextOutput and RTextOutput.
		virtual	atom_ptr<IBinder>		AsBinderImpl()			{ return NULL; }
		virtual	atom_ptr<const IBinder>	AsBinderImpl() const	{ return NULL; }
		
	private:

				class style_state {
				public:
					int32	tag;
					int32	indent;
					int32	startIndent;
					int32	front;
					
					int32	buffering;
					char*	buffer;
					ssize_t	bufferLen;
					ssize_t	bufferAvail;
					
							style_state();
							~style_state();
				};
				
				void					InitStyles();
				const char*				MakeIndent(style_state *style, int32* out_indent);
				const char*				MakeIndent(int32* inout_indent);
				style_state *			Style();
		static	void					DeleteStyle(void *style);

				IByteOutput *			m_stream;
				uint32					m_flags;
				
				style_state				m_globalStyle;
				int32					m_tlsSlot;
				int32					m_nextTag;
				
				enum {
					MAX_COLORS = 16
				};
				size_t					m_numColors;
				char					m_colors[MAX_COLORS];
};

/*-----------------------------------------------------------------*/

} } // namespace B::Support2

#endif /* _SUPPORT2_TEXTSTREAM_H */
