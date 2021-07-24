
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
kvasir_engine::~kvasir_engine()
{
	if (base)
		delete base;
	base = nullptr;
}
renderer_base *kvasir_engine::get_base(renderer_base::type b)
{
	switch (b)
	{
	case renderer_base::OPENGL:
		return new gl_render_base();
	case renderer_base::VULKAN:
		return new vulkan_render_base();
	default:
		return nullptr;
	}
}
kvasir_engine::result kvasir_engine::start_with_base()
{
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
kvasir_engine::result kvasir_engine::start(renderer_base::type t_base, const char *name, int wid, int hei)
{
	if (base)
	{
		delete base;
		base = nullptr;
	}
	base = get_base(t_base);
	if (!base)
		return NULL_BASE;
	if (!base->init(name, wid, hei))
		return BASE_INIT_FAIL;
	return start_with_base();
}

kvasir_engine::result kvasir_engine::start(std::vector<renderer_base::type> t_bases, const char *name, int wid, int hei)
{
	if (base)
	{
		delete base;
		base = nullptr;
	}
	for (size_t i = 0; i < t_bases.size(); ++i)
	{
		base = get_base(t_bases[i]);
		if (!base)
			continue;
		if (!base->init(name, wid, hei))
		{
			std::cout << "Base failed initialization: base code " << t_bases[i] << std::endl;
			delete base;
			base = nullptr;
			continue;
		}
		else
			break;
	}
	if (!base)
		return NULL_BASE;
	return start_with_base();
}
