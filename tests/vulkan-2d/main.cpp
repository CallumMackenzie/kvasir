#include "kvasir-eng-include.h"

COUNT_MEMORY

using namespace kvasir;

struct kvasir_demo : kvasir_engine
{
	camera2d cam;
	mesh2d test_m;
	shader_base *shader = nullptr;
	vec3f euler;
	float timer;

	user_result on_start()
	{
		test_m.make_quad(vec2f(1, 1), base->make_buffer());
		test_m.set_material(make_material(base, 0x00aaff));
		return user_result::ok();
	}
	void on_update()
	{
		base->clear();
		base->render_mesh2d(cam, test_m, shader);
		base->swap_buffers();
		// std::cout << "FPS: " << (1.0 / time.delta_d()) << std::endl;
	}
	void on_fixed_update()
	{
		cam.aspect = (float)base->get_aspect();
	}
	void on_close()
	{
		DEL_PTR(shader);
	}
};

int main(int, char **)
{
	{
		kvasir_init();
		kvasir_demo kvs;
		kvasir_engine::result res = kvs.start(render_base::VULKAN, "Kvasir Vulkan", 720, 480);
		if (res != kvasir_engine::NO_ERROR)
			std::cerr << "Kvasir engine crashed with code " << res << std::endl;
	}

	PRINT_MEMORY_SUMMARY
}