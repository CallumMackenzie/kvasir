
#include "krc-packer.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <inttypes.h>

using namespace kvasir;

#define KRC_FILE_LOC RESOURCE("../test.krc")

COUNT_MEMORY

struct resource_demo : kvasir_engine
{
	camera3d cam;
	mesh3d tmsh;
	vec3f euler;
	shader_base *shader = nullptr;

	user_result on_start()
	{
		cam.pos = vec3f(0, 2, -6);
		base->set_clear_colour(0x0f0f0f);
		base->depth_buffer_active(true);

		packer::krc_file des = packer::krc_file::deserialize(KRC_FILE_LOC);

		if (!tmsh.load_from_tri_data(des.get_mesh3d_data("Sphere"), base->make_buffer()))
			// if (!tmsh.load_from_obj("D:\\3D Models\\isont.obj", base->make_buffer()))
			return user_result("Tmsh failed loading.");
		tmsh.set_material(make_material(base, des.get_texture("Banana")));
		// tmsh.set_material(make_material(base, "D:\\Images\\71OpO-3gUfL.png"));

		shader = base->make_shader();
		const char *s[2] GL_SHADER_ARR(diffuse3d);
		if (!shader->compile(s, 2))
			return user_result("Shader failed compiling");

		return user_result::ok();
	}
	void on_update()
	{
		euler += vec3f(1, 1, 1) * time.delta();
		tmsh.rot.euler(euler);

		cam_debug_controls(base, cam, time.delta());
		base->clear();
		base->render_mesh3d(cam, tmsh, shader);
		base->swap_buffers();

		if (base->key_pressed(Escape))
			base->set_should_close(true);
	}
	void on_fixed_update()
	{
		if (base->get_type() == render_base::TERMINAL)
			cam.aspect = (float)base->get_aspect() * 2.f;
		else
			cam.aspect = (float)base->get_aspect();
	}
	void on_close()
	{
		DEL_PTR(shader);
	}
};

int main(int, char **)
{

	try
	{
#if 0
		packer::krc_file strt; // packer::obj_data_to_krc("Cube", data::objects3d::get_cube_obj());
		strt.add_mesh3d_from_obj_data("Cube", data::objects3d::get_cube_obj());
		strt.add_mesh3d_from_obj_data("Prism", data::objects3d::get_rect_prism_obj());
		strt.add_texture("Banana", "D:\\Images\\Bark_Pine_height.png", true);
		strt.add_mesh3d_from_obj_file("Sphere", "D:\\3D Models\\isont.obj");
		if (!strt.save(KRC_FILE_LOC))
			throw std::exception("Could not save resource.");
#endif

		kvasir_init();
		resource_demo kvs;
		kvasir_engine::result res = kvs.start(render_base::OPENGL);
		if (res != kvasir_engine::NO_ERROR)
			std::cerr << "Kvasir engine crashed with code " << res << std::endl;
		kvasir_destroy();
	}
	catch (std::exception e)
	{
		std::cerr << "FATAL EXCEPTION: " << e.what() << std::endl;
	}

	PRINT_MEMORY_SUMMARY

	return 0;
}