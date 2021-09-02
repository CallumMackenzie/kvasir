#include "text-img-render.h"
#include "ft2build.h"
#include FT_FREETYPE_H

using namespace kvasir;

text_to_img::char_img_set gcif_freetype(FT_Library &ft, FT_Face &face, unsigned int pw, unsigned int ph, text_to_img::bitmap_type output_type)
{
	using namespace text_to_img;
	FT_Error err = FT_Set_Char_Size(face, 0, 16 * 64, pw, ph);
	if (err)
		throw std::runtime_error("Could not set font pixel sizes.");

	FT_Render_Mode render_mode = ft_render_mode_normal;
	switch (output_type)
	{
	case bitmap_type::BIT:
		render_mode = ft_render_mode_mono;
		break;
	case bitmap_type::RGB:
	case bitmap_type::GRAYSCALE:
	case bitmap_type::BGRA:
	case bitmap_type::RGBA:
		render_mode = ft_render_mode_normal;
		break;
	default:
		throw std::runtime_error("Output type is not valid.");
	}

	char_img_set ret;
	for (unsigned char c = 0; c < 128; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_DEFAULT))
		{
			std::cerr << "Failed to load char: " << c << std::endl;
			continue;
		}
		if (FT_Render_Glyph(face->glyph, render_mode))
		{
			std::cerr << "Failed to render char: " << c << std::endl;
			continue;
		}
		char_img_set::character cimg = char_img_set::character(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap_left, face->glyph->bitmap_top, face->glyph->advance.x);
		for (size_t y = 0; y < cimg.sizey; ++y)
			for (size_t x = 0; x < cimg.sizex; ++x)
				cimg.pixels.push_back(face->glyph->bitmap.buffer[(y * cimg.sizex) + x]);

		cimg.type = output_type;
		switch (output_type)
		{
		case bitmap_type::BIT:
		case bitmap_type::GRAYSCALE:
			break;
		case bitmap_type::RGB:
		{
			std::vector<unsigned char> bytes;
			for (size_t i = 0; i < cimg.sizey * cimg.sizex; ++i)
				for (size_t j = 0; j < 3; ++j)
					bytes.push_back(cimg.pixels[i]);
			cimg.pixels = bytes;
		}
		break;
			break;
		case bitmap_type::BGRA:
		case bitmap_type::RGBA:
		{
			std::vector<unsigned char> bytes;
			for (size_t i = 0; i < cimg.sizey * cimg.sizex; ++i)
				for (size_t j = 0; j < 4; ++j)
					bytes.push_back(cimg.pixels[i]);
			cimg.pixels = bytes;
		}
		break;
		default:
			throw std::runtime_error("Freetype output type is not valid.");
		}

		ret.set((char)c, cimg);
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	return ret;
}

void init_freetype(FT_Library &ft)
{
	if (FT_Init_FreeType(&ft))
		throw std::runtime_error("Could not init FreeType font rendering.");
}
void check_face_errs(FT_Error err, FT_Library &ft)
{
	if (err)
		FT_Done_FreeType(ft);
	if (err == FT_Err_Unknown_File_Format)
		throw std::runtime_error("Failed to load font: unsupported file format.");
	else if (err)
		throw std::runtime_error("Failed to load font: unknown cause (is the font path correct?).");
}

text_to_img::char_img_set text_to_img::get_char_images_from_file_data(std::vector<unsigned char> &file_data, unsigned int pixel_width, unsigned int pixel_height, bitmap_type output_type)
{
	FT_Library ft;
	init_freetype(ft);
	FT_Face face;
	check_face_errs(FT_New_Memory_Face(ft, &file_data[0], (FT_Long)file_data.size(), 0, &face), ft);

	return gcif_freetype(ft, face, pixel_width, pixel_height, output_type);
}
text_to_img::char_img_set text_to_img::get_char_images_from_font(const char *font_path, unsigned int pixel_width, unsigned int pixel_height, bitmap_type output_type)
{
	FT_Library ft;
	init_freetype(ft);

	FT_Face face;
	check_face_errs(FT_New_Face(ft, font_path, 0, &face), ft);

	return gcif_freetype(ft, face, pixel_width, pixel_height, output_type);
}

using namespace text_to_img;

char_img_set::character::character(unsigned int sx, unsigned int sy, unsigned int bx, unsigned int by, unsigned int adv)
{
	sizex = sx;
	sizey = sy;
	bearingx = bx;
	bearingy = by;
	advance = adv;
}

char_img_set::character &char_img_set::get(char c)
{
	return characters[c];
}
void char_img_set::set(char c, character v)
{
	characters[(size_t)c] = v;
}