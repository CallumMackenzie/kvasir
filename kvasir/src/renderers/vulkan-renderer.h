#ifndef KVASIR_VULKAN_RENDERER_H_
#define KVASIR_VULKAN_RENDERER_H_ 1

#ifdef BUILD_VULKAN

#define GLFW_INCLUDE_VULKAN 1

#ifndef NO_USE_INCLUDES
#include <vulkan/vulkan.h>
#include <vector>
#include "renderer.h"
#include "glfw-window.h"
#include "exception.h"
#endif

namespace kvasir
{
	struct vulkan_shader_base : shader_base
	{
	};
	struct vulkan_buffer_base : buffer_base
	{
	};
	struct vulkan_texture_base : texture_base
	{
	};
	struct vulkan_render_base : render_base
	{
		~vulkan_render_base();
		bool should_close();
		void set_clear_colour(long);
		void clear();
		void swap_buffers();
		void poll_events();
		void set_position(int, int);
		void get_position(int *, int *);
		void get_size(int *, int *);
		void set_size(int, int);
		void set_title(const char *);
		bool is_fullscreen();
		bool set_fullscreen();
		bool set_windowed();
		bool set_resizable(bool);
		bool is_resizable();
		bool set_visible(bool visible);
		bool is_visible();
		bool init(const char *, int, int);
		void destroy();
		buffer_base *make_buffer();
		shader_base *make_shader();
		texture_base *make_texture();
		material_base *make_material();
		void render_mesh3d(camera3d &, mesh3d &, shader_base *, render_buffer *);
		void depth_buffer_active(bool);
		type get_type();
		keystate get_keystate(key key);

	private:
		glfw_window win;
		int hints[2]{
			GLFW_CLIENT_API, GLFW_NO_API};
		VkInstance instance = nullptr;
#ifdef NDEBUG
		const bool use_validation_layers = false;
#else
		const bool use_validation_layers = true;
#endif
		const std::vector<const char *> validation_layers = {
			"VK_LAYER_KHRONOS_validation"};

		VkResult vk_create_instance(const char *);
		bool check_validation_layer_support();
		VkApplicationInfo vk_app_info(const char *);
		VkInstanceCreateInfo vk_inst_create_info(VkApplicationInfo &app_info);
		static void resize_callback(GLFWwindow *, int, int);
	};
}

#endif
#endif