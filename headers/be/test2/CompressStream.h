#ifndef TEST2_COMPRESSSTREAM_H
#define TEST2_COMPRESSSTREAN_H

#include <support2/ByteStream.h>
#include <support2/Locker.h>

struct z_stream_s;

namespace B {
namespace Test2 {

using namespace Support2;

class BZLibState
{
public:
								BZLibState(size_t bufferSize = 16*1024);
								~BZLibState();

			ssize_t				RefreshOutput(IByteOutput::sptr_ref stream, uint32 flags, bool force = false);
			ssize_t				RefreshInput(IByteInput::sptr_ref stream);
			status_t			DecodeStatus(int status, bool* ended = NULL) const;
			
			struct z_stream_s*	m_zStream;

private:
			const size_t		m_bufferSize;
			void*				m_inputBuffer;
			void*				m_outputBuffer;
};

class BDeflateOutput : public LByteOutput
{
public:
								BDeflateOutput(	IByteOutput::sptr_ref dest,
												size_t bufferSize = 16*1024);

		virtual	ssize_t			WriteV(const iovec *vector, ssize_t count, uint32 flags = 0);
		virtual	status_t		Sync();
		
				ssize_t			Flush(uint32 flags = 0);

protected:
	virtual						~BDeflateOutput();

private:
			BLocker				m_lock;
			BZLibState			m_state;
			IByteOutput::ptr	m_destination;
};

class BInflateInput : public LByteInput
{
public:
								BInflateInput(	IByteInput::sptr_ref src,
												size_t bufferSize = 16*1024);

	virtual	ssize_t				ReadV(const iovec *vector, ssize_t count);

protected:
	virtual						~BInflateInput();
	
private:
			BLocker				m_lock;
			BZLibState			m_state;
			IByteInput::ptr		m_source;
			bool				m_ended;
};

} }	// namespace B::Test2

#endif
