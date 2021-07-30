
#include <iostream>
#include <cstdlib>
#include "kvasir-eng-include.h"

COUNT_MEMORY

using namespace kvasir;

const char *vshader = "#version 330 core\nlayout(location=0) in vec3 v_pos;layout (location = 1) in vec2 v_uv;layout (location = 2) in vec3 v_normal;out vec3 normal;out vec2 uv;uniform mat4 transform;uniform mat4 rot;uniform mat4 view;uniform mat4 projection;void main(){gl_Position=projection*view*transform*vec4(v_pos, 1.0);normal=(rot*vec4(v_normal, 1.0)).xyz;uv=v_uv;}\n";
const char *fshader = "#version 330 core\nlayout(location=0) out vec4 col;in vec3 normal;in vec2 uv;uniform sampler2D diff;void main(){float d=(dot(normal, vec3(0.0, 1.0, 0.0)) + 1.0) / 2.0;col=vec4(texture(diff, uv).rgb * d, 1.0);}\n";

struct kvasir_demo : kvasir_engine
{
	camera3d cam;
	mesh3d ground;
	std::vector<mesh3d *> mshs;
	shader_base *shader = nullptr;
	physics3d *p3d = nullptr;
	vec3f crot;

	user_result on_start()
	{
		fixed_time.set_fps(10);
		time.set_fps(1000);
		cam.far = 1000;
		base->set_clear_colour(0x0f0f0f);
		base->depth_buffer_active(true);
		p3d = default_physics3d();

		if (!ground.load_from_obj(RESOURCE("../res/models/cube.obj"), base->make_buffer()))
			return user_result("Ground failed loading.");
		ground.pos.y() = -2;
		ground.material = make_material(base, RESOURCE("../res/img/h.png"));
		ground.vertex_scale(vec3f(100.f, 1.f, 100.f));
		p3d->add_mesh_box_hitbox(ground, vec3f(100.f, 1.f, 100.f), physics3d::static_props());

		for (size_t i = 0; i < 30; ++i)
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

		cam_debug_controls(base, cam, time.delta());
		if (base->key_pressed(Num1))
			for (size_t i = 0; i < mshs.size(); ++i)
				p3d->add_central_force(*mshs[i], (cam.pos - mshs[i]->pos).normalized());

		p3d->step(time.delta());
		for (size_t i = 0; i < mshs.size(); ++i)
		{
			mshs[i]->pos = p3d->get_position(*mshs[i]);
			mshs[i]->rot = p3d->get_rotation(*mshs[i]);
		}
		std::cout << "FPS: " << (1.f / time.delta()) << std::endl;

		base->clear();
		base->render_mesh3d(cam, ground, shader);
		for (size_t i = 0; i < mshs.size(); ++i)
			base->render_mesh3d(cam, *mshs[i], shader);
		base->swap_buffers();
	}
	void on_fixed_update()
	{
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
		kvasir_engine::result res = kvs.start(render_base::OPENGL);
		if (res != kvasir_engine::NO_ERROR)
			std::cerr << "Kvasir engine crashed with code " << res << std::endl;
	}

	PRINT_MEMORY_SUMMARY
}