#ifndef KVASIR_TEXTURE_H_
#define KVASIR_TEXTURE_H_ 1

#ifndef NO_USE_INCLUDES
#include <stddef.h>
#endif

namespace kvasir
{

	struct texture_base
	{
		virtual void bind() = 0;
		virtual void set_data(void *data, size_t len) = 0;
		virtual void gen_texture() = 0;
	};

}

#endif