#ifndef KVASIR_TEXTURE_H_
#define KVASIR_TEXTURE_H_ 1

#ifndef NO_USE_INCLUDES
#include <stddef.h>
#include <vector>
#include "lodepng.h"
#include <string>
#include <iostream>
#endif

namespace kvasir
{

	struct texture_image
	{
		std::vector<unsigned char> pixels;
		unsigned w;
		unsigned h;
	};

	struct texture_base
	{
		virtual ~texture_base() = 0;
		virtual void bind() = 0;
		virtual void gen_texture() = 0;
		virtual void set_slot(size_t slot) = 0;
		virtual void set_texture(const texture_image &img) = 0;
		virtual void free_texture() = 0;
		void make(const texture_image &tex, size_t slot  = 0);
		void make_png(const char *path, size_t slot = 0);
		void make_colour(long colour, size_t slot = 0);
		static texture_image load_image(const char *file_path);
		static texture_image colour_image(long colour);
	};

}

#endif