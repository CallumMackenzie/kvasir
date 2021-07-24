#include "glfw-window.h"

glfw_window::glfw_window()
{
	if (win_instances <= 0)
		glfwInit();
	++win_instances;
}
glfw_window::~glfw_window()
{
	--win_instances;
	if (win_instances <= 0)
		glfwTerminate();
}
void glfw_window::set_hints(int *hints, unsigned int n_hints)
{
	for (size_t i = 0; i < 2 * n_hints; i += 2)
		glfwWindowHint(hints[i], hints[i + 1]);
}
void glfw_window::set_default_hints()
{
	glfwDefaultWindowHints();
}
bool glfw_window::create_window(const char *title, int width, int height, GLFWmonitor *monitor, GLFWwindow *share)
{
	if (window)
		return false;
	if (monitor != NULL)
		fullscreen = true;
	else
		fullscreen = false;
	window = glfwCreateWindow(width, height, win_name = title, monitor, share);
	return window;
}
bool glfw_window::create_fullscreen_window(const char *title)
{
	int w, h;
	get_primary_monitor_size(&w, &h);
	return create_window(title, w, h, glfwGetPrimaryMonitor());
}
void glfw_window::set_gl_current_context() { glfwMakeContextCurrent(window); }
void glfw_window::set_resize_callback(GLFWframebuffersizefun on_win_resize) { glfwSetFramebufferSizeCallback(window, on_win_resize); }
bool glfw_window::should_close() { return glfwWindowShouldClose(window); }
void glfw_window::swap_buffers() { glfwSwapBuffers(window); }
void glfw_window::poll_events() { glfwPollEvents(); }
void glfw_window::set_pos(int screen_x, int screen_y) { glfwSetWindowPos(window, screen_x, screen_y); }
int glfw_window::get_width()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return width;
}
int glfw_window::get_height()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return height;
}
void glfw_window::get_size(int *w, int *h) { glfwGetWindowSize(window, w, h); }
void glfw_window::set_size(int w, int h) { glfwSetWindowSize(window, w, h); }
void glfw_window::get_pos(int *x, int *y) { glfwGetWindowPos(window, x, y); }
void glfw_window::set_title(const char *title) { glfwSetWindowTitle(window, win_name = title); }
const char *glfw_window::get_name() { return win_name; }
void glfw_window::close() { glfwSetWindowShouldClose(window, true); }
void glfw_window::destroy()
{
	if (!window)
		return;
	glfwDestroyWindow(window);
	window = nullptr;
}
bool glfw_window::is_fullscreen() { return fullscreen; }
bool glfw_window::recreate_fullscreen()
{
	destroy();
	int h[2]{
		GLFW_DECORATED, 0};
	set_hints(h, 1);
	if (!create_fullscreen_window(win_name))
		return false;
	return (fullscreen = true);
}
bool glfw_window::recreate_windowed(int wid, int hei)
{
	if (wid == -1 && hei == -1)
		get_size(&wid, &hei);
	destroy();
	int h[2]{
		GLFW_DECORATED, 1};
	set_hints(h, 1);
	if (!create_window(win_name, wid, hei))
		return (fullscreen = false);
	return true;
}
bool glfw_window::recreate_resizable(bool resize)
{
	if (fullscreen)
		return false;
	int h[2]{
		GLFW_RESIZABLE, (int)resize};
	set_hints(h, 1);
	if (!recreate_windowed())
		return false;
	resizable = resize;
	return true;
}
bool glfw_window::is_resizable()
{
	return resizable;
}
bool glfw_window::recreate_visible(bool vis)
{

	int h[2]{
		GLFW_VISIBLE, (int)vis};
	set_hints(h, 1);
	if (fullscreen)
	{
		if (!recreate_fullscreen())
			return false;
	}
	else if (!recreate_windowed())
		return false;
	visible = vis;
	return true;
}
bool glfw_window::is_visible()
{
	return visible;
}
void glfw_window::get_primary_monitor_size(int *w, int *h)
{
	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	*w = mode->width;
	*h = mode->height;
}