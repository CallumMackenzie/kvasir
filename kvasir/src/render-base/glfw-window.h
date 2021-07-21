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
	bool create_window(const char *title, int width, int height)
	{
		window = glfwCreateWindow(width, height, title, NULL, NULL);
		return (window);
	}
	void set_gl_current_context()
	{
		glfwMakeContextCurrent(window);
	}
	void set_resize_callback(GLFWframebuffersizefun on_win_resize)
	{
		glfwSetFramebufferSizeCallback(window, on_win_resize);
	}
	bool should_close()
	{
		return glfwWindowShouldClose(window);
	}
	void swap_buffers()
	{
		glfwSwapBuffers(window);
	}
	void poll_events()
	{
		glfwPollEvents();
	}
	void set_position(int screen_x, int screen_y)
	{
		glfwSetWindowPos(window, screen_x, screen_y);
	}
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
	void get_size(int *w, int *h)
	{
		glfwGetWindowSize(window, w, h);
	}
	void set_size(int w, int h)
	{
		glfwSetWindowSize(window, w, h);
	}
	void get_pos(int *x, int *y)
	{
		glfwGetWindowPos(window, x, y);
	}
	void set_title(const char *title)
	{
		glfwSetWindowTitle(window, title);
	}
	void close()
	{
		glfwSetWindowShouldClose(window, true);
	}

private:
	GLFWwindow *window = nullptr;
	inline static unsigned int win_instances = 0;
};

#endif