#ifndef KVASIR_RENDERER_H_
#define KVASIR_RENDERER_H_ 1

#ifdef BUILD_TERMINAL

#include <stdio.h>
#include <vector>
#include <unordered_map>
#ifndef NO_USE_INCLUDES
#include "renderer.h"
#include "exception.h"
#include "memory-aid.h"
#endif

// TODO(Callum): Frustrum culling

namespace kvasir
{
	struct terminal_buffer_base : buffer_base
	{
		static inline terminal_buffer_base *bound = nullptr;

		~terminal_buffer_base();
		void gen_buffer();
		void set_data(void *data, size_t len, bool change_often = false);
		void sub_data(void *data, size_t offset, size_t len);
		void get_data(void *recv, size_t offset, size_t len);
		void attrib_ptr(size_t ptr_num, size_t size, size_t step = 0, size_t offset = 0);
		void bind_buffer();
		void bind_vao();
		void free_buffer();

		struct attrib
		{
			size_t bytesize = 0;
			size_t step = 0;
			size_t offset = 0;
		};
		char *data = nullptr;
		std::vector<attrib> attribs;
		size_t bytelen = 0;
	};

	struct terminal_texture_base : texture_base
	{
		static inline std::vector<terminal_texture_base *> slots{nullptr};
		static inline size_t slot = 0;

		~terminal_texture_base();
		void bind();
		void gen_texture();
		void set_slot(size_t slot);
		void set_texture(const texture_image &img);
		void free_texture();

		texture_image texture;
	};

	struct terminal_shader_base : shader_base
	{
		static inline terminal_shader_base *used = nullptr;

		~terminal_shader_base();
		void use();
		bool compile(const char **srcs, size_t n_srcs);
		void u_bool1(const char *name, bool a);
		void u_bool2(const char *name, bool a, bool b);
		void u_bool3(const char *name, bool a, bool b, bool c);
		void u_bool4(const char *name, bool a, bool b, bool c, bool d);
		void u_int1(const char *name, int a);
		void u_int2(const char *name, int a, int b);
		void u_int3(const char *name, int a, int b, int c);
		void u_int4(const char *name, int a, int b, int c, int d);
		void u_float1(const char *name, float a);
		void u_float2(const char *name, float a, float b);
		void u_float3(const char *name, float a, float b, float c);
		void u_float4(const char *name, float a, float b, float c, float d);
		void u_mat2f(const char *name, float m[2][2]);
		void u_mat3f(const char *name, float m[3][3]);
		void u_mat4f(const char *name, float m[4][4]);
		void render(int n_tris);
		void free_shader();

	private:
		std::unordered_map<const char *, int> ui1s;
		std::unordered_map<const char *, vec2i> ui2s;
		std::unordered_map<const char *, vec3i> ui3s;
		std::unordered_map<const char *, vec4i> ui4s;

		std::unordered_map<const char *, float> uf1s;
		std::unordered_map<const char *, vec2f> uf2s;
		std::unordered_map<const char *, vec3f> uf3s;
		std::unordered_map<const char *, vec4f> uf4s;

		std::unordered_map<const char *, mat2f> um2fs;
		std::unordered_map<const char *, mat3f> um3fs;
		std::unordered_map<const char *, mat4f> um4fs;
	};

	struct terminal_render_base : render_base
	{
		terminal_render_base();
		~terminal_render_base();
		bool should_close();
		void set_clear_colour(long c);
		void clear();
		void swap_buffers();
		void poll_events();
		void set_position(int x, int y);
		void get_position(int *x, int *y);
		void get_size(int *w, int *h);
		void set_size(int w, int h);
		void set_title(const char *n);
		bool is_fullscreen();
		bool set_fullscreen();
		bool set_windowed();
		bool set_resizable(bool b);
		bool is_resizable();
		bool set_visible(bool visible);
		bool is_visible();
		bool init(const char *n, int w, int h);
		void destroy();
		buffer_base *make_buffer();
		shader_base *make_shader();
		texture_base *make_texture();
		material_base *make_material();
		void render_mesh3d(camera3d &c, mesh3d &m, shader_base *s, render_buffer *buff = nullptr);
		void render_mesh2d(camera2d &c, mesh2d &m, shader_base *s, render_buffer *buff = nullptr);
		void depth_buffer_active(bool b);
		type get_type();
		keystate get_keystate(key key);

	private:
		static char intensity_char(float i);
		static const char *get_ansi(long col);
		struct pixel
		{
			char sym = ' ';
			long colour = 0xffffff;
		};
		enum class winding
		{
			clockwise,
			counter_clockwise
		};
		struct triangle_2d
		{
			struct bounding_box
			{
				float top, left, right, bottom = 0;
			};
			struct vert
			{
				vec4f v;
				vec2f t;
				char sym = '0';
				float &operator[](size_t i);
			};

			vert v[3];
			winding base_wind = winding::counter_clockwise;
			bool facing_view = false;

			triangle_2d();
			void set_sym(char sym);
			void get_bounding_box(bounding_box &box);
			winding get_wind();
		};

		float *depth_buffer = nullptr;
		pixel *screen = nullptr;
		size_t width = 0;
		size_t height = 0;
		float depth_buffer_clear = 1.f;
		const char *title = "";
		bool s_close = false;
		bool use_depth_buffer = false;
		vec3us clear_colour;
		bool render_colour = true;
	};
}

#endif
#endif