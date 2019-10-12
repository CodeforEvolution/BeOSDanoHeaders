/*******************************************************************************
/
/	File:			render2/Pixmap.h
/
/   Description:    BPixmap is a raster image
/
/	Copyright 1993-98, Be Incorporated, All Rights Reserved
/
*******************************************************************************/

#ifndef	_RENDER2_PIXMAP_H
#define	_RENDER2_PIXMAP_H

#include <render2/IRender.h>

namespace B {
namespace Render2 {

using namespace Support2;

/*----------------------------------------------------------------*/
/*----- BPixmap class --------------------------------------------*/

class BPixmap : public LVisual, public BPixelDescription
{
public:
	B_STANDARD_ATOM_TYPEDEFS(BPixmap)

					BPixmap(const BPixelDescription& desc);
					BPixmap(uint32 width, uint32 height, pixel_format pf, uint32 flags = 0);
					BPixmap(const BValue& desc);

	//! Implement this to render your Pixmap
	virtual	void	DrawPixels(const IRender::sptr& into) = 0;

	//{@ These are used for the implementation. Don't override them.
	virtual	void	Display(const IRender::sptr& into);
	virtual	void	Draw(const IRender::sptr& into);
	virtual	void	Invalidate(const BUpdate &update);
	virtual BRect	Frame() const;
	//@}

protected:
	int32	m_cacheId;
};

/*-------------------------------------------------------------*/
/*----- BStaticPixmap class -----------------------------------*/

/*! BStaticPixmap provides a 'BBitmap' like service.
	It is a cachable pixel-based pixmap (a bitmap).
*/

class BStaticPixmap : public BPixmap
{
public:
	B_STANDARD_ATOM_TYPEDEFS(BStaticPixmap)

							BStaticPixmap(const BPixelDescription& desc, uint32 bytesPerRow);
							BStaticPixmap(uint32 width, uint32 height, pixel_format pf, uint32 bytesPerRow, uint32 flags = 0);
							BStaticPixmap(const BValue& from);
	virtual					~BStaticPixmap();

	static	bool			IsPixmap(const BValue& who);
	
			BValue			AsValue() const;
	
			uint32			BytesPerRow() const				{ return m_bytesPerRow; };
			const void *	Pixels() const					{ return m_pixels; };
			const void *	PixelsForRow(uint32 row) const	{ return (uint8*)m_pixels + BytesPerRow()*row; };
			size_t			Size() const					{ return m_bytesPerRow*Height(); }
			BPixelData		PixelData() const;
			void *			EditPixels();
			void *			EditPixelsForRow(uint32 row);

	//! Don't override this call.
	virtual	void	DrawPixels(const IRender::sptr& into);

private:
	uint32		m_bytesPerRow;
	void *		m_pixels;
	area_id		m_area;		
};

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

} } // namespace B::Render2

#endif /* _RENDER2_PIXMAP_H */

