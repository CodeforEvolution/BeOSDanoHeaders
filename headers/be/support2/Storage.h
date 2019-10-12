
#ifndef	_SUPPORT2_STORAGE_H
#define	_SUPPORT2_STORAGE_H

#include <support2/IStorage.h>
#include <support2/Binder.h>

namespace B {
namespace Support2 {

/*-----------------------------------------------------------------*/

class LStorage : public LInterface<IStorage>
{
	public:
		virtual	status_t		Transact(uint32 code, BParcel& data, BParcel* reply = NULL, uint32 flags = 0);
		
	protected:
		inline					LStorage() { }
		inline virtual			~LStorage() { }
		
		virtual	status_t		HandleEffect(const BValue &in, const BValue &inBindings, const BValue &outBindings, BValue *out);
	
	private:
								LStorage(const LStorage& o);
};

/*-------------------------------------------------------------*/

} } // namespace B::Support2

#endif /* _SUPPORT2_BYTESTREAM_H */
