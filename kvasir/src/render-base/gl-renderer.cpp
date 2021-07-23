#include "gl-renderer.h"

using namespace kvasir;

gl_shader_base::~gl_shader_base()
{
	free_shader();
}
bool gl_shader_base::compile(const char **srcs, size_t n_srcs)
{
	if (n_srcs != 2)
		return false;
	return s.init(srcs[0], srcs[1]);
}
void gl_shader_base::use()
{
	s.use();
}
void gl_shader_base::u_bool1(const char *n, bool a)
{
	s.bool1(n, a);
}
void gl_shader_base::u_bool2(const char *n, bool a, bool b)
{
	s.bool2(n, a, b);
}
void gl_shader_base::u_bool3(const char *n, bool a, bool b, bool c)
{
	s.bool3(n, a, b, c);
}
void gl_shader_base::u_bool4(const char *n, bool a, bool b, bool c, bool d)
{
	s.bool4(n, a, b, c, d);
}
void gl_shader_base::u_int1(const char *n, int a)
{
	s.int1(n, a);
}
void gl_shader_base::u_int2(const char *n, int a, int b)
{
	s.int2(n, a, b);
}
void gl_shader_base::u_int3(const char *n, int a, int b, int c)
{
	s.int3(n, a, b, c);
}
void gl_shader_base::u_int4(const char *n, int a, int b, int c, int d)
{
	s.int4(n, a, b, c, d);
}
void gl_shader_base::u_float1(const char *n, float a)
{
	s.float1(n, a);
}
void gl_shader_base::u_float2(const char *n, float a, float b)
{
	s.float2(n, a, b);
}
void gl_shader_base::u_float3(const char *n, float a, float b, float c)
{
	s.float3(n, a, b, c);
}
void gl_shader_base::u_float4(const char *n, float a, float b, float c, float d)
{
	s.float4(n, a, b, c, d);
}
void gl_shader_base::u_mat2f(const char *n, float m[2][2])
{
	s.mat2f(n, m);
}
void gl_shader_base::u_mat3f(const char *n, float m[3][3])
{
	s.mat3f(n, m);
}
void gl_shader_base::u_mat4f(const char *n, float m[4][4])
{
	s.mat4f(n, m);
}
void gl_shader_base::render(int n_tris)
{
	glDrawArrays(GL_TRIANGLES, 0, n_tris * 3);
};
void gl_shader_base::free_shader()
{
	glDeleteProgram(s.shader_id);
}

gl_buffer_base::~gl_buffer_base()
{
	free_buffer();
}
void gl_buffer_base::gen_buffer()
{
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
}
void gl_buffer_base::set_data(void *data, size_t len, bool change_often)
{
	bind_buffer();
	glBufferData(GL_ARRAY_BUFFER, len, data, change_often ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}
void gl_buffer_base::sub_data(void *data, size_t offset, size_t len)
{
	bind_buffer();
	glBufferSubData(GL_ARRAY_BUFFER, offset, len, data);
}
void gl_buffer_base::get_data(void *recv, size_t offset, size_t len)
{
	bind_buffer();
	glGetBufferSubData(GL_ARRAY_BUFFER, offset, len, recv);
}
void gl_buffer_base::attrib_ptr(size_t ptr_num, size_t size, size_t step, size_t offset)
{
	bind_vao();
	bind_buffer();
	glEnableVertexAttribArray((GLuint)ptr_num);
	glVertexAttribPointer((GLuint)ptr_num, (GLint)size, GL_FLOAT, GL_FALSE, (GLsizei)step, (void *)offset);
}
void gl_buffer_base::bind_buffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}
void gl_buffer_base::bind_vao()
{
	glBindVertexArray(vao);
}
void gl_buffer_base::free_buffer()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

gl_texture_base::~gl_texture_base()
{
	free_texture();
}
void gl_texture_base::bind()
{
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}
void gl_texture_base::gen_texture()
{
	glGenTextures(1, &texture);
}
void gl_texture_base::set_texture(const texture_image &img)
{
	glActiveTexture(slot);
	bind();
	bool pow_2 = (img.w & (img.w - 1)) && (img.h & (img.h - 1));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pow_2 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	float px[] = {1.f, 0.5f, 1.f, 1.f};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, px);
	if (img.pixels.size() <= 0)
	{
		std::cerr << "texture_image has no pixels." << std::endl;
		return;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)img.w, (int)img.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img.pixels[0]);
	if (pow_2)
		glGenerateMipmap(GL_TEXTURE_2D);
}
void gl_texture_base::set_slot(size_t slot)
{
	slot = (uint)(GL_TEXTURE0 + slot);
}
void gl_texture_base::free_texture()
{
	glDeleteTextures(1, &texture);
}

bool gl_render_base::should_close()
{
	return win.should_close();
}
void gl_render_base::set_clear_colour(long colour)
{
	win.set_clear_colour(colour);
}
void gl_render_base::clear()
{
	win.clear();
}
void gl_render_base::swap_buffers()
{
	win.swap_buffers();
}
void gl_render_base::poll_events()
{
	win.poll_events();
}
void gl_render_base::set_position(int x, int y)
{
	win.set_pos(x, y);
}
void gl_render_base::get_position(int *x, int *y)
{
	win.get_pos(x, y);
}
void gl_render_base::get_size(int *w, int *h)
{
	win.get_size(w, h);
}
void gl_render_base::set_size(int w, int h)
{
	win.set_size(w, h);
}
void gl_render_base::set_title(const char *name)
{
	win.set_title(name);
}
bool gl_render_base::is_fullscreen()
{
	return win.is_fullscreen();
}
void gl_render_base::set_resizable(bool res)
{
	win.set_resizable(res);
}
bool gl_render_base::is_resizable()
{
	return win.is_resizable();
}
bool gl_render_base::set_fullscreen()
{
	return win.set_fullscreen();
}
bool gl_render_base::set_windowed()
{
	return win.set_windowed();
}
bool gl_render_base::set_visible(bool b)
{
	return win.set_visible(b);
};
bool gl_render_base::is_visible()
{
	return win.is_visible();
}
bool gl_render_base::init(const char *name, int w, int h)
{
	return gl_window::init_ok(win.init(name, w, h));
}
buffer_base *gl_render_base::make_buffer()
{
	return new gl_buffer_base();
}
shader_base *gl_render_base::make_shader()
{
	return new gl_shader_base();
}
texture_base *gl_render_base::make_texture()
{
	return new gl_texture_base();
}
material_base *gl_render_base::make_material()
{
	return new material_base();
}
void gl_render_base::destroy()
{
	win.destroy();
}
void gl_render_base::depth_buffer_active(bool a)
{
	if (a)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClearDepth(1.0f);
		win.clear_bits |= GL_DEPTH_BUFFER_BIT;
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
		win.clear_bits ^= GL_DEPTH_BUFFER_BIT;
	}
}
void gl_render_base::render_mesh3d(camera3d &cam, mesh3d &mesh, shader_base *sh)
{
	if (!sh || !mesh.buffer)
		return;
	mat4f m_projection = cam.perspective(),
		  m_rotation = mat4f::rotation(mesh.rot),
		  m_scale = mat4f::scale(mesh.scale),
		  m_translation = mat4f::translation(mesh.pos),
		  m_view = cam.view().inverse();

	sh->use();
	sh->u_int1("diff", 0);
	mesh.buffer->bind_vao();
	if (mesh.material)
		mesh.material->bind();
	sh->u_mat4f("transform", (m_scale * m_rotation * m_translation).m);
	sh->u_mat4f("view", m_view.m);
	sh->u_mat4f("projection", m_projection.m);
	sh->u_mat4f("rot", m_rotation.m);
	sh->render((int)mesh.tris.size());
}
