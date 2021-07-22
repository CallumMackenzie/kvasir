#ifndef KVASIR_ENGINE_H_
#define KVASIR_ENGINE_H_ 1

#include "renderer.h"
#include "galg.h"
#include "fps-manager.h"

namespace kvasir
{
	struct kvasir_engine
	{
		frame_manager f_manager;
		renderer_base *base;
		enum result
		{
			NO_ERROR = 0,
			NULL_BASE = 1,
			BASE_INIT_FAIL = 2
		};

		kvasir_engine(renderer_base_type base_type)
		{
			switch (base_type)
			{
			case renderer_base_type::OPENGL:
				base = new gl_base();
				break;
			default:
				base = nullptr;
			}
		}
		~kvasir_engine()
		{
			if (base)
				delete base;
		}

		result start()
		{
			if (!base)
				return NULL_BASE;
			if (!base->init("Kvasir", 720, 480))
				return BASE_INIT_FAIL;
			base->set_clear_colour(0xff00ff);
			base->set_fullscreen();
			base->set_windowed();
			base->set_size(720, 480);
			base->set_resizable(false);
			base->set_visible(false);
			base->set_resizable(true);
			base->set_visible(true);
			for (;;)
			{
				if (f_manager.next_frame_ready())
				{
					if (base->should_close())
						break;
					base->poll_events();
					base->clear();
					base->swap_buffers();
				}
			}
			return NO_ERROR;
		}
	};
}
#endif