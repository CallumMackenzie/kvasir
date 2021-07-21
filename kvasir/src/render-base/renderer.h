#ifndef KVASIR_RENDERER_H
#define KVASIR_RENDERER_H 1

#ifdef _WIN32
typedef unsigned int uint;
#endif

#include "opengl.h"

namespace kvasir
{
	enum class renderer_base_type
	{
		OPENGL = 0,
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
		virtual void set_fullscreen() = 0;
		virtual void remove_fullscreen() = 0;
		virtual bool is_fullscreen() = 0;
		virtual void set_resizable(bool) = 0;
		virtual bool is_resizable() = 0;
		virtual bool init(const char *, int, int) = 0;

		int get_position_x()
		{
			int x, y;
			get_position(&x, &y);
			return x;
		}
		int get_position_y()
		{
			int x, y;
			get_position(&x, &y);
			return y;
		}
		int get_width()
		{
			int x, y;
			get_size(&x, &y);
			return x;
		};
		int get_height()
		{
			int x, y;
			get_size(&x, &y);
			return y;
		};
		void set_width(int w)
		{
			set_size(w, get_height());
		}
		void set_height(int h)
		{
			set_size(get_width(), h);
		}
	};

	struct gl_base : renderer_base
	{
		bool should_close()
		{
			return win.should_close();
		}
		void set_clear_colour(long colour)
		{
			win.set_clear_colour(colour);
		}
		void clear()
		{
			win.clear();
		}
		void swap_buffers()
		{
			win.swap_buffers();
		}
		void poll_events()
		{
			win.poll_events();
		}
		void set_position(int x, int y)
		{
			win.set_position(x, y);
		}
		void get_position(int *x, int *y)
		{
			win.get_pos(x, y);
		}
		void get_size(int *w, int *h)
		{
			win.get_size(w, h);
		}
		void set_size(int w, int h)
		{
			win.set_size(w, h);
		}
		void set_title(const char *name)
		{
			win.set_title(name);
		}
		void set_fullscreen() {}
		void remove_fullscreen() {}
		bool is_fullscreen() { return false; }
		void set_resizable(bool) {}
		bool is_resizable() { return true; }
		bool init(const char *name, int w, int h)
		{
			return gl_window::init_ok(win.init(name, w, h));
		}

	private:
		gl_window win;
	};

};

#endif