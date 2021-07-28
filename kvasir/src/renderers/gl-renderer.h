#ifndef KVASIR_GL_RENDERER_H
#define KVASIR_GL_RENDERER_H 1

#ifndef NO_USE_INCLUDES
#include "renderer.h"
#include "khrplatform.h"
#include "glad.h"
#include "glfw-window.h"
#endif

namespace kvasir
{

	struct gl_shader_base : shader_base
	{
		~gl_shader_base();
		bool compile(const char **srcs, size_t n_srcs);
		void use();
		void u_bool1(const char *n, bool a);
		void u_bool2(const char *n, bool a, bool b);
		void u_bool3(const char *n, bool a, bool b, bool c);
		void u_bool4(const char *n, bool a, bool b, bool c, bool d);
		void u_int1(const char *n, int a);
		void u_int2(const char *n, int a, int b);
		void u_int3(const char *n, int a, int b, int c);
		void u_int4(const char *n, int a, int b, int c, int d);
		void u_float1(const char *n, float a);
		void u_float2(const char *n, float a, float b);
		void u_float3(const char *n, float a, float b, float c);
		void u_float4(const char *n, float a, float b, float c, float d);
		void u_mat2f(const char *n, float m[2][2]);
		void u_mat3f(const char *n, float m[3][3]);
		void u_mat4f(const char *n, float m[4][4]);
		void render(int n_tris);
		void free_shader();

	private:
		uint shader_id = GL_NONE;
		uint gsl(const char *name);
		bool gl_load();
		static uint compile_shader(const char *src, uint shader_type);
		void framebuffer_size_callback(GLFWwindow *, int, int);
	};

	struct gl_buffer_base : buffer_base
	{
		~gl_buffer_base();
		void gen_buffer();
		void set_data(void *data, size_t len, bool change_often);
		void sub_data(void *data, size_t offset, size_t len);
		void get_data(void *recv, size_t offset, size_t len);
		void attrib_ptr(size_t ptr_num, size_t size, size_t step, size_t offset);
		void bind_buffer();
		void bind_vao();
		void free_buffer();

	private:
		uint vbo;
		uint vao;
	};

	struct gl_texture_base : texture_base
	{
		~gl_texture_base();
		void bind();
		void gen_texture();
		void set_texture(const texture_image &img);
		void set_slot(size_t slot);
		void free_texture();

	private:
		uint texture = GL_NONE;
		uint slot = GL_TEXTURE0;
	};

	struct gl_render_base : render_base
	{
		~gl_render_base();

		bool should_close();
		void set_clear_colour(long colour);
		void clear();
		void swap_buffers();
		void poll_events();
		void set_position(int x, int y);
		void get_position(int *x, int *y);
		void get_size(int *w, int *h);
		void set_size(int w, int h);
		void set_title(const char *name);
		bool is_fullscreen();
		bool set_resizable(bool res);
		bool is_resizable();
		bool set_fullscreen();
		bool set_windowed();
		bool set_visible(bool b);
		bool is_visible();
		bool init(const char *name, int w, int h);
		buffer_base *make_buffer();
		shader_base *make_shader();
		texture_base *make_texture();
		material_base *make_material();
		void destroy();
		void depth_buffer_active(bool a);
		void render_mesh3d(camera3d &cam, mesh3d &mesh, shader_base *sh);
		type get_type();
		keystate get_keystate(key key);

	private:
		int gl_hints[8]{
			GLFW_CONTEXT_VERSION_MAJOR, 3,
			GLFW_CONTEXT_VERSION_MINOR, 3,
			GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE,
			GLFW_DOUBLEBUFFER, 1};
		glfw_window win;
		long clear_colour = 0x0f0f0f;
		int clear_bits = GL_COLOR_BUFFER_BIT;
		static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
		bool gl_load();
	};
};

#endif