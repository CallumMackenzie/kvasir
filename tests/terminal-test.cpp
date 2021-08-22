
#include "kvasir-eng-include.h"

COUNT_MEMORY

using namespace kvasir;

struct kvasir_demo : kvasir_engine
{
	camera3d cam;
	mesh3d test_m;
	vec3f euler;
	float timer;

	user_result on_start()
	{
		fixed_time.set_fps(5);
		time.set_fps(144);
		cam.far = 1000;
		base->depth_buffer_active(true);

		if (!test_m.load_from_obj_data(data::objects3d::cube_obj, base->make_buffer()))
			return user_result("A mesh failed loading.");
		test_m.pos.z() = 4;
		test_m.material = make_material(base, 0xffaaff);

		return user_result::ok();
	}
	void on_update()
	{
		test_m.rot.euler(euler += vec3f(1, 1, 1) * time.delta());

		base->clear();
		base->render_mesh3d(cam, test_m, nullptr);
		base->swap_buffers();
		// std::cout << "FPS: " << (1.0 / time.delta_d()) << std::endl;
	}
	void on_fixed_update()
	{
		cam.aspect = (float)base->get_aspect() * 2.f;
	}
	void on_close()
	{
	}
};

int main(int, char **)
{
	{
		kvasir_init();
		kvasir_demo kvs;
		kvasir_engine::result res = kvs.start(render_base::TERMINAL, "Kvasir", 75 * 3, 32 * 3);
		if (res != kvasir_engine::NO_ERROR)
			std::cerr << "Kvasir engine crashed with code " << res << std::endl;
	}

	PRINT_MEMORY_SUMMARY
}