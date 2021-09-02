#ifndef TEXT_RENDERER_H_
#define TEXT_RENDERER_H_ 1
#include <vector>
#include <exception>
#include <iostream>

namespace kvasir
{
	namespace text_to_img
	{
		enum class bitmap_type
		{
			BIT,	   // 1 bit per pixel
			GRAYSCALE, // 1 grayscale byte per pixel
			RGBA,	   // 4 byte rgba
			RGB,	   // 3 byte RGB,
			BGRA,	   // 4 byte BGRA
		};

		struct char_img_set
		{
			struct character
			{
				unsigned int sizex, sizey = 0;
				unsigned int bearingx, bearingy = 0;
				unsigned int advance = 0;
				std::vector<unsigned char> pixels;
				bitmap_type type;

				character(unsigned int sizex = 0, unsigned int sizey = 0, unsigned int bearingx = 0, unsigned int bearingy = 0, unsigned int advance = 0);
			};
			character characters[255]{0};
			character &get(char c);
			void set(char c, character v);
		};

		char_img_set get_char_images_from_font(const char *font_path, unsigned int pixel_width, unsigned int pixel_height, bitmap_type output_type = bitmap_type::RGB);
		char_img_set get_char_images_from_file_data(std::vector<unsigned char> &file_data, unsigned int pixel_width, unsigned int pixel_height, bitmap_type output_type = bitmap_type::RGB);
	}
}

#endif