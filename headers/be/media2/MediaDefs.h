/***************************************************************************
//
//	File:			media2/MediaDefs.h
//
//	Description:	Types, definitions, and constants for media2.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _MEDIA2_MEDIADEFS_H_
#define _MEDIA2_MEDIADEFS_H_

#include <support2/Atom.h>
#include <support2/ByteOrder.h>
#include <support2/Value.h>
#include <support2/Vector.h>
#include <support2/StaticValue.h>

#include <raster2/GraphicsDefs.h>

#include <OS.h>

namespace B {
namespace Media2 {

using namespace Support2;
using namespace Raster2;

/**************************************************************************************/

class IMediaNode;
class IMediaCollective;
class IMediaEndpoint;
class IMediaInput;
class IMediaOutput;
class IMediaControllable;

/**************************************************************************************/

enum media_endpoint_type
{
	B_INPUT_ENDPOINT		= 1,
	B_OUTPUT_ENDPOINT		= 2,

	B_ANY_ENDPOINT_TYPE		= B_INPUT_ENDPOINT | B_OUTPUT_ENDPOINT
};

enum media_endpoint_state
{
	B_FREE_ENDPOINT			= 1,
	B_RESERVED_ENDPOINT		= 2,
	B_CONNECTED_ENDPOINT	= 4,

	B_ANY_ENDPOINT_STATE	= B_FREE_ENDPOINT | B_RESERVED_ENDPOINT | B_CONNECTED_ENDPOINT
};

typedef	int32		media_buffer_id;
typedef	area_id		media_buffer_source_id;

/**************************************************************************************/

class BMediaEndpoint;

typedef B::Support2::BVector<atom_ptr<BMediaEndpoint> >	BMediaLocalEndpointVector;
typedef B::Support2::BVector<atom_ptr<IMediaEndpoint> >	BMediaEndpointVector;
typedef B::Support2::BVector<atom_ptr<IMediaNode> >		BMediaNodeVector;

/**************************************************************************************/
// format keys and constants
//
// all format fields are assumed to be int32 unless otherwise noted (the notable exception
// so far is B_FORMATKEY_FRAME_RATE, which is float.)

// ** BUFFER-SOURCE KEYS
// these are used in the buffer-allocation methods:
// IMediaEndpoint::BufferSourceConstraint() (and the BMediaNode equivalent)
// IMediaEndpoint::MakeBufferSource() (and the BMediaNode equivalent)
// they're not generally used in endpoint constraints.

extern value_clrg B_SOURCEKEY_SIZE;
extern value_clrg B_SOURCEKEY_COUNT;
extern value_clrg B_SOURCEKEY_LOCK_MODE;

// ** GENERAL ATTRIBUTES

extern const BValue B_FORMATKEY_MEDIA_TYPE;
enum media_type
{
	B_MEDIA_NO_TYPE = -1,
	B_MEDIA_UNKNOWN_TYPE = 0,
	B_MEDIA_RAW_AUDIO = 1,
	B_MEDIA_RAW_VIDEO,
	B_MEDIA_VBL,
	B_MEDIA_TIMECODE,
	B_MEDIA_MIDI,
	B_MEDIA_TEXT,
	B_MEDIA_HTML,
	B_MEDIA_MULTISTREAM,
	B_MEDIA_PARAMETERS,
	B_MEDIA_ENCODED_AUDIO,
	B_MEDIA_ENCODED_VIDEO,
	B_MEDIA_PRIVATE = 90000,
	B_MEDIA_FIRST_USER_TYPE = 100000
};

extern const BValue B_FORMATKEY_CHANNEL_COUNT;
extern const BValue B_FORMATKEY_FRAME_RATE; // float
extern const BValue B_FORMATKEY_BUFFER_FRAMES;

extern const BValue B_FORMATKEY_BYTE_ORDER;
enum byte_order
{
	B_MEDIA_BIG_ENDIAN = 1,
	B_MEDIA_LITTLE_ENDIAN,
#if B_HOST_IS_BENDIAN
	B_MEDIA_HOST_ENDIAN = B_MEDIA_BIG_ENDIAN
#else
	B_MEDIA_HOST_ENDIAN = B_MEDIA_LITTLE_ENDIAN
#endif
};

// ** RAW AUDIO

extern const BValue B_FORMATKEY_RAW_AUDIO_TYPE;
enum raw_audio_type
{
	B_AUDIO_INT8		= 0x01,
	B_AUDIO_UINT8		= 0x11,
	B_AUDIO_INT16		= 0x02,
	B_AUDIO_INT32		= 0x04,
	B_AUDIO_FLOAT		= 0x24,
	B_AUDIO_SIZE_MASK	= 0x0f
};

extern const BValue B_FORMATKEY_RAW_AUDIO_BITS;	// for B_AUDIO_INT32

// ** VIDEO

extern const BValue B_FORMATKEY_WIDTH;
extern const BValue B_FORMATKEY_HEIGHT;
extern const BValue B_FORMATKEY_BYTES_PER_ROW;
extern const BValue B_FORMATKEY_COLORSPACE;

extern const BValue B_FORMATKEY_VIDEO_ORIENTATION;
enum video_orientation
{
	B_VIDEO_TOP_LEFT_RIGHT = 1,		// This is the typical progressive scan format
	B_VIDEO_BOTTOM_LEFT_RIGHT		// This is how BMP and TGA might scan
};

// ** ENCODED FORMATS

extern const BValue B_FORMATKEY_ENCODING;

extern const BValue B_FORMATKEY_INFO;
extern const BValue B_FORMATKEY_DECODED_BUFFER_SIZE;

// ** ENDPOINT MESSAGES: sent via IMediaEndpoint::PropagateMessage()
//                       or BMediaNode::PropagateMessage().
// endpoints that implement IHandler should handle these messages as
// well.  maybe BMediaAsyncInput and BMediaAsyncOutput are in order? +++

// BMessage 'what' codes
enum endpoint_message_t
{
	B_MEDIA_FLUSH_ENDPOINT			= 'AU01',
	B_MEDIA_ENDPOINT_LATE,
	B_MEDIA_REPEAT_LAST_FRAME,
	B_MEDIA_RELEASE_ALL_BUFFERS,
	B_MEDIA_UPDATE_BUFFER_COUNT
};

// +++ deprecated +++
extern const BValue B_ENDPOINTKEY_FLUSH;
extern const BValue B_ENDPOINTKEY_LATE;		// +++ value: lateness in usecs?
extern const BValue B_ENDPOINTKEY_REPEAT_LAST_FRAME;
extern const BValue B_ENDPOINTKEY_RELEASE_ALL_BUFFERS;
extern const BValue B_ENDPOINTKEY_UPDATE_BUFFER_COUNT;

/**************************************************************************************/
// everybody's favorite error messages

enum {	/* maybe migrate these into Errors.h */
	B_MEDIA_SYSTEM_FAILURE = (int)B_MEDIA_ERROR_BASE+0x100,	/* 80004100 */
	B_MEDIA_BAD_NODE,
	B_MEDIA_NODE_BUSY,
	B_MEDIA_BAD_FORMAT,
	B_MEDIA_BAD_BUFFER,
	B_MEDIA_TOO_MANY_NODES,
	B_MEDIA_TOO_MANY_BUFFERS,
	B_MEDIA_NODE_ALREADY_EXISTS,
	B_MEDIA_BUFFER_ALREADY_EXISTS,
	B_MEDIA_CANNOT_SEEK,
	B_MEDIA_CANNOT_CHANGE_RUN_MODE,
	B_MEDIA_APP_ALREADY_REGISTERED,
	B_MEDIA_APP_NOT_REGISTERED,
	B_MEDIA_CANNOT_RECLAIM_BUFFERS,
	B_MEDIA_BUFFERS_NOT_RECLAIMED,
	B_MEDIA_TIME_SOURCE_STOPPED,
	B_MEDIA_TIME_SOURCE_BUSY,					/* 80004110 */
	B_MEDIA_BAD_SOURCE,
	B_MEDIA_BAD_DESTINATION,
	B_MEDIA_ALREADY_CONNECTED,
	B_MEDIA_NOT_CONNECTED,
	B_MEDIA_BAD_CLIP_FORMAT,
	B_MEDIA_ADDON_FAILED,
	B_MEDIA_ADDON_DISABLED,
	B_MEDIA_CHANGE_IN_PROGRESS,
	B_MEDIA_STALE_CHANGE_COUNT,
	B_MEDIA_ADDON_RESTRICTED,
	B_MEDIA_NO_HANDLER, 
	B_MEDIA_DUPLICATE_FORMAT,
	B_MEDIA_REALTIME_DISABLED,
	B_MEDIA_REALTIME_UNAVAILABLE
};

extern const BString B_SYSTEM_AUDIO_PLAYBACK_ENDPOINT;

}; }; // namespace B::Media2
#endif //_MEDIA2_MEDIADEFS_H_
