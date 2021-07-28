
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
	mesh3d mesh;
	mesh3d ground;
	shader_base *shader = nullptr;
	physics3d *p3d = default_physics3d();

	user_result on_start()
	{
		fixed_time.set_fps(10);
		base->set_clear_colour(0xff80ff);
		base->depth_buffer_active(true);

		if (!mesh.load_from_obj("../res/models/sphere.obj", base->make_buffer()))
			return user_result("Meshes failed loading.");
		mesh.pos.z() = 4;
		mesh.pos.y() = 1;
		mesh.material = base->make_material();
		mesh.material->texs[0] = base->make_texture();
		mesh.material->texs[0]->make_png("../res/img/h.png");
		p3d->add_mesh(mesh, true, 1);

		if (!ground.load_from_obj("../res/models/cube.obj", base->make_buffer()))
			return user_result("Ground failed loading.");
		ground.pos.z() = 4;
		ground.pos.y() = -2;
		ground.rot = vec4f::quaternion(0.2, vec3f(0.4, 1, 0.4));
		ground.material = base->make_material();
		ground.material->texs[0] = base->make_texture();
		ground.material->texs[0]->make_png("../res/img/h.png");
		ground.vertex_scale(vec3f(2, 0.1, 2));
		p3d->add_mesh(ground, true, 0);

		shader = base->make_shader();
		const char *s[2]{vshader, fshader};
		if (!shader->compile(s, 2))
			return user_result("Shader failed compiling");
		return user_result::ok();
	}
	void on_update()
	{
		p3d->step(time.delta());
		mesh.pos = p3d->get_position(mesh);
		mesh.rot = p3d->get_rotation(mesh);
		ground.pos = p3d->get_position(ground);
		ground.rot = p3d->get_rotation(ground);
		base->clear();
		base->render_mesh3d(cam, mesh, shader);
		base->render_mesh3d(cam, ground, shader);
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
	}
};

int main(int, char **)
{
	{
		kvasir_init();
		linkverify().verify_link();
		kvasir_demo kvs;
		kvasir_engine::result res = kvs.start(std::vector<render_base::type>{
			render_base::VULKAN,
			render_base::TERMINAL,
			render_base::OPENGL});
		if (res != kvasir_engine::NO_ERROR)
			std::cerr << "Kvasir engine crashed with code " << res << std::endl;
	}

	PRINT_MEMORY_SUMMARY
}