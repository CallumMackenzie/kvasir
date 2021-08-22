#ifndef GLFW_WIN_H_
#define GLFW_WIN_H_ 1

#ifndef NO_USE_INCLUDES
#include "glfw3.h"
#include "glfw3native.h"
#endif

struct glfw_window
{
	glfw_window();
	~glfw_window();

	void set_hints(int *hints, unsigned int n_hints);
	void set_default_hints();
	bool create_window(const char *title, int width, int height, GLFWmonitor *monitor = NULL, GLFWwindow *share = NULL);
	bool create_fullscreen_window(const char *title);
	void set_gl_current_context();
	void set_resize_callback(GLFWframebuffersizefun on_win_resize);
	bool should_close();
	void swap_buffers();
	void poll_events();
	void set_pos(int screen_x, int screen_y);
	int get_width();
	int get_height();
	void get_size(int *w, int *h);
	void set_size(int w, int h);
	void get_pos(int *x, int *y);
	void set_title(const char *title);
	const char *get_name();
	void close();
	void destroy();
	bool is_fullscreen();
	bool recreate_fullscreen();
	bool recreate_windowed(int wid = -1, int hei = -1);
	bool recreate_resizable(bool resize);
	bool is_resizable();
	bool recreate_visible(bool vis);
	bool is_visible();
	void set_key_callback(GLFWkeyfun fn);
	int get_keystate(int key);
	void set_should_close(bool sc);
	static void get_primary_monitor_size(int *w, int *h);

private:
	GLFWwindow *window = nullptr;
	bool fullscreen = false;
	bool resizable = true;
	bool visible = true;
	const char *win_name;
	inline static unsigned int win_instances = 0;
};

#endif