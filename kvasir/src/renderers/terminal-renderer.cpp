#include "terminal-renderer.h"

using namespace kvasir;

terminal_buffer_base::~terminal_buffer_base()
{
	free_buffer();
}
void terminal_buffer_base::gen_buffer() { data = nullptr; }
void terminal_buffer_base::set_data(void *in_data, size_t len, bool)
{
	DEL_ARR_PTR(data);
	data = new char[len];
	for (size_t i = 0; i < len; ++i)
		data[i] = ((char *)in_data)[i];
	bytelen = len;
}
void terminal_buffer_base::sub_data(void *in_data, size_t offset, size_t len)
{
	size_t j = 0;
	for (size_t i = offset; i < len; ++i)
	{
		data[i] = ((char *)in_data)[j];
		++j;
	}
}
void terminal_buffer_base::get_data(void *recv, size_t offset, size_t len)
{
	char *c_recv = (char *)recv;
	size_t j = 0;
	for (size_t i = offset; i < len; ++i)
	{
		c_recv[j] = data[i];
		++j;
	}
}
void terminal_buffer_base::attrib_ptr(size_t ptr_num, size_t size, size_t step, size_t offset)
{
	while (attribs.size() <= ptr_num)
		attribs.push_back(attrib());
	attribs[ptr_num].bytesize = size;
	attribs[ptr_num].step = step;
	attribs[ptr_num].offset = offset;
}
void terminal_buffer_base::bind_buffer() { bound = this; }
void terminal_buffer_base::bind_vao() { bound = this; }
void terminal_buffer_base::free_buffer()
{
	DEL_ARR_PTR(data);
	attribs.clear();
}

terminal_texture_base::~terminal_texture_base() { free_texture(); }
void terminal_texture_base::bind()
{
	while (slots.size() <= slot)
		slots.push_back(nullptr);
	slots[slot] = this;
}
void terminal_texture_base::gen_texture() { return; }
void terminal_texture_base::set_slot(size_t s) { slot = s; }
void terminal_texture_base::set_texture(const texture_image &img) { texture = img; }
void terminal_texture_base::free_texture() { return; }

terminal_shader_base::~terminal_shader_base()
{
	free_shader();
}
void terminal_shader_base::use()
{
	used = this;
}
bool terminal_shader_base::compile(const char **srcs, size_t n_srcs)
{
	throw EX_UNIMPLEMENTED;
	if (n_srcs != 2)
		return false;

	return true;
}
void terminal_shader_base::u_bool1(const char *name, bool a) { ui1s[name] = (int)a; }
void terminal_shader_base::u_bool2(const char *name, bool a, bool b) { ui2s[name] = vec2i((int)a, (int)b); }
void terminal_shader_base::u_bool3(const char *name, bool a, bool b, bool c) { ui3s[name] = vec3i((int)a, (int)b, (int)c); }
void terminal_shader_base::u_bool4(const char *name, bool a, bool b, bool c, bool d) { ui4s[name] = vec4i((int)a, (int)b, (int)c, (int)d); }
void terminal_shader_base::u_int1(const char *name, int a) { ui1s[name] = a; }
void terminal_shader_base::u_int2(const char *name, int a, int b) { ui2s[name] = vec2i(a, b); }
void terminal_shader_base::u_int3(const char *name, int a, int b, int c) { ui3s[name] = vec3i(a, b, c); }
void terminal_shader_base::u_int4(const char *name, int a, int b, int c, int d) { ui4s[name] = vec4i(a, b, c, d); }
void terminal_shader_base::u_float1(const char *name, float a) { uf1s[name] = a; }
void terminal_shader_base::u_float2(const char *name, float a, float b) { uf2s[name] = vec2f(a, b); }
void terminal_shader_base::u_float3(const char *name, float a, float b, float c) { uf3s[name] = vec3f(a, b, c); }
void terminal_shader_base::u_float4(const char *name, float a, float b, float c, float d) { uf4s[name] = vec4f(a, b, c, d); }
void terminal_shader_base::u_mat2f(const char *name, float m[2][2])
{
	um2fs[name] = mat2f{
		{m[0][0], m[0][1]},
		{m[1][0], m[1][1]}};
}
void terminal_shader_base::u_mat3f(const char *name, float m[3][3])
{
	um3fs[name] = mat3f{
		{m[0][0], m[0][1], m[0][2]},
		{m[1][0], m[1][1], m[1][2]},
		{m[2][0], m[2][1], m[2][2]}};
}
void terminal_shader_base::u_mat4f(const char *name, float m[4][4])
{
	um4fs[name] = mat4f{
		{m[0][0], m[0][1], m[0][2], m[0][3]},
		{m[1][0], m[1][1], m[1][2], m[1][3]},
		{m[2][0], m[2][1], m[2][2], m[2][3]},
		{m[3][0], m[3][1], m[3][2], m[3][3]}};
}
void terminal_shader_base::render(int n_tris)
{
	if (!terminal_buffer_base::bound)
		return;
	terminal_buffer_base *bound_buff = terminal_buffer_base::bound;
	throw EX_UNIMPLEMENTED;
}
void terminal_shader_base::free_shader()
{
	throw EX_UNIMPLEMENTED;
}

terminal_render_base::terminal_render_base() {}
terminal_render_base::~terminal_render_base()
{
	if (use_depth_buffer)
		DEL_ARR_PTR(depth_buffer_arr);
	DEL_ARR_PTR(screen_arr);
}
bool terminal_render_base::should_close() { return s_close; }
void terminal_render_base::set_clear_colour(long c)
{
	unsigned short r = (unsigned short)((c & 0xFF0000) >> 16);
	unsigned short g = (unsigned short)((c & 0x00FF00) >> 8);
	unsigned short b = (unsigned short)(c & 0x0000FF);
	clear_colour = vec3us(r, g, b);
}
void terminal_render_base::clear()
{
	if (use_depth_buffer)
		for (size_t i = 0; i < width * height; ++i)
			depth_buffer_arr[i] = depth_buffer_clear;
	for (size_t i = 0; i < width * height; ++i)
		screen_arr[i] = pixel{};
}
void terminal_render_base::swap_buffers()
{
	for (size_t y = 0; y < height; ++y)
	{
		for (size_t x = 0; x < width; ++x)
			printf("%c", screen[y][x].sym);
		printf("\n");
	}
}
void terminal_render_base::poll_events() {}
void terminal_render_base::set_position(int x, int y) { return; }
void terminal_render_base::get_position(int *x, int *y) { return; }
void terminal_render_base::get_size(int *w, int *h)
{
	*w = width;
	*h = height;
}
void terminal_render_base::set_size(int w, int h)
{
	width = w;
	height = h;
	DEL_ARR_PTR(screen_arr);
	screen_arr = new pixel[w * h];
	if (use_depth_buffer)
	{
		DEL_ARR_PTR(depth_buffer_arr);
		depth_buffer_arr = new float[w * h];
	}
}
void terminal_render_base::set_title(const char *n) { title = n; }
bool terminal_render_base::is_fullscreen() { return false; }
bool terminal_render_base::set_fullscreen() { return false; }
bool terminal_render_base::set_windowed() { return false; }
bool terminal_render_base::set_resizable(bool b) { return false; }
bool terminal_render_base::is_resizable() { return false; }
bool terminal_render_base::set_visible(bool visible) { return false; }
bool terminal_render_base::is_visible() { return true; }
bool terminal_render_base::init(const char *n, int w, int h)
{
	set_title(n);
	set_size(w, h);
	return false;
#ifdef _WIN32
	return false;
#else
	return true;
#endif
}
void terminal_render_base::destroy() { s_close = true; }
void terminal_render_base::render_mesh3d(camera3d &c, mesh3d &m, shader_base *s) { throw EX_UNIMPLEMENTED; }
void terminal_render_base::depth_buffer_active(bool b)
{
	if (b == use_depth_buffer)
		return;
	DEL_ARR_PTR(depth_buffer_arr);
	if (use_depth_buffer = b)
		depth_buffer_arr = new float[width * height];
}
buffer_base *terminal_render_base::make_buffer()
{
	return new terminal_buffer_base();
}
shader_base *terminal_render_base::make_shader()
{
	return new terminal_shader_base();
}
texture_base *terminal_render_base::make_texture()
{
	return new terminal_texture_base();
}
material_base *terminal_render_base::make_material()
{
	return new material_base();
}
render_base::type terminal_render_base::get_type()
{
	return TERMINAL;
}