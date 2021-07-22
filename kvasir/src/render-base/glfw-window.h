#ifndef GLFW_WIN_H_
#define GLFW_WIN_H_ 1

#include "glfw3.h"
#include "glfw3native.h"

struct glfw_window
{
	glfw_window()
	{
		if (win_instances <= 0)
			glfwInit();
		++win_instances;
	}
	~glfw_window()
	{
		--win_instances;
		if (win_instances <= 0)
			glfwTerminate();
	}

	void set_hints(int *hints, unsigned int n_hints)
	{
		for (size_t i = 0; i < 2 * n_hints; i += 2)
			glfwWindowHint(hints[i], hints[i + 1]);
	}
	bool create_window(const char *title, int width, int height, GLFWmonitor *monitor = NULL, GLFWwindow *share = NULL)
	{
		if (window)
			return false;
		if (monitor != NULL)
			fullscreen = true;
		else 
			fullscreen = false;
		window = glfwCreateWindow(width, height, title, monitor, share);
		return window;
	}
	bool create_fullscreen_window(const char *title)
	{
		int w, h;
		if (!create_window(title, 10, 10))
			return false;
		get_primary_monitor_size(&w, &h);
		set_size(w, h);
		return true;
	}
	inline void set_gl_current_context() { glfwMakeContextCurrent(window); }
	inline void set_resize_callback(GLFWframebuffersizefun on_win_resize) { glfwSetFramebufferSizeCallback(window, on_win_resize); }
	inline bool should_close() { return glfwWindowShouldClose(window); }
	inline void swap_buffers() { glfwSwapBuffers(window); }
	inline void poll_events() { glfwPollEvents(); }
	inline void set_pos(int screen_x, int screen_y) { glfwSetWindowPos(window, screen_x, screen_y); }
	int get_width()
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		return width;
	}
	int get_height()
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		return height;
	}
	inline void get_size(int *w, int *h) { glfwGetWindowSize(window, w, h); }
	inline void set_size(int w, int h) { glfwSetWindowSize(window, w, h); }
	inline void get_pos(int *x, int *y) { glfwGetWindowPos(window, x, y); }
	inline void set_title(const char *title) { glfwSetWindowTitle(window, title); }
	inline void close() { glfwSetWindowShouldClose(window, true); }
	inline void destroy() { glfwDestroyWindow(window); }
	inline bool is_fullscreen() { return fullscreen; }

	static void get_primary_monitor_size(int *w, int *h)
	{
		const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		*w = mode->width;
		*h = mode->height;
	}

private:
	GLFWwindow *window = nullptr;
	bool fullscreen = false;
	inline static unsigned int win_instances = 0;
};

#endif