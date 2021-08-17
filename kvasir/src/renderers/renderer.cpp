#include "renderer.h"

using namespace kvasir;

render_base::~render_base() {}
buffer_base *render_base::make_buffer() { return nullptr; };
shader_base *render_base::make_shader() { return nullptr; };
texture_base *render_base::make_texture() { return nullptr; };
material_base *render_base::make_material() { return nullptr; };
render_buffer *render_base::make_render_buffer() { return nullptr; }
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
bool render_base::key_pressed(key k)
{
	return this->get_keystate(k) == KeyDown;
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
void render_base::render_mesh3d(camera3d &c, mesh3d &m, shader_base *s, render_buffer *buff)
{
}
void render_base::render_mesh2d(camera2d &c, mesh2d &m, shader_base *s, render_buffer *buff)
{
}