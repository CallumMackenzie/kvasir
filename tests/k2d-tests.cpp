#include "kvasir-eng-include.h"

// COUNT_MEMORY

using namespace kvasir;

const char *vshader = "#version 330 core\nlayout(location=0) in vec2 vpos;layout(location=1) in vec2 vuv;uniform float aspect;uniform vec2 pos;uniform mat2 transform;out vec2 uv; void main() {uv = vuv;vec4 tx=vec4(((vpos * transform) + pos), 0.0, 1.0);tx.x *= aspect;gl_Position=tx;}\n";
const char *fshader = "#version 330 core\nlayout(location=0) out vec4 col; in vec2 uv; uniform sampler2D diff;void main() {col=texture(diff, uv).rgba;}\n";

int main(int, char **)
{
	// Testing no engine
	{
		frame_manager fm;
		bool running = true;
		render_base *base = kvasir_engine::get_base(render_base::OPENGL);
		if (base)
			if (base->init("Kvasir", 720, 480))
			{
				/////////////
				mesh2d m2;
				m2.make_quad(vec2f(1, 1), base->make_buffer());
				m2.material = make_material(base, 0x00aaff);
				base->set_clear_colour(0xafafaf);

				shader_base *shader = base->make_shader();
				const char *s[2]{vshader, fshader};
				if (!shader->compile(s, 2))
					return -1;

				camera2d cam;
				/////////////
				while (running)
				{
					if (fm.next_frame_ready())
					{
						////////////////
						float rspeed = 0.5f;
						float mspeed = 0.6f;
						if (base->key_pressed(Left))
							cam.rot -= rspeed * fm.delta();
						if (base->key_pressed(Right))
							cam.rot += rspeed * fm.delta();
						if (base->key_pressed(KeyW))
							cam.pos.y() += mspeed * fm.delta();
						if (base->key_pressed(KeyS))
							cam.pos.y() -= mspeed * fm.delta();
						if (base->key_pressed(KeyD))
							cam.pos.x() += mspeed * fm.delta();
						if (base->key_pressed(KeyA))
							cam.pos.x() -= mspeed * fm.delta();

						m2.rot += 1.f * fm.delta();

						if (base->get_type() == render_base::TERMINAL)
							cam.aspect = (float)base->get_aspect() * 2.f;
						else
							cam.aspect = (float)base->get_aspect();
						base->clear();
						base->render_mesh2d(cam, m2, shader);
						base->swap_buffers();
						////////////////
					}
					base->poll_events();
					if (base->should_close())
						break;
				}
				////////////////
				DEL_PTR(shader)
				DEL_PTR(base)
				////////////////
			}
	}
	// PRINT_MEMORY_SUMMARY
	return 0;
}