#include "texture.h"
#include "lodepng.h"
#include "memory-aid.h"

using namespace kvasir;

long texture_image::pixel_rgb(size_t x, size_t y)
{
	long ret = 0x000000;
	if (x >= w || y >= h || x < 0 || y < 0)
		return ret;
	size_t i = (y * w + x) * 4;
	ret |= (get_pixels()[i + 0] & 0xffffff);
	ret |= (get_pixels()[i + 1] & 0xffffff) << 8;
	ret |= (get_pixels()[i + 2] & 0xffffff) << 16;
	return ret;
}
long texture_image::pixel_rgb(const galg::vec2f &uv)
{
	return pixel_rgb((size_t)(uv.x() * (float)w), (size_t)(uv.y() * (float)h));
}

texture_base::~texture_base()
{
}
texture_image texture_base::load_image(const char *file_path)
{
	std::string file_path_str(file_path);
	if (use_image_cache)
		if (get_image_cache().find(file_path_str) != get_image_cache().end())
			return get_image_cache()[file_path_str];
	std::vector<unsigned char> buffer;
	texture_image img;
	lodepng::load_file(buffer, file_path_str);
	unsigned error = lodepng::decode(img.get_pixels(), img.w, img.h, buffer);
	if (error)
	{
		std::cerr << "lodepng decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		return texture_image{};
	}
	if (use_image_cache)
		get_image_cache()[file_path_str] = img;
	return img;
}
texture_image texture_base::colour_image(long colour)
{

	int r = (colour & 0xFF0000) >> 16;
	int g = (colour & 0x00FF00) >> 8;
	int b = (colour & 0x0000FF);
	texture_image ret;
	ret.get_pixels().push_back(r);
	ret.get_pixels().push_back(g);
	ret.get_pixels().push_back(b);
	ret.get_pixels().push_back(255);
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
const std::vector<unsigned char> &texture_image::get_pixels() const { return pixels; };
std::vector<unsigned char> &texture_image::get_pixels() { return pixels; }

void texture_base::init()
{
	image_cache = new std::unordered_map<std::string, texture_image>();
}
void texture_base::destroy()
{
	DEL_PTR(image_cache);
}
texture_image::texture_image()
{
	// pixels = new std::vector<unsigned char>();
}
texture_image::~texture_image()
{
	// DEL_PTR(pixels);
	// w = (h = 0);
}
std::unordered_map<std::string, texture_image> &texture_base::get_image_cache()
{
	return *image_cache;
}