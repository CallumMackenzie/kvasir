
// #define KVASIR_DLL 1

#include <iostream>
#include <cstdlib>
#include "kvasir-eng-include.h"

COUNT_MEMORY

using namespace kvasir;

struct kvasir_demo : kvasir_engine
{
	camera3d cam;
	mesh3d ground;
	mesh3d ramp;
	std::vector<mesh3d *> mshs;
	shader_base *shader = nullptr;
	physics3d *p3d = nullptr;

	user_result on_start()
	{
		fixed_time.set_fps(5);
		time.set_fps(144);
		cam.far = 1000;
		base->set_clear_colour(0x0f0f0f);
		base->depth_buffer_active(true);
		p3d = default_physics3d();
		if (!p3d)
			return user_result("Physics was nullptr.");

		if (!ground.load_from_obj_data(data::objects3d::cube_obj, base->make_buffer()))
			return user_result("Ground failed loading.");
		ground.pos.y() = -2;
		ground.set_material(make_material(base, 0xffffff));
		vec3f scale(300.f, 2.f, 300.f);
		ground.vertex_scale(scale);
		p3d->add_mesh_box_hitbox(ground, scale, physics3d::static_props());

		if (!ramp.load_from_obj_data(data::objects3d::rect_prism_obj, base->make_buffer()))
			return user_result("Ground failed loading.");
		ramp.pos.z() = 30;
		ramp.set_material(make_material(base, 0x80bf80));
		ramp.vertex_scale(vec3f(10, 9, 10));
		p3d->add_mesh(ramp, true, physics3d::static_props());

		for (size_t i = 0; i < 20; ++i)
		{
			mesh3d *ms = new mesh3d();
			if (!ms->load_from_obj_data(data::objects3d::cube_obj, base->make_buffer()))
				return user_result("Ground failed loading.");
			ms->pos = vec3f((float)(i % 2) * 0.4f, (float)i * 3.f + 2, (float)(i % 3) * 0.4f);
			ms->set_material(make_material(base, 0xfa4c3a));
			p3d->add_mesh_box_hitbox(*ms, vec3f(1, 1, 1), physics3d::dynamic_props(1));
			mshs.push_back(ms);
		}

		shader = base->make_shader();
		const char *s[2] GL_SHADER_ARR(diffuse3d);
		if (!shader->compile(s, 2))
			return user_result("Shader failed compiling");

		return user_result::ok();
	}
	void on_update()
	{
		for (size_t i = 0; i < mshs.size(); ++i)
			if ((mshs[i]->pos - cam.pos).len() <= 4)
				p3d->activate(*mshs[i]);
		if (base->key_pressed(Num1))
			for (size_t i = 0; i < mshs.size(); ++i)
				p3d->add_central_force(*mshs[i], (cam.pos - mshs[i]->pos).normalized());

		p3d->step(time.delta());

		cam_debug_controls(base, cam, time.delta());
		for (size_t i = 0; i < mshs.size(); ++i)
		{
			mshs[i]->pos = p3d->get_position(*mshs[i]);
			mshs[i]->rot = p3d->get_rotation(*mshs[i]);
		}

		base->clear();
		base->render_mesh3d(cam, ground, shader);
		for (size_t i = 0; i < mshs.size(); ++i)
			base->render_mesh3d(cam, *mshs[i], shader);
		base->render_mesh3d(cam, ramp, shader);
		base->swap_buffers();
		std::cout << "FPS: " << (1.0 / time.delta_d()) << std::endl;

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
		DEL_PTR(p3d);
		for (size_t i = 0; i < mshs.size(); ++i)
			DEL_PTR(mshs[i]);
		mshs.clear();
	}
};

int main(int, char **)
{
	{
		kvasir_init();
		kvasir_demo kvs;
		// kvasir_engine::result res = kvs.start(render_base::TERMINAL, "Kvasir", 75, 32);
		kvasir_engine::result res = kvs.start(render_base::OPENGL);
		if (res != kvasir_engine::NO_ERROR)
			std::cerr << "Kvasir engine crashed with code " << res << std::endl;
		kvasir_destroy();
	}

	PRINT_MEMORY_SUMMARY
}