
#ifndef	_SUPPORT2_BYTESTREAM_H
#define	_SUPPORT2_BYTESTREAM_H

#include <support2/IByteStream.h>
#include <support2/SupportDefs.h>
#include <support2/Binder.h>

namespace B {
namespace Support2 {

/*-----------------------------------------------------------------*/

class LByteInput : public LInterface<IByteInput>
{
	public:
		virtual	status_t		Link(IBinder::sptr_ref to, const BValue &bindings, uint32 flags = 0);
		virtual	status_t		Unlink(IBinder::wptr_ref from, const BValue &bindings, uint32 flags = 0);
		virtual	status_t		Effect(const BValue &in, const BValue &inBindings, const BValue &outBindings, BValue *out);
		virtual	status_t		Transact(uint32 code, BParcel& data, BParcel* reply = NULL, uint32 flags = 0);
		
	protected:
		inline					LByteInput() { }
		inline virtual			~LByteInput() { }
		
		virtual	status_t		HandleEffect(const BValue &in, const BValue &inBindings, const BValue &outBindings, BValue *out);
	
	private:
								LByteInput(const LByteInput& o);
};

/*-----------------------------------------------------------------*/

class LByteOutput : public LInterface<IByteOutput>
{
	public:
		virtual	status_t		Link(IBinder::sptr_ref to, const BValue &bindings, uint32 flags = 0);
		virtual	status_t		Unlink(IBinder::wptr_ref from, const BValue &bindings, uint32 flags = 0);
		virtual	status_t		Effect(const BValue &in, const BValue &inBindings, const BValue &outBindings, BValue *out);
		virtual	status_t		Transact(uint32 code, BParcel& data, BParcel* reply = NULL, uint32 flags = 0);
		
	protected:
		inline					LByteOutput() { }
		inline virtual			~LByteOutput() { }
		
		virtual	status_t		HandleEffect(const BValue &in, const BValue &inBindings, const BValue &outBindings, BValue *out);
	
	private:
								LByteOutput(const LByteOutput& o);
};

/*-----------------------------------------------------------------*/

class LByteSeekable : public LInterface<IByteSeekable>
{
	public:
		virtual	status_t		Link(IBinder::sptr_ref to, const BValue &bindings, uint32 flags = 0);
		virtual	status_t		Unlink(IBinder::wptr_ref from, const BValue &bindings, uint32 flags = 0);
		virtual	status_t		Effect(const BValue &in, const BValue &inBindings, const BValue &outBindings, BValue *out);
		virtual	status_t		Transact(uint32 code, BParcel& data, BParcel* reply = NULL, uint32 flags = 0);
		
	protected:
		inline					LByteSeekable() { }
		inline virtual			~LByteSeekable() { }
		
		virtual	status_t		HandleEffect(const BValue &in, const BValue &inBindings, const BValue &outBindings, BValue *out);
	
	private:
								LByteSeekable(const LByteSeekable& o);
};

/*-------------------------------------------------------------*/

} } // namespace B::Support2

#endif /* _SUPPORT2_BYTESTREAM_H */
