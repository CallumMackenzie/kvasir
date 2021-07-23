#ifndef KVASIR_ENGINE_H_
#define KVASIR_ENGINE_H_ 1

#include "renderer.h"
#include "gl-renderer.h"
#include "galg.h"
#include "fps-manager.h"

namespace kvasir
{
	struct kvasir_engine
	{
		frame_manager time;
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
				base = new gl_render_base();
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

		result start(int wid = 480, int hei = 480)
		{
			if (!base)
				return NULL_BASE;
			if (!base->init("Kvasir", wid, hei))
				return BASE_INIT_FAIL;
				on_start();
			for (;;)
			{
				if (time.next_frame_ready())
				{
					if (base->should_close())
						break;
					base->poll_events();
					on_update();
				}
			}
			on_close();
			return NO_ERROR;
		}

		virtual void on_start() = 0;
		virtual void on_update() = 0;
		virtual void on_close() = 0;
	};
}
#endif