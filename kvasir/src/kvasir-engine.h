#ifndef KVASIR_ENGINE_H_
#define KVASIR_ENGINE_H_ 1

#include <vector>
#ifndef NO_USE_INCLUDES
#include "renderer.h"
#include "gl-renderer.h"
#include "vulkan-renderer.h"
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
		renderer_base *base = nullptr;
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
		result start(std::vector<renderer_base::type> t_bases, const char *name = "Kvasir", int wid = 480, int hei = 480);
		result start(renderer_base::type t_base, const char *name = "Kvasir", int wid = 480, int hei = 480);
		virtual user_result on_start() = 0;
		virtual void on_update() = 0;
		virtual void on_fixed_update() = 0;
		virtual void on_close() = 0;
		static renderer_base *get_base(renderer_base::type b);
		private:
		result start_with_base();
	};
}
#endif