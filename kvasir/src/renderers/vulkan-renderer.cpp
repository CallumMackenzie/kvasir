#include "vulkan-renderer.h"

using namespace kvasir;

bool vulkan_render_base::should_close() { return win.should_close(); }
void vulkan_render_base::set_clear_colour(long) { throw UNIMPLEMENTED_EXCEPTION; }
void vulkan_render_base::clear() { throw UNIMPLEMENTED_EXCEPTION; }
void vulkan_render_base::swap_buffers() { throw UNIMPLEMENTED_EXCEPTION; }
void vulkan_render_base::poll_events() { win.poll_events(); }
void vulkan_render_base::set_position(int x, int y) { win.set_pos(x, y); }
void vulkan_render_base::get_position(int *x, int *y) { win.get_pos(x, y); }
void vulkan_render_base::get_size(int *w, int *h) { win.get_size(w, h); }
void vulkan_render_base::set_size(int w, int h) { win.set_size(w, h); }
void vulkan_render_base::set_title(const char *n) { win.set_title(n); }
bool vulkan_render_base::is_fullscreen() { return win.is_fullscreen(); }
bool vulkan_render_base::set_fullscreen()
{
	if (!win.recreate_fullscreen())
		return false;
	// Restore state
	return true;
}
bool vulkan_render_base::set_windowed()
{
	if (!win.recreate_windowed())
		return false;
	// Restore state
	return true;
}
bool vulkan_render_base::set_resizable(bool b)
{
	if (!win.recreate_resizable(b))
		return false;
	// Restore state
	return true;
}
bool vulkan_render_base::is_resizable() { return win.is_resizable(); }
bool vulkan_render_base::set_visible(bool visible)
{
	if (!win.recreate_visible(visible))
		return false;
	// Restore state
	return true;
}
bool vulkan_render_base::is_visible() { return win.is_visible(); }
bool vulkan_render_base::init(const char *n, int w, int h)
{
	win.create_window(n, w, h);
	return false;
}
void vulkan_render_base::destroy() { win.destroy(); }
buffer_base *vulkan_render_base::make_buffer()
{
	throw UNIMPLEMENTED_EXCEPTION;
	return nullptr;
}
shader_base *vulkan_render_base::make_shader()
{
	throw UNIMPLEMENTED_EXCEPTION;
	return nullptr;
}
texture_base *vulkan_render_base::make_texture()
{
	throw UNIMPLEMENTED_EXCEPTION;
	return nullptr;
}
material_base *vulkan_render_base::make_material()
{
	throw UNIMPLEMENTED_EXCEPTION;
	return nullptr;
}
void vulkan_render_base::render_mesh3d(camera3d &, mesh3d &, shader_base *)
{
	throw UNIMPLEMENTED_EXCEPTION;
}
void vulkan_render_base::depth_buffer_active(bool)
{
	throw UNIMPLEMENTED_EXCEPTION;
}