#include "texture.h"
#include "lodepng.h"
#include "memory-aid.h"
#include <exception>

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
	if (use_image_cache && image_cache_valid())
		if (get_image_cache().find(file_path_str) != get_image_cache().end())
			return get_image_cache()[file_path_str];
	std::vector<unsigned char> buffer = texture_image::get_png_data(file_path);
	texture_image img = texture_image::decode_png_data(buffer);
	if (use_image_cache && image_cache_valid())
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
void texture_image::set_pixels(const std::vector<unsigned char> &new_pixels)
{
	pixels = new_pixels;
}

void texture_base::init()
{
	DEL_PTR(image_cache);
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
bool texture_base::image_cache_valid()
{
	return (bool)image_cache;
}
std::vector<unsigned char> texture_image::get_png_data(const char *file_path)
{
	std::vector<unsigned char> buff;
	unsigned error = lodepng::load_file(buff, std::string(file_path));
	if (error)
		throw std::runtime_error(std::string("Lodepng decoder error ").append(std::to_string(error).append(std::string(": ")).append(std::string(lodepng_error_text(error)))).c_str());
	return buff;
}
texture_image texture_image::decode_png_data(std::vector<unsigned char> &png_data)
{
	texture_image img;
	unsigned error = lodepng::decode(img.get_pixels(), img.w, img.h, png_data);
	if (error)
		throw std::runtime_error(std::string("Lodepng decoder error ").append(std::to_string(error).append(std::string(": ")).append(std::string(lodepng_error_text(error)))).c_str());
	return img;
}