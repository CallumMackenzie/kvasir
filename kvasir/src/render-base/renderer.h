#ifndef KVASIR_RENDERER_H
#define KVASIR_RENDERER_H 1

#ifdef _WIN32
typedef unsigned int uint;
#endif

#include "opengl.h"
#include "shader.h"
#include "buffer.h"
#include "mesh.h"
#include "camera.h"

namespace kvasir
{
	enum class renderer_base_type
	{
		OPENGL = 0b0001,
		VULKAN = 0b0010,
		TERMINAL = 0b0100
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

};

#endif