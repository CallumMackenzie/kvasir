#ifndef KVASIR_RENDERER_H
#define KVASIR_RENDERER_H 1

#ifdef _WIN32
typedef unsigned int uint;
#endif

#ifndef NO_USE_INCLUDES
#include "shader.h"
#include "buffer.h"
#include "texture.h"
#include "mesh.h"
#include "camera.h"
#endif

namespace kvasir
{
	struct renderer_base
	{
		virtual bool should_close() = 0;
		virtual void set_clear_colour(long c) = 0;
		virtual void clear() = 0;
		virtual void swap_buffers() = 0;
		virtual void poll_events() = 0;
		virtual void set_position(int x, int y) = 0;
		virtual void get_position(int *x, int *y) = 0;
		virtual void get_size(int *w, int *h) = 0;
		virtual void set_size(int w, int h) = 0;
		virtual void set_title(const char *n) = 0;
		virtual bool is_fullscreen() = 0;
		virtual bool set_fullscreen() = 0;
		virtual bool set_windowed() = 0;
		virtual bool set_resizable(bool b) = 0;
		virtual bool is_resizable() = 0;
		virtual bool set_visible(bool visible) = 0;
		virtual bool is_visible() = 0;
		virtual bool init(const char *n, int w, int h) = 0;
		virtual void destroy() = 0;
		virtual buffer_base *make_buffer() = 0;
		virtual shader_base *make_shader() = 0;
		virtual texture_base *make_texture() = 0;
		virtual material_base *make_material() = 0;
		virtual void render_mesh3d(camera3d &c, mesh3d &m, shader_base *s) = 0;
		virtual void depth_buffer_active(bool b) = 0;

		int get_position_x();
		int get_position_y();
		int get_width();
		int get_height();
		void set_width(int w);
		void set_height(int h);
		double get_aspect();

		enum type
		{
			NONE = 0,
			OPENGL = 1,
			VULKAN = 2,
			TERMINAL = 4,
			DIRECTX = 8,
		};
	};

};

#endif