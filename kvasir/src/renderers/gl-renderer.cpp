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
	shader_id = glCreateProgram();
	uint vs = compile_shader(srcs[0], GL_VERTEX_SHADER);
	if (vs == GL_NONE)
		return false;
	uint fs = compile_shader(srcs[1], GL_FRAGMENT_SHADER);
	if (fs == GL_NONE)
		return false;
	glAttachShader(shader_id, vs);
	glAttachShader(shader_id, fs);
	glLinkProgram(shader_id);
	glValidateProgram(shader_id);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return true;
}
void gl_shader_base::use()
{
	glUseProgram(shader_id);
}
uint gl_shader_base::gsl(const char *name)
{
	return glGetUniformLocation(shader_id, name);
}
void gl_shader_base::u_bool1(const char *n, bool a)
{
	glUniform1i(gsl(n), a);
}
void gl_shader_base::u_bool2(const char *n, bool a, bool b)
{
	glUniform2i(gsl(n), a, b);
}
void gl_shader_base::u_bool3(const char *n, bool a, bool b, bool c)
{
	glUniform3i(gsl(n), a, b, c);
}
void gl_shader_base::u_bool4(const char *n, bool a, bool b, bool c, bool d)
{
	glUniform4i(gsl(n), a, b, c, d);
}
void gl_shader_base::u_int1(const char *n, int a)
{
	glUniform1i(gsl(n), a);
}
void gl_shader_base::u_int2(const char *n, int a, int b)
{
	glUniform2i(gsl(n), a, b);
}
void gl_shader_base::u_int3(const char *n, int a, int b, int c)
{
	glUniform3i(gsl(n), a, b, c);
}
void gl_shader_base::u_int4(const char *n, int a, int b, int c, int d)
{
	glUniform4i(gsl(n), a, b, c, d);
}
void gl_shader_base::u_float1(const char *n, float a)
{
	glUniform1f(gsl(n), a);
}
void gl_shader_base::u_float2(const char *n, float a, float b)
{
	glUniform2f(gsl(n), a, b);
}
void gl_shader_base::u_float3(const char *n, float a, float b, float c)
{
	glUniform3f(gsl(n), a, b, c);
}
void gl_shader_base::u_float4(const char *n, float a, float b, float c, float d)
{
	glUniform4f(gsl(n), a, b, c, d);
}
void gl_shader_base::u_mat2f(const char *n, float m[2][2])
{
	glUniformMatrix2fv(gsl(n), 1, false, &m[0][0]);
}
void gl_shader_base::u_mat3f(const char *n, float m[3][3])
{
	glUniformMatrix3fv(gsl(n), 1, false, &m[0][0]);
}
void gl_shader_base::u_mat4f(const char *n, float m[4][4])
{
	glUniformMatrix4fv(gsl(n), 1, false, &m[0][0]);
}
void gl_shader_base::render(int n_tris)
{
	glDrawArrays(GL_TRIANGLES, 0, n_tris * 3);
};
void gl_shader_base::free_shader()
{
	glDeleteProgram(shader_id);
}
uint gl_shader_base::compile_shader(const char *src, uint shader_type)
{
	uint id = glCreateShader(shader_type);
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int el;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &el);
		char *message = (char *)alloca(el * sizeof(char));
		glGetShaderInfoLog(id, el, &el, message);
		std::cerr << "Failed to compile " << (shader_type == GL_VERTEX_SHADER ? "vertex" : (shader_type == GL_FRAGMENT_SHADER ? "fragment" : "unknown")) << " shader: " << message << std::endl;
		glDeleteShader(id);
		return GL_NONE;
	}
	return id;
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

gl_render_base::~gl_render_base()
{
	win.destroy();
	win.set_default_hints();
}
bool gl_render_base::should_close()
{
	return win.should_close();
}
void gl_render_base::set_clear_colour(long colour)
{
	int r = (colour & 0xFF0000) >> 16;
	int g = (colour & 0x00FF00) >> 8;
	int b = (colour & 0x0000FF);
	clear_colour = colour;
	glClearColor((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, 1.0);
}
void gl_render_base::clear()
{
	glClear(clear_bits);
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
bool gl_render_base::set_resizable(bool res)
{
	win.set_default_hints();
	win.set_hints(gl_hints, 4);
	if (!win.recreate_resizable(res))
		return false;
	return gl_load();
}
bool gl_render_base::is_resizable()
{
	return win.is_resizable();
}
bool gl_render_base::set_fullscreen()
{
	win.set_default_hints();
	win.set_hints(gl_hints, 4);
	if (!win.recreate_fullscreen())
		return false;
	return gl_load();
}
bool gl_render_base::set_windowed()
{
	win.set_default_hints();
	win.set_hints(gl_hints, 4);
	if (!win.recreate_windowed())
		return false;
	return gl_load();
}
bool gl_render_base::set_visible(bool b)
{
	win.set_default_hints();
	win.set_hints(gl_hints, 4);
	if (!win.recreate_visible(b))
		return false;
	return gl_load();
};
bool gl_render_base::is_visible()
{
	return win.is_visible();
}
bool gl_render_base::gl_load()
{
	win.set_gl_current_context();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return false;
	glViewport(0, 0, win.get_width(), win.get_height());
	set_clear_colour(clear_colour);
	depth_buffer_active(clear_bits & GL_DEPTH_BUFFER_BIT != 0);
	return true;
}
void gl_render_base::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}
bool gl_render_base::init(const char *name, int w, int h)
{
	win.set_default_hints();
	win.set_hints(gl_hints, 4);
	if (!win.create_window(name, w, h))
	{
		std::cerr << "OpenGL GLFW window could not be created." << std::endl;
		return false;
	}
	win.set_resize_callback(framebuffer_size_callback);
	if (!gl_load())
	{
		std::cerr << "OpenGL could not be loeaded." << std::endl;
		return false;
	}
	return true;
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
		clear_bits |= GL_DEPTH_BUFFER_BIT;
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
		clear_bits ^= GL_DEPTH_BUFFER_BIT;
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
	sh->render((int)mesh.n_tris);
}
render_base::type gl_render_base::get_type()
{
	return OPENGL;
}
keystate gl_render_base::get_keystate(key key)
{
	return input::get_glfw_kst(win.get_keystate(input::get_key(key)));
}