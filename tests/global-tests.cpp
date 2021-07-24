
#include <iostream>
#include "kvasir-eng-include.h"

COUNT_MEMORY

using namespace kvasir;

const char *vshader = "#version 330 core\nlayout (location = 0) in vec3 v_pos;layout (location = 1) in vec2 v_uv;layout (location = 2) in vec3 v_normal;out vec3 normal;out vec2 uv;uniform mat4 transform;uniform mat4 rot;uniform mat4 view;uniform mat4 projection;void main(){gl_Position=projection*view*transform*vec4(v_pos, 1.0);normal=(rot*vec4(v_normal, 1.0)).xyz;uv=v_uv;}\n";
const char *fshader = "#version 330 core\nlayout (location = 0) out vec4 col;in vec3 normal;in vec2 uv;uniform sampler2D diff;void main(){float d=(dot(normal, vec3(0.0, 1.0, 0.0)) + 1.0) / 2.0;col=vec4(texture(diff, uv).rgb * d, 1.0);}\n";

struct kvasir_demo : kvasir_engine
{
	camera3d cam;
	mesh3d mesh;
	shader_base *shader = nullptr;

	user_result on_start()
	{
		cam.aspect = (float)base->get_aspect();
		base->set_clear_colour(0xff80ff);
		base->depth_buffer_active(true);

		if (!mesh.load_from_obj("../res/models/cube.obj"))
			return user_result("Mesh failed loading.");
		mesh.load_to_buffer(base->make_buffer());
		mesh.pos.z() = 4;
		mesh.material = base->make_material();
		mesh.material->diffuse() = base->make_texture();
		mesh.material->diffuse()->make_png("../res/img/h.png");

		shader = base->make_shader();
		const char *s[2]{vshader, fshader};
		if (!shader->compile(s, 2))
			return user_result("Shader failed compiling");
		return user_result::ok();
	}
	void on_update()
	{
		mesh.rot += vec3f{1.f, 1.f, 1.f} * (float)time.delta();
		base->clear();
		base->render_mesh3d(cam, mesh, shader);
		base->swap_buffers();
	}
	void on_close()
	{
		delete shader;
	}
};

int main(int, char **)
{
	{
		linkverify().verify_link();
		kvasir_demo kvs;
		kvasir_engine::result res = kvs.start(std::vector<renderer_base::type>{
			renderer_base::VULKAN,
			renderer_base::OPENGL});
		if (res != kvasir_engine::NO_ERROR)
			std::cerr << "Kvasir engine crashed with code " << res << std::endl;
	}

	PRINT_MEMORY_SUMMARY
}