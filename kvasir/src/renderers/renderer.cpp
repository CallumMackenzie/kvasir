#include "renderer.h"

using namespace kvasir;

render_base::~render_base() {}
int render_base::get_position_x()
{
	int x, y;
	get_position(&x, &y);
	return x;
}
int render_base::get_position_y()
{
	int x, y;
	get_position(&x, &y);
	return y;
}
int render_base::get_width()
{
	int x, y;
	get_size(&x, &y);
	return x;
};
int render_base::get_height()
{
	int x, y;
	get_size(&x, &y);
	return y;
};
void render_base::set_width(int w)
{
	set_size(w, get_height());
}
void render_base::set_height(int h)
{
	set_size(get_width(), h);
}
double render_base::get_aspect()
{
	int w, h;
	get_size(&w, &h);
	return (double)h / (double)w;
}
const char *render_base::type_to_string(type t)
{
	switch (t)
	{
	case OPENGL:
		return "OpenGL";
	case VULKAN:
		return "Vulkan";
	case DIRECTX:
		return "DirectX";
	case TERMINAL:
		return "Terminal";
	default:
		break;
	}
	return "Unknown";
}