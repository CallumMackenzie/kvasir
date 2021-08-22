#ifndef KVASIR_COMMON_OPS_H
#define KVASIR_COMMON_OPS_H 1

#ifndef NO_USE_INCLUDES
#include "config.h"
#include "galg.h"
#include "renderer.h"
#include "mesh.h"
#include "camera.h"
#endif

namespace kvasir
{
	KV_EXPORT void cam_debug_controls(render_base *base, camera3d &cam, float delta, float mv_speed = 3.f, float rot_speed = 1.4f, float spb1 = 3.f, float spb2 = 7.f);
	KV_EXPORT void cam_debug_rotation(render_base *base, camera3d &cam, float delta, float rot_speed = 1.4f);
	KV_EXPORT material_base *make_material(render_base *base, const char *diffuse_img_path);
	KV_EXPORT material_base *make_material(render_base *base, long diffuse_colour);
}

#endif