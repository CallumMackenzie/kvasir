#ifndef OPENGL_H_CUSTOM
#define OPENGL_H_CUSTOM 1

#include <iostream>
#ifndef NO_USE_INCLUDES
#include "khrplatform.h"
#include "glad.h"
#include "glfw-window.h"
#endif

#ifdef _WIN32
typedef unsigned int uint;
#endif

void gl_print_errors();

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
	int clear_bits = GL_COLOR_BUFFER_BIT;

	void set_clear_colour(long colour);
	void reset_clear_colour();
	void clear();
	void size_viewport();
	bool gl_load();
	init_result init(const char *title = "", int s_width = 720, int s_height = 480,
					 GLFWframebuffersizefun on_win_resize = framebuffer_size_callback);
	bool set_fullscreen();
	bool set_windowed();
	bool set_resizable(bool r);
	bool set_visible(bool b);

	static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
	static bool init_ok(init_result res);
};

struct shader_vf
{
	uint shader_id = GL_NONE;
	bool init(const char *v_src, const char *f_src);
	void use();
	void active_texture(uint gl_tex_unit);
	void bind_texture(uint tex_id);
	uint gsl(const char *name);

	void float1(const char *n, float v);
	void float2(const char *n, float a, float b);
	void float3(const char *n, float a, float b, float c);
	void float4(const char *n, float a, float b, float c, float d);

	void int1(const char *n, int v);
	void int2(const char *n, int a, int b);
	void int3(const char *n, int a, int b, int c);
	void int4(const char *n, int a, int b, int c, int d);

	void bool1(const char *n, bool v);
	void bool2(const char *n, bool a, bool b);
	void bool3(const char *n, bool a, bool b, bool c);
	void bool4(const char *n, bool a, bool b, bool c, bool d);

	void mat4f(const char *name, float mat[4][4]);
	void mat3f(const char *name, float mat[3][3]);
	void mat2f(const char *name, float mat[2][2]);

	static uint compile_shader(const char *src, uint shader_type);
};

#endif