#ifndef KVASIR_TEXTURE_H_
#define KVASIR_TEXTURE_H_ 1

#include <vector>
#include <string>
#include <iostream>
#include <stddef.h>
#include <unordered_map>
#ifndef NO_USE_INCLUDES
#include "galg.h"
#endif

namespace kvasir
{

	struct texture_image
	{
		std::vector<unsigned char> pixels; // RGBA
		unsigned w; // width
		unsigned h; // height

		long pixel_rgb(size_t x, size_t y);
		long pixel_rgb(const galg::vec2f &uv);
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

		static inline std::unordered_map<std::string, texture_image> image_cache;
		static inline bool use_image_cache = true;
	};

}

#endif