#ifndef KVASIR_ENGINE_INCLUDE_H
#define KVASIR_ENGINE_INCLUDE_H 1

#include <vector>
#include <time.h>
#include "./lib/glfw/include/GLFW/glfw3.h"
#include "./lib/glfw/include/GLFW/glfw3native.h"
#include "./lib/galg/galg.h"
#include "./lib/galg/galg_ops.h"
#include "./src/utils/memory-aid.h"

using namespace galg;

namespace kvasir
{

	struct position3d
	{
		vec3f pos;
		vec3f rot;
		vec3f scale = vec3f(1, 1, 1);
		vec3f rot_pt;
	};

	struct position2d
	{
		vec2f pos;
		float rot;
		vec2f scale = galg::vec2f(1, 1);
		vec2f rot_pt;
	};

	struct frame_manager
	{
		bool next_frame_ready();
		void set_fps(double fps);
		double delta() const;
		static inline double clock_to_sec(const clock_t clock);
	};

	struct buffer_base
	{
		virtual void gen_buffer() = 0;
		virtual void set_data(void *data, size_t len, bool change_often = false) = 0;
		virtual void sub_data(void *data, size_t offset, size_t len) = 0;
		virtual void get_data(void *recv, size_t offset, size_t len) = 0;
		virtual void attrib_ptr(size_t ptr_num, size_t size, size_t step = 0, size_t offset = 0) = 0;
		virtual void bind_buffer() = 0;
		virtual void bind_vao() = 0;
	};

	struct texture_base
	{
		virtual void bind() = 0;
		virtual void set_data(void *data, size_t len) = 0;
		virtual void gen_texture() = 0;
	};

	struct material_base
	{
		std::vector<texture_base *> texs{nullptr, nullptr, nullptr, nullptr};
		texture_base *get_diffuse();
		texture_base *get_specular();
		texture_base *get_normal();
		texture_base *get_parallax();
		void bind();
	};

	struct mesh3d : position3d
	{
		struct triangle
		{
			struct component
			{
				vec3f p;
				vec2f t;
				vec3f n;
			};

			component v[3];

			triangle();
			triangle(vec3f p1, vec3f p2, vec3f p3);
		};

		std::vector<triangle> tris;
		material_base *material = nullptr;
		buffer_base *buffer = nullptr;

		~mesh3d();
		void load_to_buffer(buffer_base *buf);
		bool load_from_obj(const char *file_name);
	};

	struct camera3d : position3d
	{
		float fov = 75.f;
		float near = 0.1f;
		float far = 20.f;
		float aspect = 1.f;

		mat4f view();
		mat4f perspective();
		vec4f look_vector();
	};

	struct shader_base
	{

		virtual void use() = 0;
		virtual bool compile(const char **srcs, size_t n_srcs) = 0;
		virtual void u_bool1(const char *name, bool a) = 0;
		virtual void u_bool2(const char *name, bool a, bool b) = 0;
		virtual void u_bool3(const char *name, bool a, bool b, bool c) = 0;
		virtual void u_bool4(const char *name, bool a, bool b, bool c, bool d) = 0;
		virtual void u_int1(const char *name, int a) = 0;
		virtual void u_int2(const char *name, int a, int b) = 0;
		virtual void u_int3(const char *name, int a, int b, int c) = 0;
		virtual void u_int4(const char *name, int a, int b, int c, int d) = 0;
		virtual void u_float1(const char *name, float a) = 0;
		virtual void u_float2(const char *name, float a, float b) = 0;
		virtual void u_float3(const char *name, float a, float b, float c) = 0;
		virtual void u_float4(const char *name, float a, float b, float c, float d) = 0;
		virtual void u_mat2f(const char *name, float m[2][2]) = 0;
		virtual void u_mat3f(const char *name, float m[3][3]) = 0;
		virtual void u_mat4f(const char *name, float m[4][4]) = 0;
		virtual void render(int) = 0;

		void bind_material(material_base *base);
	};

	enum class renderer_base_type
	{
		OPENGL = 0b0001,
		VULKAN = 0b0010,
		TERMINAL = 0b0100,
		NONE = 0b1000
	};

	struct renderer_base
	{
		virtual bool should_close() = 0;
		virtual void set_clear_colour(long) = 0;
		virtual void clear() = 0;
		virtual void swap_buffers() = 0;
		virtual void poll_events() = 0;
		virtual void set_position(int, int) = 0;
		virtual void get_position(int *, int *) = 0;
		virtual void get_size(int *, int *) = 0;
		virtual void set_size(int, int) = 0;
		virtual void set_title(const char *) = 0;
		virtual bool is_fullscreen() = 0;
		virtual bool set_fullscreen() = 0;
		virtual bool set_windowed() = 0;
		virtual void set_resizable(bool) = 0;
		virtual bool is_resizable() = 0;
		virtual bool set_visible(bool visible) = 0;
		virtual bool is_visible() = 0;
		virtual bool init(const char *, int, int) = 0;
		virtual void destroy() = 0;
		virtual buffer_base *make_buffer() = 0;
		virtual shader_base *make_shader() = 0;
		virtual void render_mesh3d(camera3d &, mesh3d &, shader_base *) = 0;
		virtual void depth_buffer_active(bool) = 0;

		int get_position_x();
		int get_position_y();
		int get_width();
		int get_height();
		void set_width(int w);
		void set_height(int h);
		double get_aspect();
	};

	struct linkverify
	{
		void verify_link();
	};

	struct kvasir_engine
	{
		frame_manager time;
		renderer_base *base = nullptr;
		enum result
		{
			NO_ERROR = 0x001,
			NULL_BASE = 0x002,
			BASE_INIT_FAIL = 0x003,
			ON_START_RET_FALSE = 0x004
		};

		struct user_result
		{
			user_result(const char *m, bool f = true);
			const char *msg = nullptr;
			bool fatal = true;
			static user_result ok();
		};

		kvasir_engine(renderer_base_type base_type);
		~kvasir_engine();
		bool set_base(renderer_base_type base_type);
		result start(const char *name = "Kvasir", int wid = 480, int hei = 480);
		virtual user_result on_start() = 0;
		virtual void on_update() = 0;
		virtual void on_close() = 0;

		
	};


}

#endif