/******************************************************************************
/
/	File:			RandomAccessIO.h
/
/	Copyright 1993-98, Be Incorporated
/
******************************************************************************/

#ifndef	_SUPPORT2_MEMORYSTORE_H
#define	_SUPPORT2_MEMORYSTORE_H

#include <support2/Storage.h>

namespace B {
namespace Support2 {

/*-------------------------------------------------------------------*/

class BMemoryStore : public LStorage
{
	public:
		B_STANDARD_ATOM_TYPEDEFS(BMemoryStore);

							BMemoryStore();
							BMemoryStore(const BMemoryStore &);
							BMemoryStore(void *data, size_t size);
		
				BMemoryStore&operator=(const BMemoryStore &);
				
				bool		operator<(const BMemoryStore &) const;
				bool		operator<=(const BMemoryStore &) const;
				bool		operator==(const BMemoryStore &) const;
				bool		operator!=(const BMemoryStore &) const;
				bool		operator>=(const BMemoryStore &) const;
				bool		operator>(const BMemoryStore &) const;
				
				const void *Buffer() const;
				status_t	AssertSpace(size_t newSize);
				status_t	Copy(const BMemoryStore &);
				int32		Compare(const BMemoryStore &) const;

		virtual	off_t		Size() const;
		virtual	status_t	SetSize(off_t position);

		virtual	ssize_t		ReadAtV(off_t position, const struct iovec *vector, ssize_t count);
		virtual	ssize_t		WriteAtV(off_t position, const struct iovec *vector, ssize_t count);
		virtual	status_t	Sync();

				void		SetBlockSize(size_t blocksize);
		
	protected:
		virtual				~BMemoryStore();
		
	private:

		virtual	void *		MoreCore(void *oldBuf, size_t newSize);
		virtual	void		FreeCore(void *oldBuf);
				
				size_t		fLength;
				char		*fData;
};

/*-------------------------------------------------------------------*/

class BMallocStore : public BMemoryStore
{
	public:
		B_STANDARD_ATOM_TYPEDEFS(BMallocStore);
		
							BMallocStore();
							BMallocStore(const BMemoryStore &);
		
				void		SetBlockSize(size_t blocksize);

	protected:
		virtual				~BMallocStore();
		
	private:

		virtual	void *		MoreCore(void *oldBuf, size_t newSize);
		virtual	void		FreeCore(void *oldBuf);
		
				size_t		fBlockSize;
				size_t		fMallocSize;
};

/*-------------------------------------------------------------*/
/*---- No user serviceable parts after this -------------------*/

inline bool BMemoryStore::operator<(const BMemoryStore &o) const
{
	return Compare(o) < 0;
}

inline bool BMemoryStore::operator<=(const BMemoryStore &o) const
{
	return Compare(o) <= 0;
}

inline bool BMemoryStore::operator==(const BMemoryStore &o) const
{
	return Compare(o) == 0;
}

inline bool BMemoryStore::operator!=(const BMemoryStore &o) const
{
	return Compare(o) != 0;
}

inline bool BMemoryStore::operator>=(const BMemoryStore &o) const
{
	return Compare(o) >= 0;
}

inline bool BMemoryStore::operator>(const BMemoryStore &o) const
{
	return Compare(o) > 0;
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

} } // namespace B::Support2

#endif /* _SUPPORT2_MEMORYSTORE_H */
