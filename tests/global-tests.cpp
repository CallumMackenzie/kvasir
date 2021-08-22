
#include <iostream>
#include <cstdlib>
#include "kvasir-eng-include.h"

COUNT_MEMORY

using namespace kvasir;

const char *vshader = "#version 330 core\nlayout(location=0) in vec3 v_pos;layout (location = 1) in vec2 v_uv;layout (location = 2) in vec3 v_normal;out vec3 normal;out vec2 uv;uniform mat4 transform;uniform mat4 rot;uniform mat4 view;uniform mat4 projection;void main(){gl_Position=projection*view*transform*vec4(v_pos, 1.0);normal=(rot*vec4(v_normal, 1.0)).xyz;uv=v_uv;}\n";
const char *fshader = "#version 330 core\nlayout(location=0) out vec4 col;in vec3 normal;in vec2 uv;uniform sampler2D diff;void main(){float d=(dot(normal, normalize(vec3(1.0, 1.5, 0.2))) + 1.0) / 2.0;col=vec4(texture(diff, uv).rgb * d, 1.0);}\n";

struct kvasir_demo : kvasir_engine
{
	camera3d cam;
	mesh3d cam_mesh;
	mesh3d ground;
	mesh3d ramp;
	std::vector<mesh3d *> mshs;
	shader_base *shader = nullptr;
	physics3d *p3d = nullptr;

	user_result on_start()
	{
		// base->set_fullscreen();
		fixed_time.set_fps(5);
		time.set_fps(144);
		cam.far = 1000;
		base->set_clear_colour(0x0f0f0f);
		base->depth_buffer_active(true);
		p3d = default_physics3d();
		if (!p3d)
			return user_result("Physics was null.");

		if (!ground.load_from_obj(RESOURCE("../res/models/cube.obj"), base->make_buffer()))
			return user_result("Ground failed loading.");
		ground.pos.y() = -2;
		ground.material = make_material(base, RESOURCE("../res/img/h.png"));
		vec3f scale(300.f, 2.f, 300.f);
		ground.vertex_scale(scale);
		p3d->add_mesh_box_hitbox(ground, scale, physics3d::static_props());

		cam_mesh.pos.z() = -7;
		cam.pos = cam_mesh.pos;
		p3d->add_mesh_sphere_hitbox(cam_mesh, 1.f, physics3d::dynamic_props(10));

		if (!ramp.load_from_obj(RESOURCE("../res/models/ramp.obj"), base->make_buffer()))
			return user_result("Ground failed loading.");
		ramp.pos.z() = 30;
		ramp.material = make_material(base, 0x80bf80);
		p3d->add_mesh(ramp, false, physics3d::static_props());

		for (size_t i = 0; i < 20; ++i)
		{
			mesh3d *ms = new mesh3d();
			if (!ms->load_from_obj(RESOURCE("../res/models/cube.obj"), base->make_buffer()))
				return user_result("Ground failed loading.");
			ms->pos = vec3f((float)(i % 2) * 0.4f, (float)i * 3.f + 2, (float)(i % 3) * 0.4f);
			ms->material = make_material(base, RESOURCE("../res/img/skak.png"));
			p3d->add_mesh_box_hitbox(*ms, vec3f(1, 1, 1), physics3d::dynamic_props(1));
			mshs.push_back(ms);
		}

		shader = base->make_shader();
		const char *s[2]{vshader, fshader};
		if (!shader->compile(s, 2))
			return user_result("Shader failed compiling");

		return user_result::ok();
	}
	void on_update()
	{
		mesh3d *first_hit = p3d->raycast_first_hit(cam_mesh.pos, cam_mesh.pos - vec3f(0, 1.01f, 0));
		if (first_hit)
		{
			if (base->key_pressed(Space))
				p3d->add_central_force(cam_mesh, vec3f(0, 8 * p3d->get_mass(cam_mesh), 0));
			vec3f clv = cam.look_vector().xyz().normalized();
			vec3f forward;
			vec3f up = vec3f(0.f, 1.f, 0.f);
			if (base->key_pressed(KeyW))
				forward += clv;
			if (base->key_pressed(KeyS))
				forward -= clv;
			if (base->key_pressed(KeyD))
				forward -= clv.cross(up);
			if (base->key_pressed(KeyA))
				forward += clv.cross(up);
			if (base->key_pressed(KeyQ))
				forward.y() += 1.f;
			if (base->key_pressed(KeyE))
				forward.y() -= 1.f;
			vec3f vel = p3d->get_velocity(cam_mesh);
			if (vel.len() > 34.f)
				p3d->set_velocity(cam_mesh, vel.normalized() * 34.f);
			p3d->add_central_force(cam_mesh, forward.normalize() * p3d->get_mass(cam_mesh));
		}

		for (size_t i = 0; i < mshs.size(); ++i)
			if ((mshs[i]->pos - cam.pos).len() <= 4)
				p3d->activate(*mshs[i]);
		if (base->key_pressed(Num1))
			for (size_t i = 0; i < mshs.size(); ++i)
				p3d->add_central_force(*mshs[i], (cam.pos - mshs[i]->pos).normalized());

		p3d->step(time.delta());

		cam_debug_rotation(base, cam, time.delta());
		cam_mesh.pos = p3d->get_position(cam_mesh);
		cam.pos = cam_mesh.pos;
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
			base->should_close();
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