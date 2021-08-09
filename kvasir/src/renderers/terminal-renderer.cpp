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
	// throw EX_UNIMPLEMENTED;
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
	// throw EX_UNIMPLEMENTED;
}
void terminal_shader_base::free_shader()
{
	return;
}

terminal_render_base::terminal_render_base() {}
terminal_render_base::~terminal_render_base()
{
	if (use_depth_buffer)
		DEL_ARR_PTR(depth_buffer);
	DEL_ARR_PTR(screen);
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
			depth_buffer[i] = depth_buffer_clear;
	for (size_t i = 0; i < width * height; ++i)
		screen[i] = pixel{};
}
void terminal_render_base::swap_buffers()
{
	if (!screen)
		return;
#ifndef _WIN32
	printf("\x1b[H");
#endif
	for (size_t y = 0; y < height * width; ++y)
	{
		printf("%c", screen[y].sym);
		if (y % width == 0)
			printf("\n");
	}
}
void terminal_render_base::poll_events() {}
void terminal_render_base::set_position(int x, int y) { return; }
void terminal_render_base::get_position(int *x, int *y) { return; }
void terminal_render_base::get_size(int *w, int *h)
{
	*w = (int)width;
	*h = (int)height;
}
void terminal_render_base::set_size(int w, int h)
{
	width = w;
	height = h;
	DEL_ARR_PTR(screen);
	screen = new pixel[w * h];
	if (use_depth_buffer)
	{
		DEL_ARR_PTR(depth_buffer);
		depth_buffer = new float[w * h];
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
#ifdef _WIN32
	return false;
#else
	printf("\x1b[2J");
	return true;
#endif
}
void terminal_render_base::destroy() { s_close = true; }
void terminal_render_base::render_mesh3d(camera3d &c, mesh3d &m, shader_base *s)
{
	if (m.buffer)
		m.buffer->bind_vao();
	auto buff = terminal_buffer_base::bound;
	if (!buff)
		return;
	auto projection_m = c.perspective();
	auto rotation_m = mat4f::rotation(m.rot);
	auto transform_m = mat4f::scale(m.scale) *
					   rotation_m *
					   mat4f::translation(m.pos);
	auto clv = c.look_vector();
	auto cam_view_m = c.view().inverse();
	auto edge_fn = [](const vec4f &a, const vec4f &b, const vec2f &c)
	{
		return ((c.x() - a.x()) * (b.y() - a.y()) - (c.y() - a.y()) * (b.x() - a.x()) >= 0);
	};
	auto render_tri_3d = [&clv, &cam_view_m, &projection_m, &rotation_m, &transform_m, this, &edge_fn](mesh3d::triangle &tri)
	{
		triangle_2d rast_tri;
		for (size_s j = 0; j < 3; ++j)
		{
			vec3f rot_n = (rotation_m * tri.v[j].n).xyz().normalized();
			float normal_view_dot = rot_n.dot(clv.xyz().normalize());
			float lcd = vec3f(0, -1, 0).normalize().dot(rot_n);
			rast_tri.facing_view = normal_view_dot < 0;
			vec4f v_pos(projection_m * (cam_view_m * (transform_m * tri.v[j].p)));
			v_pos.w() = 1.f / v_pos.w();
			rast_tri.v[j].v = vec4f(
				(v_pos.x() * v_pos.w() + 0.5f) * width,
				(v_pos.y() * v_pos.w() + 0.5f) * height,
				v_pos.z() * v_pos.w(),
				v_pos.w());

			rast_tri.v[j].sym = intensity_char((lcd / 2.f) + 0.5f);
		}
		auto o_box = [](float r, float b, const vec2f &pos)
		{
			return !(pos.x() >= 0 && pos.y() >= 0 && pos.x() < r && pos.y() < b);
		};
		if (o_box(width, height, rast_tri.v[0].v.xy()) &&
			o_box(width, height, rast_tri.v[1].v.xy()) &&
			o_box(width, height, rast_tri.v[2].v.xy()))
			return;
		winding wind = rast_tri.get_wind();
		auto in_tri = [&rast_tri, &wind, &edge_fn](vec2f &p)
		{
			bool inside = true;
			if (wind == winding::clockwise)
			{
				inside &= edge_fn(rast_tri.v[0].v, rast_tri.v[2].v, p);
				inside &= edge_fn(rast_tri.v[1].v, rast_tri.v[0].v, p);
				inside &= edge_fn(rast_tri.v[2].v, rast_tri.v[1].v, p);
			}
			else
			{
				inside &= edge_fn(rast_tri.v[0].v, rast_tri.v[1].v, p);
				inside &= edge_fn(rast_tri.v[1].v, rast_tri.v[2].v, p);
				inside &= edge_fn(rast_tri.v[2].v, rast_tri.v[0].v, p);
			}
			return inside;
		};
		const float denom = 1.f / ((rast_tri.v[0][0] - rast_tri.v[2][0]) * (rast_tri.v[1][1] - rast_tri.v[0][1]) - (rast_tri.v[0][0] - rast_tri.v[1][0]) * (rast_tri.v[2][1] - rast_tri.v[0][1]));
		const vec3f barycentric_d0 = denom * vec3f(rast_tri.v[1][1] - rast_tri.v[2][1], rast_tri.v[2][1] - rast_tri.v[0][1], rast_tri.v[0][1] - rast_tri.v[1][1]);
		const vec3f barycentric_d1 = denom * vec3f(rast_tri.v[2][0] - rast_tri.v[1][0], rast_tri.v[0][0] - rast_tri.v[2][0], rast_tri.v[1][0] - rast_tri.v[0][0]);
		const vec3f barycentric_0 = denom * vec3f(
												rast_tri.v[1][0] * rast_tri.v[2][1] - rast_tri.v[2][0] * rast_tri.v[1][1],
												rast_tri.v[2][0] * rast_tri.v[0][1] - rast_tri.v[0][0] * rast_tri.v[2][1],
												rast_tri.v[0][0] * rast_tri.v[1][1] - rast_tri.v[1][0] * rast_tri.v[0][1]);
		triangle_2d::bounding_box box;
		rast_tri.get_bounding_box(box);
		if (box.top < 0)
			box.top = 0;
		if (box.left < 0)
			box.left = 0;
		if (box.right >= width)
			box.right = width - 1;
		if (box.bottom >= height)
			box.bottom = height - 1;
		for (size_t y = (size_t)box.top; y <= box.bottom; ++y)
			for (size_t x = (size_t)box.left; x <= box.right; ++x)
			{
				vec2f p((float)x, (float)y);
				if (!in_tri(p))
					continue;
				vec4f frag_coord;
				frag_coord.x() = (float)x + 0.5f;
				frag_coord.y() = (float)y + 0.5f;
				const vec3f barycentric = frag_coord[0] * barycentric_d0 + frag_coord[1] * barycentric_d1 + barycentric_0;
				// if (barycentric[0] < 0 || barycentric[1] < 0 || barycentric[2] < 0)
				// 	continue;
				frag_coord[2] = barycentric.dot(vec3f(rast_tri.v[0][2], rast_tri.v[1][2], rast_tri.v[2][2]));
				frag_coord[3] = barycentric.dot(vec3f(rast_tri.v[0][3], rast_tri.v[1][3], rast_tri.v[2][3]));
				if (frag_coord[2] < 0 || frag_coord[2] > 1)
					continue;
				const vec3f perspective = 1 / frag_coord[3] * barycentric * vec3f(rast_tri.v[0][3], rast_tri.v[1][3], rast_tri.v[2][3]);
				float frag_depth = barycentric.dot(vec3f(rast_tri.v[0][2], rast_tri.v[1][2], rast_tri.v[2][2]));

				if (frag_depth < depth_buffer[(y * width) + x])
				{
					screen[(y * width) + x].sym = rast_tri.v[0].sym;
					depth_buffer[(y * width) + x] = frag_depth;
				}
			}
	};
	mesh3d::triangle *tris = new mesh3d::triangle[m.n_tris];
	buff->get_data(tris, 0, m.n_tris * sizeof(mesh3d::triangle));
	if (!tris)
		return;
	for (size_t i = 0; i < m.n_tris; ++i)
		render_tri_3d(tris[i]);
	delete[] tris;
}
void terminal_render_base::depth_buffer_active(bool b)
{
	if (b == use_depth_buffer)
		return;
	DEL_ARR_PTR(depth_buffer);
	if (use_depth_buffer = b)
		depth_buffer = new float[width * height];
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
keystate terminal_render_base::get_keystate(key key)
{
	return NoPress;
}
char terminal_render_base::intensity_char(float i)
{
	if (i <= 0)
		return '\'';
	if (i < 0.05)
		return '.';
	if (i < 0.1)
		return ',';
	if (i < 0.15)
		return '`';
	if (i < 0.2)
		return '\"';
	if (i < 0.25)
		return '-';
	if (i < 0.275)
		return 'k';
	if (i < 0.3)
		return '_';
	if (i < 0.325)
		return 'h';
	if (i < 0.35)
		return '~';
	if (i < 0.375)
		return '^';
	if (i < 0.4)
		return '!';
	if (i < 0.425)
		return 't';
	if (i < 0.45)
		return '|';
	if (i < 0.475)
		return 'r';
	if (i < 0.5)
		return '1';
	if (i < 0.525)
		return '>';
	if (i < 0.55)
		return '/';
	if (i < 0.575)
		return 'f';
	if (i < 0.6)
		return '\\';
	if (i < 0.625)
		return 'l';
	if (i < 0.65)
		return '(';
	if (i < 0.675)
		return 'v';
	if (i < 0.7)
		return ')';
	if (i < 0.725)
		return 'j';
	if (i < 0.75)
		return '[';
	if (i < 0.775)
		return 'I';
	if (i < 0.8)
		return ']';
	if (i < 0.825)
		return 'o';
	if (i < 0.85)
		return '{';
	if (i < 0.875)
		return 'd';
	if (i < 0.9)
		return '}';
	if (i < 0.925)
		return '0';
	if (i < 0.95)
		return '$';
	if (i < 0.975)
		return '%';
	return '#';
}
terminal_render_base::triangle_2d::triangle_2d() {}

void terminal_render_base::triangle_2d::set_sym(char sym)
{
	for (size_t i = 0; i < 3; i++)
		v[i].sym = sym;
}

void terminal_render_base::triangle_2d::get_bounding_box(bounding_box &box)
{
	box.left = v[0].v[0];
	box.top = v[0].v[1];
	for (int i = 0; i < 3; i++)
	{
		if (v[i].v[1] < box.top)
			box.top = v[i].v[1];
		if (v[i].v[1] > box.bottom)
			box.bottom = v[i].v[1];
		if (v[i].v[0] < box.left)
			box.left = v[i].v[0];
		if (v[i].v[0] > box.right)
			box.right = v[i].v[0];
	}
}

terminal_render_base::winding terminal_render_base::triangle_2d::get_wind()
{
	if (facing_view)
		return base_wind;
	else if (base_wind == winding::counter_clockwise)
		return winding::clockwise;
	return winding::counter_clockwise;
}

float &terminal_render_base::triangle_2d::vert::operator[](size_t i)
{
	return this->v[i];
}