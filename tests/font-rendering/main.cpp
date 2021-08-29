#include "kvasir-eng-include.h"

using namespace kvasir;

COUNT_MEMORY

int main(int, char **)
{
	{
		frame_manager fm;
		bool running = true;
		render_base *base = kvasir_engine::get_base(render_base::OPENGL);
		if (base)
		{
			if (base->init("Kvasir", 720, 480))
			{
				/////////////
				base->set_clear_colour(0xafafaf);

				shader_base *shader = base->make_shader();
				const char *s[2] GL_SHADER_ARR(diffuse2d);
				if (!shader->compile(s, 2))
					return -1;

				camera2d cam;
				/////////////
				while (running)
				{
					if (fm.next_frame_ready())
					{
						////////////////
						if (base->get_type() == render_base::TERMINAL)
							cam.aspect = (float)base->get_aspect() * 2.f;
						else
							cam.aspect = (float)base->get_aspect();
						base->clear();
						base->swap_buffers();
						////////////////
					}
					base->poll_events();
					if (base->should_close())
						break;
				}
				////////////////
				DEL_PTR(shader)
				////////////////
			}
			DEL_PTR(base)
		}
	}
	PRINT_MEMORY_SUMMARY
	return 0;
}