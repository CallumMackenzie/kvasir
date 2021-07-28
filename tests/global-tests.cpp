
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
	physics3d *p3d = nullptr;

	user_result on_start()
	{
		fixed_time.set_fps(10);
		base->set_clear_colour(0xff80ff);
		base->depth_buffer_active(true);
		std::cout << allocs << ", " << deletes << std::endl;
		p3d = default_physics3d();

		if (!mesh.load_from_obj(R_PATH("../res/models/sphere.obj").c_str(), base->make_buffer()))
			return user_result("Meshes failed loading.");
		mesh.pos.z() = 4;
		mesh.pos.y() = 1;
		mesh.material = base->make_material();
		mesh.material->texs[0] = base->make_texture();
		mesh.material->texs[0]->make_png(R_PATH("../res/img/h.png").c_str());
		p3d->add_mesh(mesh, true, 1);

		if (!ground.load_from_obj(R_PATH("../res/models/cube.obj").c_str(), base->make_buffer()))
			return user_result("Ground failed loading.");
		ground.pos.z() = 4;
		ground.pos.y() = -2;
		ground.rot = vec4f::quaternion(0.2f, vec3f(0.4f, 1.f, 0.4f));
		ground.material = base->make_material();
		ground.material->texs[0] = base->make_texture();
		ground.material->texs[0]->make_png(R_PATH("../res/img/h.png").c_str());
		ground.vertex_scale(vec3f(2.f, 0.1f, 2.f));
		p3d->add_mesh(ground, true, 0);

		shader = base->make_shader();
		const char *s[2]{vshader, fshader};
		if (!shader->compile(s, 2))
			return user_result("Shader failed compiling");
		return user_result::ok();
	}
	void on_update()
	{

		vec3f clv = cam.look_vector().xyz().normalized();
		float speed = 3.f;
		float cms = 1.3f;
		vec3f forward;
		vec3f up = vec3f(0.f, 1.f, 0.f);
		vec3f rotate;
		if (base->get_keystate(KeyW) == KeyDown) // w
			forward += clv;
		if (base->get_keystate(KeyS) == KeyDown) // s
			forward -= clv;
		if (base->get_keystate(KeyD) == KeyDown) // d
			forward -= clv.cross(up);
		if (base->get_keystate(KeyA) == KeyDown) // a
			forward += clv.cross(up);
		if (base->get_keystate(Space) == KeyDown)
			forward.y() += 1.f;
		if (base->get_keystate(KeyE) == KeyDown)
			forward.y() -= 1.f;

		if (base->get_keystate(Left) == KeyDown) // Arrow left
			rotate.y() = cms;
		if (base->get_keystate(Right) == KeyDown) // Arrow right
			rotate.y() = -cms;
		if (base->get_keystate(Up) == KeyDown) // Arrow up
			rotate.x() = -cms;
		if (base->get_keystate(Down) == KeyDown) // Arrow down
			rotate.x() = cms;
		if (base->get_keystate(LShift) == KeyDown)
			speed *= 3.f;
		if (base->get_keystate(LControl) == KeyDown)
			speed *= 7.f;

		cam.rot += rotate * time.delta();
		cam.pos += forward.normalize() * speed * time.delta();

		p3d->set_transform(mesh, mesh);
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
		// linkverify().verify_link();
		// kvasir_init();
		kvasir_demo kvs;
		kvasir_engine::result res = kvs.start(render_base::OPENGL);
		if (res != kvasir_engine::NO_ERROR)
			std::cerr << "Kvasir engine crashed with code " << res << std::endl;
	}

	PRINT_MEMORY_SUMMARY
}