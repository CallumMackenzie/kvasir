#ifndef KVASIR_BUFFER_H_
#define KVASIR_BUFFER_H_ 1

#ifndef NO_STD_INCLUDES
#include <stddef.h>
#endif
#ifndef NO_USE_INCLUDES
#include "config.h"
#endif

namespace kvasir
{

	struct KV_EXPORT buffer_base
	{
		virtual ~buffer_base() = 0;
		virtual void gen_buffer() = 0;
		virtual void set_data(void *data, size_t len, bool change_often = false) = 0;
		virtual void sub_data(void *data, size_t offset, size_t len) = 0;
		virtual void get_data(void *recv, size_t offset, size_t len) = 0;
		virtual void attrib_ptr(size_t ptr_num, size_t size, size_t step = 0, size_t offset = 0) = 0;
		virtual void bind_buffer() = 0;
		virtual void bind_vao() = 0;
		virtual void free_buffer() = 0;
	};

}

#endif