#ifndef KVASIR_TEXTURE_H_
#define KVASIR_TEXTURE_H_ 1

#ifndef NO_STD_INCLUDES
#include <vector>
#include <string>
#include <iostream>
#include <stddef.h>
#include <unordered_map>
#endif

#ifndef NO_USE_INCLUDES
#include "config.h"
#include "galg.h"
#include "galg-dllexport.h"
#endif

namespace kvasir
{

	struct KV_EXPORT texture_image
	{
		unsigned w; // width
		unsigned h; // height

		texture_image();
		~texture_image();
		long pixel_rgb(size_t x, size_t y);
		long pixel_rgb(const galg::vec2f &uv);
		std::vector<unsigned char> &get_pixels();
		const std::vector<unsigned char> &get_pixels() const;
		void set_pixels(const std::vector<unsigned char> &new_pixels);

		static std::vector<unsigned char> get_png_data(const char *file_path);
		static texture_image decode_png_data(std::vector<unsigned char> &png_data);

	private:
		std::vector<unsigned char> pixels; // RGBA
	};

	struct KV_EXPORT texture_base
	{
		virtual ~texture_base() = 0;
		virtual void bind() = 0;
		virtual void gen_texture() = 0;
		virtual void set_slot(size_t slot) = 0;
		virtual void set_texture(const texture_image &img) = 0;
		virtual void free_texture() = 0;
		void make(const texture_image &tex, size_t slot = 0);
		void make_png(const char *path, size_t slot = 0);
		void make_colour(long colour, size_t slot = 0);
		static texture_image load_image(const char *file_path);
		static texture_image colour_image(long colour);

		static std::unordered_map<std::string, texture_image> &get_image_cache();
		static bool image_cache_valid();
		static void init();
		static void destroy();

		static inline bool use_image_cache = true;

	private:
		static inline std::unordered_map<std::string, texture_image> *image_cache = nullptr;
	};

}

#endif