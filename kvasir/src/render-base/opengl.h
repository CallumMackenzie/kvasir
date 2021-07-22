#ifndef OPENGL_H_CUSTOM
#define OPENGL_H_CUSTOM 1

#include "khrplatform.h"
#include "glad.h"
#include "glfw-window.h"
#include <iostream>

#ifdef _WIN32
typedef unsigned int uint;
#endif

void gl_print_errors()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
		std::cerr << "OpenGL error code: " << err << std::endl;
}

struct gl_window : glfw_window
{
	int gl_hints[8]{
		GLFW_CONTEXT_VERSION_MAJOR, 3,
		GLFW_CONTEXT_VERSION_MINOR, 3,
		GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE,
		GLFW_DOUBLEBUFFER, 1};

	enum init_result
	{
		init_success,
		null_window_ptr,
		no_gl_funcs_found
	};

	long clear_colour = 0x0f0f0f;

	void set_clear_colour(long colour)
	{
		int r = (colour & 0xFF0000) >> 16;
		int g = (colour & 0x00FF00) >> 8;
		int b = (colour & 0x0000FF);
		clear_colour = colour;
		glClearColor((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, 1.0);
	}
	inline void reset_clear_colour() { set_clear_colour(clear_colour); }
	void clear() { glClear(GL_COLOR_BUFFER_BIT); }
	inline void size_viewport() { glViewport(0, 0, get_width(), get_height()); }
	inline bool gl_load()
	{
		set_gl_current_context();
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			return false;
		size_viewport();
		reset_clear_colour();
		return true;
	}
	init_result init(const char *title = "", int s_width = 720, int s_height = 480,
					 GLFWframebuffersizefun on_win_resize = framebuffer_size_callback)
	{
		set_hints(gl_hints, 4);
		if (!create_window(title, s_width, s_height)) //, s_width, s_height))
			return null_window_ptr;
		set_resize_callback(on_win_resize);
		if (!gl_load())
			return no_gl_funcs_found;
		return init_success;
	}
	bool set_fullscreen()
	{
		set_hints(gl_hints, 4);
		if (!recreate_fullscreen())
			return false;
		return gl_load();
	}
	bool set_windowed()
	{
		set_hints(gl_hints, 4);
		if (!recreate_windowed())
			return false;
		return gl_load();
	}
	bool set_resizable(bool r)
	{
		set_hints(gl_hints, 4);
		if (!recreate_resizable(r))
			return false;
		return gl_load();
	}
	bool set_visible(bool b)
	{
		set_hints(gl_hints, 4);
		if (!recreate_visible(b))
			return false;
		return gl_load();
	}

	static void framebuffer_size_callback(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }
	static inline bool init_ok(init_result res) { return res == init_success; }
};

struct shader_vf
{
	uint shader_id = GL_NONE;
	bool init(const char *v_src, const char *f_src)
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
	inline void use()
	{
		glUseProgram(shader_id);
	}
	inline void active_texture(uint gl_tex_unit)
	{
		glActiveTexture(gl_tex_unit);
	}
	inline void bind_texture(uint tex_id)
	{
		glBindTexture(GL_TEXTURE_2D, tex_id);
	}
	inline uint gsl(const char *name)
	{
		return glGetUniformLocation(shader_id, name);
	}

	void float1(const char *n, float v)
	{
		glUniform1f(gsl(n), v);
	}
	void float2(const char *n, float a, float b)
	{
		glUniform2f(gsl(n), a, b);
	}
	void float3(const char *n, float a, float b, float c)
	{
		glUniform3f(gsl(n), a, b, c);
	}
	void float4(const char *n, float a, float b, float c, float d)
	{
		glUniform4f(gsl(n), a, b, c, d);
	}

	void int1(const char *n, int v)
	{
		glUniform1i(gsl(n), v);
	}
	void int2(const char *n, int a, int b)
	{
		glUniform2i(gsl(n), a, b);
	}
	void int3(const char *n, int a, int b, int c)
	{
		glUniform3i(gsl(n), a, b, c);
	}
	void int4(const char *n, int a, int b, int c, int d)
	{
		glUniform4i(gsl(n), a, b, c, d);
	}

	void bool1(const char *n, bool v)
	{
		glUniform1i(gsl(n), v);
	}
	void bool2(const char *n, bool a, bool b)
	{
		glUniform2i(gsl(n), a, b);
	}
	void bool3(const char *n, bool a, bool b, bool c)
	{
		glUniform3i(gsl(n), a, b, c);
	}
	void bool4(const char *n, bool a, bool b, bool c, bool d)
	{
		glUniform4i(gsl(n), a, b, c, d);
	}

	void mat4f(const char *name, float mat[4][4])
	{
		glUniformMatrix4fv(gsl(name), 1, false, &mat[0][0]);
	}

	void mat3f(const char *name, float mat[3][3])
	{
		glUniformMatrix3fv(gsl(name), 1, false, &mat[0][0]);
	}

	void mat2f(const char *name, float mat[2][2])
	{
		glUniformMatrix2fv(gsl(name), 1, false, &mat[0][0]);
	}

	static uint compile_shader(const char *src, uint shader_type)
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
};

#endif