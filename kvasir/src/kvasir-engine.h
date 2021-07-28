#ifndef KVASIR_ENGINE_H_
#define KVASIR_ENGINE_H_ 1

#include <vector>
#include <iostream>
#ifndef NO_USE_INCLUDES
#include "renderer.h"
#include "physics.h"
#include "bullet-physics3d.h"
#include "gl-renderer.h"
#include "vulkan-renderer.h"
#include "terminal-renderer.h"
#include "galg.h"
#include "fps-manager.h"
#include "memory-aid.h"
#endif

namespace kvasir
{

	bool kvasir_init();

	struct linkverify
	{
		void verify_link();
	};

	struct kvasir_engine
	{
		frame_manager time;
		frame_manager fixed_time;
		render_base *base = nullptr;
		enum result
		{
			NO_ERROR = 0,
			NULL_BASE = 1,
			BASE_INIT_FAIL = 2,
			ON_START_RET_FALSE = 4
		};

		struct user_result
		{
			user_result(const char *m, bool f = true);
			const char *msg = nullptr;
			bool fatal = true;
			static user_result ok();
		};

		~kvasir_engine();
		result start(std::vector<render_base::type> t_bases, const char *name = "Kvasir", int wid = 480, int hei = 480);
		result start(render_base::type t_base, const char *name = "Kvasir", int wid = 480, int hei = 480);
		virtual user_result on_start() = 0;
		virtual void on_update() = 0;
		virtual void on_fixed_update() = 0;
		virtual void on_close() = 0;
		static render_base *get_base(render_base::type b);

		static physics3d *default_physics3d();

	private:
		result start_with_base();
	};
}
#endif