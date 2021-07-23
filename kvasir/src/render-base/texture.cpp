#include "texture.h"

using namespace kvasir;

texture_base::~texture_base()
{
}
texture_image texture_base::load_image(const char *file_path)
{
	std::vector<unsigned char> buffer;
	texture_image img;
	lodepng::load_file(buffer, std::string(file_path));
	unsigned error = lodepng::decode(img.pixels, img.w, img.h, buffer);
	if (error)
		std::cout << "lodepng decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	return img;
}
texture_image texture_base::colour_image(long colour)
{

	int r = (colour & 0xFF0000) >> 16;
	int g = (colour & 0x00FF00) >> 8;
	int b = (colour & 0x0000FF);
	texture_image ret;
	ret.pixels.push_back(r);
	ret.pixels.push_back(g);
	ret.pixels.push_back(b);
	ret.pixels.push_back(255);
	ret.w = 1;
	ret.h = 1;
	return ret;
}
void texture_base::make(const texture_image &tex, size_t slot)
{
	gen_texture();
	bind();
	set_slot(slot);
	set_texture(tex);
}
void texture_base::make_png(const char *path, size_t slot)
{
	make(load_image(path), slot);
}
void texture_base::make_colour(long colour, size_t slot)
{
	make(colour_image(colour), slot);
}