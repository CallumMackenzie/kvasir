
#include <iostream>
#include "kvasir-engine.h"

using namespace kvasir;

void linkverify::verify_link()
{
	std::cout << "LINK VERIFIED FOR KVASIR" << std::endl;
}

kvasir_engine::user_result::user_result(const char *m, bool f)
{
	msg = m;
	fatal = f;
}
kvasir_engine::user_result kvasir_engine::user_result::ok()
{
	return kvasir_engine::user_result(nullptr, false);
}

kvasir_engine::kvasir_engine(renderer_base_type base_type)
{
	set_base(base_type);
}
kvasir_engine::~kvasir_engine()
{
	if (base)
		delete base;
}
bool kvasir_engine::set_base(renderer_base_type base_type)
{
	if (this->base)
		delete this->base;
	switch (base_type)
	{
	case renderer_base_type::OPENGL:
		this->base = new gl_render_base();
		return true;
	default:
		this->base = nullptr;
		return false;
	}
}
kvasir_engine::result kvasir_engine::start(const char *name, int wid, int hei)
{
	if (!base)
		return NULL_BASE;
	if (!base->init(name, wid, hei))
		return BASE_INIT_FAIL;
	user_result res = on_start();
	if (res.fatal)
	{
		std::cerr << res.msg << std::endl;
		return ON_START_RET_FALSE;
	}
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
