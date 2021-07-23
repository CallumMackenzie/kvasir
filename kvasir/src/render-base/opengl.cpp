#include "opengl.h"

void gl_print_errors()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
		std::cerr << "OpenGL error code: " << err << std::endl;
}

void gl_window::set_clear_colour(long colour)
{
	int r = (colour & 0xFF0000) >> 16;
	int g = (colour & 0x00FF00) >> 8;
	int b = (colour & 0x0000FF);
	clear_colour = colour;
	glClearColor((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, 1.0);
}
void gl_window::reset_clear_colour()
{
	set_clear_colour(clear_colour);
}
void gl_window::clear()
{
	glClear(clear_bits);
}
void gl_window::size_viewport()
{
	glViewport(0, 0, get_width(), get_height());
}
bool gl_window::gl_load()
{
	set_gl_current_context();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return false;
	size_viewport();
	reset_clear_colour();
	return true;
}
gl_window::init_result gl_window::init(const char *title, int s_width, int s_height, GLFWframebuffersizefun on_win_resize)
{
	set_hints(gl_hints, 4);
	if (!create_window(title, s_width, s_height))
		return null_window_ptr;
	set_resize_callback(on_win_resize);
	if (!gl_load())
		return no_gl_funcs_found;
	return init_success;
}
bool gl_window::set_fullscreen()
{
	set_hints(gl_hints, 4);
	if (!recreate_fullscreen())
		return false;
	return gl_load();
}
bool gl_window::set_windowed()
{
	set_hints(gl_hints, 4);
	if (!recreate_windowed())
		return false;
	return gl_load();
}
bool gl_window::set_resizable(bool r)
{
	set_hints(gl_hints, 4);
	if (!recreate_resizable(r))
		return false;
	return gl_load();
}
bool gl_window::set_visible(bool b)
{
	set_hints(gl_hints, 4);
	if (!recreate_visible(b))
		return false;
	return gl_load();
}
void gl_window::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}
bool gl_window::init_ok(init_result res)
{
	return res == init_success;
}

bool shader_vf::init(const char *v_src, const char *f_src)
{
	shader_id = glCreateProgram();
	uint vs = compile_shader(v_src, GL_VERTEX_SHADER);
	if (vs == GL_NONE)
		return false;
	uint fs = compile_shader(f_src, GL_FRAGMENT_SHADER);
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
void shader_vf::use()
{
	glUseProgram(shader_id);
}
void shader_vf::active_texture(uint gl_tex_unit)
{
	glActiveTexture(gl_tex_unit);
}
void shader_vf::bind_texture(uint tex_id)
{
	glBindTexture(GL_TEXTURE_2D, tex_id);
}
uint shader_vf::gsl(const char *name)
{
	return glGetUniformLocation(shader_id, name);
}
void shader_vf::float1(const char *n, float v)
{
	glUniform1f(gsl(n), v);
}
void shader_vf::float2(const char *n, float a, float b)
{
	glUniform2f(gsl(n), a, b);
}
void shader_vf::float3(const char *n, float a, float b, float c)
{
	glUniform3f(gsl(n), a, b, c);
}
void shader_vf::float4(const char *n, float a, float b, float c, float d)
{
	glUniform4f(gsl(n), a, b, c, d);
}
void shader_vf::int1(const char *n, int v)
{
	glUniform1i(gsl(n), v);
}
void shader_vf::int2(const char *n, int a, int b)
{
	glUniform2i(gsl(n), a, b);
}
void shader_vf::int3(const char *n, int a, int b, int c)
{
	glUniform3i(gsl(n), a, b, c);
}
void shader_vf::int4(const char *n, int a, int b, int c, int d)
{
	glUniform4i(gsl(n), a, b, c, d);
}
void shader_vf::bool1(const char *n, bool v)
{
	glUniform1i(gsl(n), v);
}
void shader_vf::bool2(const char *n, bool a, bool b)
{
	glUniform2i(gsl(n), a, b);
}
void shader_vf::bool3(const char *n, bool a, bool b, bool c)
{
	glUniform3i(gsl(n), a, b, c);
}
void shader_vf::bool4(const char *n, bool a, bool b, bool c, bool d)
{
	glUniform4i(gsl(n), a, b, c, d);
}
void shader_vf::mat4f(const char *name, float mat[4][4])
{
	glUniformMatrix4fv(gsl(name), 1, false, &mat[0][0]);
}
void shader_vf::mat3f(const char *name, float mat[3][3])
{
	glUniformMatrix3fv(gsl(name), 1, false, &mat[0][0]);
}
void shader_vf::mat2f(const char *name, float mat[2][2])
{
	glUniformMatrix2fv(gsl(name), 1, false, &mat[0][0]);
}
uint shader_vf::compile_shader(const char *src, uint shader_type)
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