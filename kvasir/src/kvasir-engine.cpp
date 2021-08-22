
#include "kvasir-engine.h"

bool kvasir::kvasir_init()
{
	texture_base::init();
	mesh3d::init();
	return true;
}
void kvasir::kvasir_destroy()
{
	texture_base::destroy();
	mesh3d::destroy();
}

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
	DEL_PTR(base);
}
render_base *kvasir_engine::get_base(render_base::type b)
{
	switch (b)
	{
	case render_base::OPENGL:
#ifdef BUILD_OPENGL
		return new gl_render_base();
#else
		std::cerr << "This Kvasir version was not compiled with OpenGL rendering." << std::endl;
		break;
#endif
	case render_base::VULKAN:
#ifdef BUILD_VULKAN
		return new vulkan_render_base();
#else
		std::cerr << "This Kvasir version was not compiled with Vulkan rendering." << std::endl;
		break;
#endif
	case render_base::TERMINAL:
#ifdef BUILD_TERMINAL
		return new terminal_render_base();
#else
		std::cerr << "This Kvasir version was not compiled with Terminal rendering." << std::endl;
		break;
#endif
	case render_base::DIRECTX:
#ifdef BUILD_DIRECTX
		return nullptr;
#else
		std::cerr << "This Kvasir version was not compiled with DirectX rendering." << std::endl;
		break;
#endif
	default:
		break;
	}
	return nullptr;
}
kvasir_engine::result kvasir_engine::start_with_base()
{
	fixed_time.set_fps(30);
	user_result res = on_start();
	if (res.fatal)
	{
		std::cerr << res.msg << std::endl;
		return ON_START_RET_FALSE;
	}
	time.next_frame_ready();
	fixed_time.next_frame_ready();
	for (;;)
	{
		if (time.next_frame_ready())
		{
			if (base->should_close())
				break;
			base->poll_events();
			on_update();
		}
		if (fixed_time.next_frame_ready())
			on_fixed_update();
	}
	on_close();
	return NO_ERROR;
}
kvasir_engine::result kvasir_engine::start(render_base::type t_base, const char *name, int wid, int hei)
{
	DEL_PTR(base);
	base = get_base(t_base);
	if (!base)
		return NULL_BASE;
	if (!base->init(name, wid, hei))
		return BASE_INIT_FAIL;
	std::cout << render_base::type_to_string(base->get_type()) << " initialization SUCCESS." << std::endl;
	return start_with_base();
}
kvasir_engine::result kvasir_engine::start(std::vector<render_base::type> t_bases, const char *name, int wid, int hei)
{
	DEL_PTR(base);
	for (size_t i = 0; i < t_bases.size(); ++i)
	{
		base = get_base(t_bases[i]);
		if (!base)
			continue;
		if (!base->init(name, wid, hei))
		{
			std::cout << render_base::type_to_string(t_bases[i]) << " initialization FALIURE." << std::endl;
			DEL_PTR(base);
			continue;
		}
		else
			break;
	}
	if (!base)
		return NULL_BASE;
	std::cout << render_base::type_to_string(base->get_type()) << " initialization SUCCESS." << std::endl;
	return start_with_base();
}

physics3d *kvasir_engine::default_physics3d()
{
#ifdef BUILD_BULLET
	return new bullet_physics3d();
#else
	return nullptr;
#endif
}