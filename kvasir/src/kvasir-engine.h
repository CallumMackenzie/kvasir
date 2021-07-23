#ifndef KVASIR_ENGINE_H_
#define KVASIR_ENGINE_H_ 1

#include "renderer.h"
#include "gl-renderer.h"
#include "galg.h"
#include "fps-manager.h"

namespace kvasir
{

	struct linkverify
	{
		void verify_link();
	};

	struct kvasir_engine
	{
		frame_manager time;
		renderer_base *base;
		enum result
		{
			NO_ERROR = 0x001,
			NULL_BASE = 0x002,
			BASE_INIT_FAIL = 0x003,
			ON_START_RET_FALSE = 0x004
		};

		struct user_result
		{
			user_result(const char *m, bool f = true);
			const char *msg = nullptr;
			bool fatal = true;
			static user_result ok();
		};

		kvasir_engine(renderer_base_type base_type);
		~kvasir_engine();
		result start(const char *name = "Kvasir", int wid = 480, int hei = 480);
		bool set_base(renderer_base_type base_type);
		virtual user_result on_start() = 0;
		virtual void on_update() = 0;
		virtual void on_close() = 0;
	};
}
#endif