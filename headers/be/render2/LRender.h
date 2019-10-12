/***************************************************************************
//
//	File:			render2/LRender.h
//
//	Description:	Drawing host
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _RENDER2_LRENDER_H_
#define _RENDER2_LRENDER_H_

#include <support2/Atom.h>
#include <support2/Locker.h>
#include <support2/Vector.h>
#include <support2/KeyedVector.h>
#include <raster2/RasterPoint.h>
#include <render2/IRender.h>
#include <render2_p/RenderInputPipe.h>

namespace B {
namespace Render2 {

class BRenderStream;

class LRender : public LInterface<IRender>
{
	public:

		B_STANDARD_ATOM_TYPEDEFS(LRender)

								LRender();
		virtual					~LRender();

		virtual	lock_status_t	Lock();
		virtual	void			Unlock();


		//! Get the current proxy. returns NULL if not valid anymore
				atom_ptr<BRenderStream>	Proxy();

		/*!	Set \p state as the new current Render state.
			Passing NULL resets the LRender to its original state after construction.
			In all cases, returns the old state (which can be destructed with DestroyState)
		*/
		virtual	const void *	SetState(const void *state = NULL) = 0;

		//!	Destroy a state, and free all ressources associated with it
		virtual	void			DestroyState(const void *render_state) = 0;

		//!	Returns a copy of the current Render state
		virtual	const void *	CopyState() = 0;

		/*!	Returns a reference on the current Render state
			the reference is released when DestroyState is called
		*/
		virtual	const void *	ReferenceState() = 0;

		/*! This hook is called when all BRenderProxies are done
			with this LRender. The Default implementation is empty.
		*/
		virtual void			Finished();



		/*! Reference counting on this LRender usage. When the count
			reaches zero, Finished() is called.
		*/
				void			IncUsers();
				void			DecUsers();

		/*!
			Saves the current state in the current active BRenderStream,
			sets \p proxy as the new active BRenderStream and update
			the state with the new BRenderStream's one.
		*/
				void			SetProxy(const atom_ptr<BRenderStream>& proxy);

	private:
		atom_ref<BRenderStream>		m_proxy;
		BNestedLocker				m_lock;
		int32						m_users;
};

/******************************************************************************/

class BRenderProxy : public BBinder
{
public:
		B_STANDARD_ATOM_TYPEDEFS(BRenderProxy)
									BRenderProxy(const LRender::sptr& render);
									BRenderProxy(const atom_ptr<BRenderStream>& stream);
		virtual						~BRenderProxy();

		virtual status_t			Transact(	uint32 code,
												BParcel& data,
												BParcel* reply = NULL,
												uint32 flags = 0);
		virtual status_t			Called(BValue &in, const BValue &outBindings, BValue &out);
		virtual BValue				Inspect(const BValue& which, uint32 flags = 0);

				void 				DrawRemote(const IVisual::sptr& visual);
		
private:
        atom_ptr<BRenderStream>		m_stream;
};

} }

#endif
