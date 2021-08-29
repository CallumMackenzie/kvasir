
#include "krc-packer.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <inttypes.h>

using namespace kvasir;

#define KRC_FILE_LOC RESOURCE("../../../res/test.krc")

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

		// packer::krc_file des = packer::krc_file::deserialize(KRC_FILE_LOC);

		if (!tmsh.load_from_tri_data(packer::krc_file::get_mesh3d_data_from_file("Prism", KRC_FILE_LOC), base->make_buffer()))
			return user_result("Tmsh failed loading.");
		texture_image img = packer::krc_file::get_texture_from_file("Heightmp", KRC_FILE_LOC);
		tmsh.set_material(make_material(base, img));

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

		std::cout << "FPS: " << (1.0 / time.delta_d()) << std::endl;
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
		{
			// packer::krc_file strt; // packer::obj_data_to_krc("Cube", data::objects3d::get_cube_obj());
			// strt.add_mesh3d_from_obj_data("Cube", data::objects3d::get_cube_obj());
			// strt.add_mesh3d_from_obj_data("Prism", data::objects3d::get_rect_prism_obj());
			// strt.add_texture("Heightmp", "D:\\Images\\Bark_Pine_height.png", false);
			// strt.add_mesh3d_from_obj_file("Icos", "D:\\3D Models\\isont.obj");
			// strt.add_mesh3d_from_obj_file("Sphere", "D:\\3D Models\\uvsmoothnt.obj");
			// strt.add_mesh3d_from_obj_file("Mando", "D:\\3D Models\\shipnt.obj");
			// if (!strt.save(KRC_FILE_LOC))
				// throw std::runtime_error("Could not save resource.");
			// strt.clear();
		}

		kvasir_init();
		resource_demo kvs;
		kvasir_engine::result res = kvs.start(render_base::OPENGL);
		if (res != kvasir_engine::NO_ERROR)
			std::cerr << "Kvasir engine crashed with code " << res << std::endl;
		kvasir_destroy();
	}
	catch (std::runtime_error e)
	{
		std::cerr << "FATAL EXCEPTION: " << e.what() << std::endl;
	}

	PRINT_MEMORY_SUMMARY

	return 0;
}