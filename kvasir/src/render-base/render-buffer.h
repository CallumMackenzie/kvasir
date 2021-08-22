#ifndef KVASIR_RENDER_BUFFER_H
#define KVASIR_RENDER_BUFFER_H 1

#ifndef NO_STD_INCLUDES
#include <string.h>
#endif

#ifndef NO_USE_INCLUDES
#include "config.h"
#endif

namespace kvasir
{
	struct KV_EXPORT render_buffer
	{
		virtual ~render_buffer() = 0;
		virtual bool set_size(size_t width, size_t height) = 0;
		virtual void bind() = 0;
		virtual int get_width() = 0;
		virtual int get_height() = 0;
	};
}

#endif
