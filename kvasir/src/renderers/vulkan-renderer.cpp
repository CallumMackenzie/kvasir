#include "vulkan-renderer.h"

using namespace kvasir;

vulkan_render_base::~vulkan_render_base()
{
	if (instance)
		vkDestroyInstance(instance, nullptr);
	win.set_default_hints();
	win.destroy();
}
bool vulkan_render_base::should_close() { return win.should_close(); }
void vulkan_render_base::set_clear_colour(long) { throw EX_UNIMPLEMENTED; }
void vulkan_render_base::clear() { throw EX_UNIMPLEMENTED; }
void vulkan_render_base::swap_buffers() { throw EX_UNIMPLEMENTED; }
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
	if (use_validation_layers)
	{
		create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
		create_info.ppEnabledLayerNames = validation_layers.data();
	}
	else
		create_info.enabledLayerCount = 0;
	return create_info;
}
VkResult vulkan_render_base::vk_create_instance(const char *n)
{
	if (use_validation_layers && !check_validation_layer_support())
	{
		std::cerr << "Vulkan validation layers requested but not availible." << std::endl;
		return VK_ERROR_LAYER_NOT_PRESENT;
	}
	auto app_info = vk_app_info(n);
	auto create_info = vk_inst_create_info(app_info);
	return vkCreateInstance(&create_info, nullptr, &instance);
}
bool vulkan_render_base::check_validation_layer_support()
{
	uint32_t layer_count;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
	std::vector<VkLayerProperties> a_layers(layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, a_layers.data());
	for (const char *layer_name : validation_layers)
	{
		bool layer_found = false;
		for (const auto &layer_properties : a_layers)
			if (strcmp(layer_name, layer_properties.layerName) == 0)
			{
				layer_found = true;
				break;
			}
		if (!layer_found)
			return false;
	}
	return true;
}
bool vulkan_render_base::init(const char *n, int w, int h)
{
	win.set_default_hints();
	int other_hints[2]{
		GLFW_RESIZABLE, GLFW_FALSE};
	win.set_hints(hints, 1);
	win.set_hints(other_hints, 1);
	win.create_window(n, w, h);

	if (vk_create_instance(n) != VK_SUCCESS)
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
	throw EX_UNIMPLEMENTED;
	return nullptr;
}
shader_base *vulkan_render_base::make_shader()
{
	throw EX_UNIMPLEMENTED;
	return nullptr;
}
texture_base *vulkan_render_base::make_texture()
{
	throw EX_UNIMPLEMENTED;
	return nullptr;
}
material_base *vulkan_render_base::make_material()
{
	throw EX_UNIMPLEMENTED;
	return nullptr;
}
void vulkan_render_base::render_mesh3d(camera3d &, mesh3d &, shader_base *, render_buffer *)
{
	throw EX_UNIMPLEMENTED;
}
void vulkan_render_base::depth_buffer_active(bool)
{
	throw EX_UNIMPLEMENTED;
}
render_base::type vulkan_render_base::get_type()
{
	return VULKAN;
}
keystate vulkan_render_base::get_keystate(key key)
{
	return input::get_glfw_kst(win.get_keystate(input::get_key(key)));
}