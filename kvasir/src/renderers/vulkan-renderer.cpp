#include "vulkan-renderer.h"

using namespace kvasir;

vulkan_render_base::~vulkan_render_base()
{
	if (instance)
		vkDestroyInstance(instance, nullptr);
	win.set_default_hints();
}
bool vulkan_render_base::should_close() { return win.should_close(); }
void vulkan_render_base::set_clear_colour(long) { throw UNIMPLEMENTED_EXCEPTION; }
void vulkan_render_base::clear() { throw UNIMPLEMENTED_EXCEPTION; }
void vulkan_render_base::swap_buffers() { throw UNIMPLEMENTED_EXCEPTION; }
void vulkan_render_base::poll_events() { win.poll_events(); }
void vulkan_render_base::set_position(int x, int y) { win.set_pos(x, y); }
void vulkan_render_base::get_position(int *x, int *y) { win.get_pos(x, y); }
void vulkan_render_base::get_size(int *w, int *h) { win.get_size(w, h); }
void vulkan_render_base::set_size(int w, int h) { win.set_size(w, h); }
void vulkan_render_base::set_title(const char *n) { win.set_title(n); }
bool vulkan_render_base::is_fullscreen() { return win.is_fullscreen(); }
bool vulkan_render_base::set_fullscreen()
{
	win.set_hints(hints, 1);
	if (!win.recreate_fullscreen())
		return false;
	// Restore state
	return true;
}
bool vulkan_render_base::set_windowed()
{
	win.set_hints(hints, 1);
	if (!win.recreate_windowed())
		return false;
	// Restore state
	return true;
}
bool vulkan_render_base::set_resizable(bool b)
{
	win.set_hints(hints, 1);
	if (!win.recreate_resizable(b))
		return false;
	// Restore state
	return true;
}
bool vulkan_render_base::is_resizable() { return win.is_resizable(); }
bool vulkan_render_base::set_visible(bool visible)
{
	win.set_hints(hints, 1);
	if (!win.recreate_visible(visible))
		return false;
	// Restore state
	return true;
}
bool vulkan_render_base::is_visible() { return win.is_visible(); }
VkApplicationInfo vulkan_render_base::vk_app_info(const char *n)
{
	VkApplicationInfo app_info{};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = n;
	app_info.pEngineName = "Kvasir";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;
	return app_info;
}
VkInstanceCreateInfo vulkan_render_base::vk_inst_create_info(VkApplicationInfo &app_info)
{
	uint32_t glfw_ext_count = 0;
	const char **glfw_exts = nullptr;
	glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_ext_count);

	VkInstanceCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;
	create_info.enabledExtensionCount = glfw_ext_count;
	create_info.ppEnabledExtensionNames = glfw_exts;
	create_info.enabledLayerCount = 0;
	return create_info;
}
bool vulkan_render_base::init(const char *n, int w, int h)
{
	win.set_default_hints();
	int other_hints[2]{
		GLFW_RESIZABLE, GLFW_FALSE};
	win.set_hints(hints, 1);
	win.set_hints(other_hints, 1);
	win.create_window(n, w, h);

	auto app_info = vk_app_info(n);
	auto create_info = vk_inst_create_info(app_info);
	if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
	{
		std::cerr << "Vulkan failed to create an instance." << std::endl;
		return false;
	}

	uint32_t n_extensions = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &n_extensions, nullptr);
	std::vector<VkExtensionProperties> extensions(n_extensions);
	vkEnumerateInstanceExtensionProperties(nullptr, &n_extensions, extensions.data());
	std::cout << "VULKAN: available extensions:\n";
	for (const auto &extension : extensions)
		std::cout << '\t' << extension.extensionName << '\n';

	return false;
}
void vulkan_render_base::destroy() { win.destroy(); }
buffer_base *vulkan_render_base::make_buffer()
{
	throw UNIMPLEMENTED_EXCEPTION;
	return nullptr;
}
shader_base *vulkan_render_base::make_shader()
{
	throw UNIMPLEMENTED_EXCEPTION;
	return nullptr;
}
texture_base *vulkan_render_base::make_texture()
{
	throw UNIMPLEMENTED_EXCEPTION;
	return nullptr;
}
material_base *vulkan_render_base::make_material()
{
	throw UNIMPLEMENTED_EXCEPTION;
	return nullptr;
}
void vulkan_render_base::render_mesh3d(camera3d &, mesh3d &, shader_base *)
{
	throw UNIMPLEMENTED_EXCEPTION;
}
void vulkan_render_base::depth_buffer_active(bool)
{
	throw UNIMPLEMENTED_EXCEPTION;
}